/****************************************************************************
 * arch/arm/src/da1470x/da1470x_bt.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/* NuttX Bluetooth driver over the CMAC mailbox.  The controller speaks
 * H4-framed HCI: a packet type byte followed by the standard HCI header
 * and parameters.  Outgoing packets are serialised through a small
 * buffer; incoming bytes are reassembled into packets in the interrupt
 * and handed to the stack from the high-priority work queue.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/mutex.h>
#include <nuttx/signal.h>
#include <nuttx/wqueue.h>
#include <nuttx/wireless/bluetooth/bt_driver.h>
#include <nuttx/wireless/bluetooth/bt_hci.h>

#include "da1470x_cmac.h"
#include "da1470x_bt.h"

#ifdef CONFIG_DA1470X_BLE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* H4 packet indicators */

#define H4_CMD          0x01
#define H4_ACL          0x02
#define H4_SCO          0x03
#define H4_EVT          0x04

#define H4_CMD_HDR_LEN  3
#define H4_ACL_HDR_LEN  4
#define H4_EVT_HDR_LEN  2

/* Largest packet we accept from the controller */

#define BT_RX_MAX       (4 + 1 + 255 + 8)
#define BT_TX_MAX       (4 + 1 + 255)

/* Ring of received bytes, filled from the interrupt */

#define BT_RXRING_SIZE  2048

/* Default public address when the board does not provide one */

#define BT_DEFAULT_BDADDR { 0x01, 0x00, 0x00, 0xca, 0xea, 0x80 }

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct da1470x_bt_s
{
  struct bt_driver_s btdev;
  bool open;

  /* Received bytes ring (interrupt -> work queue) */

  uint8_t  rxring[BT_RXRING_SIZE];
  volatile uint16_t rxhead;
  volatile uint16_t rxtail;
  struct work_s rxwork;

  /* Packet being reassembled */

  uint8_t  pkt[BT_RX_MAX];
  size_t   pktlen;

  /* Outgoing packet copy: the mailbox keeps a pointer while pending */

  uint8_t  txbuf[BT_TX_MAX];
  mutex_t  txlock;

  uint8_t  bdaddr[6];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  bt_open(struct bt_driver_s *btdev);
static int  bt_send(struct bt_driver_s *btdev, enum bt_buf_type_e type,
                    void *data, size_t len);
static void bt_close(struct bt_driver_s *btdev);
static void bt_rxwork(void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_bt_s g_bt =
{
  .btdev =
  {
    .head_reserve = 1,        /* Room for the H4 type byte */
    .open         = bt_open,
    .send         = bt_send,
    .close        = bt_close,
  },
  .txlock = NXMUTEX_INITIALIZER,
  .bdaddr = BT_DEFAULT_BDADDR,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: bt_rxring_put
 *
 * Description:
 *   Interrupt-context callback from the mailbox layer.
 *
 ****************************************************************************/

static void bt_rxring_put(void *arg, const uint8_t *data, size_t len)
{
  struct da1470x_bt_s *priv = (struct da1470x_bt_s *)arg;
  uint16_t head = priv->rxhead;
  size_t i;

  for (i = 0; i < len; i++)
    {
      uint16_t next = (head + 1) % BT_RXRING_SIZE;

      if (next == priv->rxtail)
        {
          wlerr("RX ring overflow\n");
          break;
        }

      priv->rxring[head] = data[i];
      head = next;
    }

  priv->rxhead = head;

  if (work_available(&priv->rxwork))
    {
      work_queue(HPWORK, &priv->rxwork, bt_rxwork, priv, 0);
    }
}

/****************************************************************************
 * Name: bt_packet_length
 *
 * Description:
 *   Total length of the H4 packet whose beginning is in pkt, or 0 if the
 *   header is not complete yet, or -1 if the type is unknown.
 *
 ****************************************************************************/

static int bt_packet_length(const uint8_t *pkt, size_t have)
{
  switch (pkt[0])
    {
      case H4_EVT:
        if (have < 1 + H4_EVT_HDR_LEN)
          {
            return 0;
          }

        return 1 + H4_EVT_HDR_LEN + pkt[2];

      case H4_ACL:
        if (have < 1 + H4_ACL_HDR_LEN)
          {
            return 0;
          }

        return 1 + H4_ACL_HDR_LEN + (pkt[3] | (pkt[4] << 8));

      case H4_CMD:
        if (have < 1 + H4_CMD_HDR_LEN)
          {
            return 0;
          }

        return 1 + H4_CMD_HDR_LEN + pkt[3];

      default:
        return -1;
    }
}

/****************************************************************************
 * Name: bt_rxwork
 *
 * Description:
 *   Reassemble packets from the byte ring and deliver them to the stack.
 *
 ****************************************************************************/

static void bt_rxwork(void *arg)
{
  struct da1470x_bt_s *priv = (struct da1470x_bt_s *)arg;

  while (priv->rxtail != priv->rxhead)
    {
      int total;

      priv->pkt[priv->pktlen++] = priv->rxring[priv->rxtail];
      priv->rxtail = (priv->rxtail + 1) % BT_RXRING_SIZE;

      total = bt_packet_length(priv->pkt, priv->pktlen);
      if (total < 0)
        {
          wlerr("Unknown H4 type %02x, resynchronising\n", priv->pkt[0]);
          priv->pktlen = 0;
          continue;
        }

      if (total == 0 || (size_t)total > priv->pktlen)
        {
          if (priv->pktlen >= BT_RX_MAX)
            {
              wlerr("Packet too long, dropped\n");
              priv->pktlen = 0;
            }

          continue;
        }

      wlinfo("RX type %02x len %d\n", priv->pkt[0], total);

      if (priv->open)
        {
          enum bt_buf_type_e type =
            priv->pkt[0] == H4_EVT ? BT_EVT : BT_ACL_IN;

          bt_netdev_receive(&priv->btdev, type, priv->pkt + 1, total - 1);
        }

      priv->pktlen = 0;
    }
}

/****************************************************************************
 * Name: bt_open
 ****************************************************************************/

static int bt_open(struct bt_driver_s *btdev)
{
  struct da1470x_bt_s *priv = (struct da1470x_bt_s *)btdev;
  int ret;

  priv->rxhead = 0;
  priv->rxtail = 0;
  priv->pktlen = 0;

  ret = da1470x_cmac_start(priv->bdaddr, bt_rxring_put, priv);
  if (ret < 0)
    {
      wlerr("Failed to start the controller: %d\n", ret);
      return ret;
    }

  priv->open = true;
  return OK;
}

/****************************************************************************
 * Name: bt_send
 ****************************************************************************/

static int bt_send(struct bt_driver_s *btdev, enum bt_buf_type_e type,
                   void *data, size_t len)
{
  struct da1470x_bt_s *priv = (struct da1470x_bt_s *)btdev;
  uint8_t *hdr;
  int ret;
  int i;

  if (!priv->open)
    {
      return -ENODEV;
    }

  if (len + 1 > BT_TX_MAX)
    {
      return -E2BIG;
    }

  /* The stack reserved head_reserve bytes in front of data */

  hdr = (uint8_t *)data - 1;

  switch (type)
    {
      case BT_CMD:
        *hdr = H4_CMD;
        break;

      case BT_ACL_OUT:
        *hdr = H4_ACL;
        break;

      default:
        return -EINVAL;
    }

  ret = nxmutex_lock(&priv->txlock);
  if (ret < 0)
    {
      return ret;
    }

  /* The mailbox may hold a pointer to the data until the controller
   * drains it, so keep a private copy.
   */

  memcpy(priv->txbuf, hdr, len + 1);

  wlinfo("TX type %02x len %zu\n", *hdr, len + 1);

  ret = da1470x_cmac_write(priv->txbuf, len + 1);

  /* Wait for the mailbox to accept everything before reusing txbuf */

  for (i = 0; i < 1000 && da1470x_cmac_write_pending(); i++)
    {
      nxsig_usleep(1000);
    }

  nxmutex_unlock(&priv->txlock);
  return ret < 0 ? ret : (int)len;
}

/****************************************************************************
 * Name: bt_close
 ****************************************************************************/

static void bt_close(struct bt_driver_s *btdev)
{
  struct da1470x_bt_s *priv = (struct da1470x_bt_s *)btdev;

  priv->open = false;
  da1470x_cmac_stop();
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_bt_initialize
 ****************************************************************************/

int da1470x_bt_initialize(const uint8_t *bdaddr)
{
  if (bdaddr != NULL)
    {
      memcpy(g_bt.bdaddr, bdaddr, 6);
    }

  return bt_netdev_register(&g_bt.btdev);
}

#endif /* CONFIG_DA1470X_BLE */
