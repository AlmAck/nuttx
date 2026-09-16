/****************************************************************************
 * arch/arm/src/da1470x/da1470x_cmac.c
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

/* The DA1470x BLE controller runs on a separate Cortex-M0+ (the CMAC) from
 * a firmware image the host copies into RAM10.  Host and controller talk
 * through two 504-byte rings in the controller's data area, each with a
 * small header {magic, flags, write index, read index}, and signal each
 * other with the SYS2CMAC / CMAC2SYS interrupt bits.
 *
 * The boot handshake, the ring protocol and the fixed table addresses
 * below were recovered from the SDK's cmac_cpu_lld / cmac_mailbox objects
 * and belong to the firmware image "CMAC v1.0.0" of SDK 10.2.6.49.
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

#include "arm_internal.h"
#include "nvic.h"
#include "hardware/da1470x_crg_top.h"
#include "hardware/da1470x_crg_xtal.h"
#include "hardware/da1470x_memctrl.h"
#include "hardware/da1470x_memorymap.h"
#include "da1470x_clockconfig.h"
#include "da1470x_cmac.h"
#include "da1470x_pdc.h"
#include "da1470x_pmu.h"

#ifdef CONFIG_DA1470X_BLE

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Firmware area header (36 bytes) followed by the image */

#define CMAC_FW_HDR_SIZE          0x24
#define CMAC_FW_HDR_MAGIC         0x11223344
#define CMAC_FW_HDR_IMGSIZE_OFF   4
#define CMAC_FW_HDR_MAGIC_OFF     8

/* Where the image goes and the fixed tables inside it (firmware 1.0.0) */

#define CMAC_CODE_BASE            DA1470X_SRAM10_BASE     /* 0x20150000 */
#define CMAC_AREA_SIZE            0x23024
#define CMAC_CONFIG_TABLE         0x2016a0d8
#define CMAC_DYNAMIC_TABLE        0x20172bb4
#define CMAC_INFO_TABLE           0x20172b6c
#define CMAC_TCS_TABLE            0x20171d74
#define CMAC_SYS_TCS_TABLE        0x20171d84
#define CMAC_EXCEPTION_CTX        0x20171dc4

/* Host-side tables the controller dereferences.  They must sit inside the
 * controller's data window (above RAM8), so they are placed in RAM10
 * right after the firmware area, which the linker reserves.
 */

#define CMAC_HOST_TCS_ATTR        0x20174000   /* uint32_t[12] */
#define CMAC_HOST_TCS_DATA        0x20174040   /* uint32_t[4] */
#define CMAC_HOST_TEMPSENS        0x20174050   /* uint32_t */

/* Mailbox blocks: {u16 magic; u16 flags; u16 wr; u16 rd; u8 buf[504]} */

#define CMAC_MBOX_SYS2CMAC        0x2017296c   /* Host writes */
#define CMAC_MBOX_CMAC2SYS        0x2017276c   /* Host reads */
#define CMAC_MBOX_BUFSIZE         504
#define CMAC_MBOX_MAGIC           0xa55a

#define CMAC_MBOX_FLAG_HCI        0x01
#define CMAC_MBOX_FLAG_ERROR      0x02
#define CMAC_MBOX_FLAG_FLOW       0x04
#define CMAC_MBOX_FLAG_RESET      0x08
#define CMAC_MBOX_FLAG_WRITE_PEND 0x10

/* CMAC status word visible to the host; bits 7-9 flag a controller fault */

#define CMAC_STATUS_REG           (DA1470X_CMAC_BASE + 0x2000)
#define CMAC_STATUS_ERROR_MASK    0x380
#define CMAC_STATUS_READY         (1u << 31)

/* Bounded waits */

#define CMAC_WAIT_LOOPS           4000000

/* Configuration table field offsets (cmac_configuration_table_t) */

#define CFG_WAIT_ON_MAIN          0
#define CFG_BD_ADDRESS            1
#define CFG_RF_CAL_DELAY          7
#define CFG_LP_CLOCK_FREQ         8
#define CFG_LP_CLOCK_DRIFT        10
#define CFG_RX_BUFFER_SIZE        12
#define CFG_TX_BUFFER_SIZE        14
#define CFG_LENGTH_EXCHANGE       16
#define CFG_SYSTEM_TCS_LENGTH     26
#define CFG_SYNTH_TCS_LENGTH      27
#define CFG_RFCU_TCS_LENGTH       28
#define CFG_INITIAL_TX_POWER      29
#define CFG_DUP_FILTER_MAX        30

/* Dynamic table offsets (cmac_dynamic_configuration_table_t) */

#define DYN_SLEEP_ENABLE          0
#define DYN_POWER_CTRL_WAKEUP     0x20
#define DYN_POWER_CTRL_SLEEP      0x24
#define DYN_POWER_LVL_WAKEUP      0x28
#define DYN_POWER_LVL_SLEEP       0x2c
#define DYN_TEMPSENS_PTR          0x30
#define DYN_MACCPU_STATE          0x34

/* TCS table (cmac_tcs_table_t) */

#define TCS_ATTR_PTR              0
#define TCS_ATTR_SIZE             4
#define TCS_DATA_PTR              8
#define TCS_DATA_SIZE             12
#define TCS_GROUP_MAX             12

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct cmac_mbox_s
{
  volatile uint16_t magic;
  volatile uint16_t flags;
  volatile uint16_t wr;
  volatile uint16_t rd;
  volatile uint8_t  buf[CMAC_MBOX_BUFSIZE];
};

struct da1470x_cmac_s
{
  struct cmac_mbox_s *tx;       /* Host -> controller ring */
  struct cmac_mbox_s *rx;       /* Controller -> host ring */
  uint16_t last_rd;             /* Last known controller read index */
  int pdc_entry;                /* SYS2CMAC wake-up entry */
  bool running;

  /* Deferred part of a write that did not fit */

  const uint8_t *pend_buf;
  size_t pend_len;

  da1470x_cmac_rx_t rx_cb;
  void *rx_arg;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct da1470x_cmac_s g_cmac =
{
  .tx        = (struct cmac_mbox_s *)CMAC_MBOX_SYS2CMAC,
  .rx        = (struct cmac_mbox_s *)CMAC_MBOX_CMAC2SYS,
  .pdc_entry = DA1470X_PDC_INVALID_ENTRY,
};

/* Firmware image provided by the board (see da1470x_cmac_fw.S) */

extern const uint8_t g_da1470x_cmac_fw[];
extern const uint8_t g_da1470x_cmac_fw_end[];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cmac_signal
 *
 * Description:
 *   Raise the SYS2CMAC interrupt.
 *
 ****************************************************************************/

static inline void cmac_signal(void)
{
  modifyreg32(DA1470X_CRG_XTAL_SET_SYS_IRQ_CTRL, 0,
              CRG_XTAL_SYS_IRQ_CTRL_SYS2CMAC_IRQ_BIT);
}

/****************************************************************************
 * Name: cmac_wait_bits
 ****************************************************************************/

static int cmac_wait_bits(uintptr_t addr, uint32_t mask, bool set)
{
  int i;

  for (i = 0; i < CMAC_WAIT_LOOPS; i++)
    {
      uint32_t v = getreg32(addr) & mask;

      if ((set && v != 0) || (!set && v == 0))
        {
          return OK;
        }
    }

  return -ETIMEDOUT;
}

/****************************************************************************
 * Name: cmac_m33_to_cmac_addr
 *
 * Description:
 *   Translate a SYSRAM address into the controller's view, as the SDK's
 *   m33_to_cmac_addr() does, using the data window base programmed by
 *   the controller itself.  Addresses below RAM8 are not visible to the
 *   controller and are returned unchanged.
 *
 ****************************************************************************/

static uint32_t cmac_m33_to_cmac_addr(uint32_t addr)
{
  uint32_t base = getreg32(DA1470X_MEMCTRL_CMI_DATA_BASE);

  if (addr > 0x2014ffff)
    {
      return addr + (0xff000000 - base + 0xeb0000);
    }
  else if (addr > 0x2012ffff)
    {
      return addr + (0xff000000 - base + 0xf00000);
    }
  else if (addr > 0x2010ffff)
    {
      return addr + 0xffff0000;
    }

  return addr;
}

/****************************************************************************
 * Name: cmac_lpclk_code
 ****************************************************************************/

static uint8_t cmac_lpclk_code(void)
{
  switch (da1470x_get_lpclk())
    {
      case 32768:
        return 0;

      case 32000:
      default:

        /* RCLP is not a supported controller clock; report 32 kHz and
         * keep the controller awake (sleep_enable stays false).
         */

        return 1;
    }
}

/****************************************************************************
 * Name: cmac_fill_tables
 *
 * Description:
 *   What the SDK's cmac_config_table_setup() does, reduced to the fields
 *   a NuttX host cares about.
 *
 ****************************************************************************/

static void cmac_fill_tables(const uint8_t bdaddr[6])
{
  uint8_t  *cfg = (uint8_t *)CMAC_CONFIG_TABLE;
  uint8_t  *dyn = (uint8_t *)CMAC_DYNAMIC_TABLE;
  uint32_t *tcs = (uint32_t *)CMAC_TCS_TABLE;

  memcpy(cfg + CFG_BD_ADDRESS, bdaddr, 6);
  cfg[CFG_LP_CLOCK_FREQ]        = cmac_lpclk_code();
  cfg[CFG_LENGTH_EXCHANGE]      = 1;
  cfg[CFG_SYSTEM_TCS_LENGTH]    = 0;
  cfg[CFG_SYNTH_TCS_LENGTH]     = 0;
  cfg[CFG_RFCU_TCS_LENGTH]      = 0;
  *(uint16_t *)(cfg + CFG_LP_CLOCK_DRIFT)   = 500;
  *(uint16_t *)(cfg + CFG_RX_BUFFER_SIZE)   = 262;
  *(uint16_t *)(cfg + CFG_TX_BUFFER_SIZE)   = 262;

  /* Empty TCS tables: the controller runs with default trims.  Loading
   * the OTP configuration script is a follow-up.
   */

  memset((void *)CMAC_HOST_TCS_ATTR, 0, TCS_GROUP_MAX * sizeof(uint32_t));
  memset((void *)CMAC_HOST_TCS_DATA, 0, 4 * sizeof(uint32_t));
  putreg32(0, CMAC_HOST_TEMPSENS);

  tcs[TCS_ATTR_PTR / 4]  = CMAC_HOST_TCS_ATTR;
  tcs[TCS_ATTR_SIZE / 4] = TCS_GROUP_MAX;
  tcs[TCS_DATA_PTR / 4]  = CMAC_HOST_TCS_DATA;
  tcs[TCS_DATA_SIZE / 4] = 0;

  dyn[DYN_SLEEP_ENABLE] = 0;
  *(uint32_t *)(dyn + DYN_POWER_CTRL_WAKEUP) =
    getreg32(DA1470X_CRG_TOP_POWER_CTRL);
  *(uint32_t *)(dyn + DYN_POWER_CTRL_SLEEP) =
    getreg32(DA1470X_CRG_TOP_POWER_CTRL);
  *(uint32_t *)(dyn + DYN_POWER_LVL_WAKEUP) =
    getreg32(DA1470X_CRG_TOP_POWER_LVL);
  *(uint32_t *)(dyn + DYN_POWER_LVL_SLEEP) =
    getreg32(DA1470X_CRG_TOP_POWER_LVL);
  *(uint32_t *)(dyn + DYN_TEMPSENS_PTR) = CMAC_HOST_TEMPSENS;
}

/****************************************************************************
 * Name: cmac_mbox_free
 *
 * Description:
 *   Free bytes in the host->controller ring and the contiguous run
 *   available at the write index.
 *
 ****************************************************************************/

static size_t cmac_mbox_free(struct cmac_mbox_s *m, size_t *contiguous)
{
  uint16_t wr = m->wr;
  uint16_t rd = m->rd;

  if (rd > wr)
    {
      *contiguous = rd - wr - 1;
      return *contiguous;
    }
  else if (rd == 0)
    {
      *contiguous = CMAC_MBOX_BUFSIZE - wr - 1;
      return *contiguous;
    }
  else
    {
      *contiguous = CMAC_MBOX_BUFSIZE - wr;
      return CMAC_MBOX_BUFSIZE - wr + rd - 1;
    }
}

/****************************************************************************
 * Name: cmac_mbox_push
 *
 * Description:
 *   Copy up to len bytes into the host->controller ring.  Returns the
 *   number copied.  Must be called with interrupts disabled.
 *
 ****************************************************************************/

static size_t cmac_mbox_push(struct da1470x_cmac_s *priv,
                             const uint8_t *data, size_t len)
{
  struct cmac_mbox_s *m = priv->tx;
  size_t contiguous;
  size_t avail;
  size_t n;
  uint16_t wr;

  if ((m->flags & CMAC_MBOX_FLAG_RESET) != 0)
    {
      /* The controller has not consumed our reset yet */

      return 0;
    }

  avail = cmac_mbox_free(m, &contiguous);
  if (len > avail)
    {
      len = avail;
    }

  wr = m->wr;
  n  = len < contiguous ? len : contiguous;
  memcpy((void *)&m->buf[wr], data, n);
  wr += n;
  if (wr >= CMAC_MBOX_BUFSIZE)
    {
      wr = 0;
    }

  if (len > n)
    {
      memcpy((void *)&m->buf[0], data + n, len - n);
      wr = len - n;
    }

  m->wr = wr;
  return len;
}

/****************************************************************************
 * Name: cmac_tx_continue
 *
 * Description:
 *   Push pending write bytes; maintain the WRITE_PEND flag.
 *
 ****************************************************************************/

static void cmac_tx_continue(struct da1470x_cmac_s *priv)
{
  if (priv->pend_len > 0)
    {
      size_t n = cmac_mbox_push(priv, priv->pend_buf, priv->pend_len);

      priv->pend_buf += n;
      priv->pend_len -= n;

      if (priv->pend_len > 0)
        {
          priv->tx->flags |= CMAC_MBOX_FLAG_WRITE_PEND;
        }
      else
        {
          priv->tx->flags &= ~CMAC_MBOX_FLAG_WRITE_PEND;
        }

      cmac_signal();
    }
}

/****************************************************************************
 * Name: cmac_rx_drain
 *
 * Description:
 *   Deliver whatever the controller has written to the host ring.
 *
 ****************************************************************************/

static void cmac_rx_drain(struct da1470x_cmac_s *priv)
{
  struct cmac_mbox_s *m = priv->rx;
  uint8_t chunk[64];
  uint16_t rd;
  uint16_t wr;
  size_t n;

  if ((m->flags & CMAC_MBOX_FLAG_RESET) != 0)
    {
      /* Controller restarted its ring: discard and resynchronise */

      m->rd = m->wr;
      priv->last_rd = m->wr;
      m->flags &= ~CMAC_MBOX_FLAG_RESET;
    }

  if (m->magic != CMAC_MBOX_MAGIC || priv->last_rd != m->rd)
    {
      m->rd = m->wr;
      priv->last_rd = m->wr;
    }

  rd = m->rd;
  wr = m->wr;

  while (rd != wr)
    {
      n = 0;
      while (rd != wr && n < sizeof(chunk))
        {
          chunk[n++] = m->buf[rd];
          if (++rd >= CMAC_MBOX_BUFSIZE)
            {
              rd = 0;
            }
        }

      m->rd = rd;
      priv->last_rd = rd;

      if (priv->rx_cb != NULL)
        {
          priv->rx_cb(priv->rx_arg, chunk, n);
        }
    }

  /* The controller has data waiting for room on its side */

  if ((m->flags & CMAC_MBOX_FLAG_WRITE_PEND) != 0)
    {
      cmac_signal();
    }
}

/****************************************************************************
 * Name: cmac_interrupt
 ****************************************************************************/

static int cmac_interrupt(int irq, void *context, void *arg)
{
  struct da1470x_cmac_s *priv = (struct da1470x_cmac_s *)arg;
  uint32_t status = getreg32(CMAC_STATUS_REG);

  if ((status & CMAC_STATUS_ERROR_MASK) != 0)
    {
      wlerr("CMAC fault, status %08" PRIx32 "\n", status);
    }

  /* Acknowledge */

  modifyreg32(DA1470X_CRG_XTAL_RESET_SYS_IRQ_CTRL, 0,
              CRG_XTAL_SYS_IRQ_CTRL_CMAC2SYS_IRQ_BIT);

  cmac_rx_drain(priv);
  cmac_tx_continue(priv);
  return OK;
}

/****************************************************************************
 * Name: cmac_mbox_init
 *
 * Description:
 *   Reset the host ring, wait for the controller to publish its magic and
 *   enable the interrupt.
 *
 ****************************************************************************/

static int cmac_mbox_init(struct da1470x_cmac_s *priv)
{
  struct cmac_mbox_s *tx = priv->tx;
  int i;

  memset((void *)tx->buf, 0, CMAC_MBOX_BUFSIZE);
  tx->flags = CMAC_MBOX_FLAG_RESET;
  tx->wr    = 0;
  tx->rd    = 0;
  priv->last_rd = priv->rx->rd;
  tx->magic = CMAC_MBOX_MAGIC;

  for (i = 0; i < CMAC_WAIT_LOOPS; i++)
    {
      if (priv->rx->magic == CMAC_MBOX_MAGIC)
        {
          break;
        }
    }

  if (i == CMAC_WAIT_LOOPS)
    {
      wlerr("Controller did not open its mailbox\n");
      return -ETIMEDOUT;
    }

  cmac_rx_drain(priv);

  irq_attach(DA1470X_IRQ_CMAC2SYS, cmac_interrupt, priv);
  up_enable_irq(DA1470X_IRQ_CMAC2SYS);
  cmac_signal();
  return OK;
}

/****************************************************************************
 * Name: cmac_boot
 *
 * Description:
 *   The controller boot handshake as performed by the SDK's
 *   cmac_cpu_start().
 *
 ****************************************************************************/

static int cmac_boot(struct da1470x_cmac_s *priv, const uint8_t bdaddr[6])
{
  const uint8_t *fw = g_da1470x_cmac_fw;
  size_t fwlen = g_da1470x_cmac_fw_end - g_da1470x_cmac_fw;
  uint32_t imgsize;
  uint32_t *tcs;
  uint32_t status;
  uint8_t *cfg;
  int ret;
  int i;

  if (fwlen < CMAC_FW_HDR_SIZE ||
      *(const uint32_t *)(fw + CMAC_FW_HDR_MAGIC_OFF) != CMAC_FW_HDR_MAGIC)
    {
      wlerr("Bad CMAC firmware header\n");
      return -EINVAL;
    }

  imgsize = *(const uint32_t *)(fw + CMAC_FW_HDR_IMGSIZE_OFF);
  if (imgsize + CMAC_FW_HDR_SIZE > fwlen || imgsize > CMAC_AREA_SIZE)
    {
      wlerr("CMAC firmware size mismatch\n");
      return -EINVAL;
    }

  /* Wake-up entry that powers PD_RAD for the controller.  The firmware
   * scans the LUT for exactly this trigger (MAC timer, master CMAC) at
   * boot and reuses it for the SYS2CMAC mailbox interrupt.
   */

  priv->pdc_entry = da1470x_pdc_add(DA1470X_PDC_TRIG_PERIPHERAL,
                                    DA1470X_PDC_PERIPH_MAC_TIMER,
                                    DA1470X_PDC_MASTER_CMAC,
                                    DA1470X_PDC_FLAG_EN_XTAL);
  if (priv->pdc_entry < 0)
    {
      return priv->pdc_entry;
    }

  /* Hold the controller in reset and drop anything left over from a
   * previous run (a warm reset of the host does not reset the CMAC).
   */

  modifyreg32(DA1470X_CRG_TOP_CLK_RADIO, 0,
              CRG_TOP_CLK_RADIO_CMAC_SYNCH_RESET);
  modifyreg32(DA1470X_CRG_XTAL_RESET_SYS_IRQ_CTRL, 0,
              CRG_XTAL_SYS_IRQ_CTRL_CMAC2SYS_IRQ_BIT);

  /* Power the radio domain through the PDC if it is down */

  if ((getreg32(DA1470X_CRG_TOP_SYS_STAT) &
       CRG_TOP_SYS_STAT_RAD_IS_UP) == 0)
    {
      da1470x_pdc_set_pending(priv->pdc_entry);
      da1470x_pdc_acknowledge(priv->pdc_entry);
      ret = cmac_wait_bits(DA1470X_CRG_TOP_SYS_STAT,
                           CRG_TOP_SYS_STAT_RAD_IS_UP, true);
      if (ret < 0)
        {
          wlerr("PD_RAD did not come up\n");
          return ret;
        }
    }

  /* Release every hardware semaphore */

  for (i = 0; i < 32; i += 2)
    {
      putreg32(3u << i, DA1470X_MEMCTRL_BUSY_RESET);
    }

  /* CMAC and RF clocks, XTAL32M must be running and feed the CMAC */

  modifyreg32(DA1470X_CRG_TOP_CLK_RADIO, 0,
              CRG_TOP_CLK_RADIO_CMAC_CLK_ENABLE |
              CRG_TOP_CLK_RADIO_RFCU_ENABLE);

  ret = da1470x_xtal32m_enable();
  if (ret < 0)
    {
      return ret;
    }

  modifyreg32(DA1470X_CRG_TOP_CLK_CMAC_SWITCH, 0,
              CRG_TOP_CLK_CMAC_SWITCH_CMAC_CLK_SEL);
  ret = cmac_wait_bits(DA1470X_CRG_TOP_CLK_CMAC_SWITCH,
                       CRG_TOP_CLK_CMAC_SWITCH_CMAC_RUNNING_ON_XTAL, true);
  if (ret < 0)
    {
      wlerr("CMAC clock did not switch to XTAL32M\n");
      return ret;
    }

  /* Memory windows, then the image */

  putreg32(CMAC_CODE_BASE, DA1470X_MEMCTRL_CMI_CODE_BASE);
  putreg32(0, DA1470X_MEMCTRL_CMI_DATA_BASE);
  putreg32(0, DA1470X_MEMCTRL_CMI_SHARED_BASE);

  memcpy((void *)CMAC_CODE_BASE, fw + CMAC_FW_HDR_SIZE, imgsize);

  cfg = (uint8_t *)CMAC_CONFIG_TABLE;
  cfg[CFG_WAIT_ON_MAIN] = 1;

  /* Clear any stale interrupt, release the reset and wait for the
   * controller to raise CMAC2SYS from its boot code.
   */

  putreg32(1, NVIC_IRQ0_31_CLEAR);
  putreg32(1, NVIC_IRQ0_31_CLRPEND);

  modifyreg32(DA1470X_CRG_TOP_CLK_RADIO,
              CRG_TOP_CLK_RADIO_CMAC_SYNCH_RESET, 0);

  ret = cmac_wait_bits(DA1470X_CRG_XTAL_SYS_IRQ_CTRL,
                       CRG_XTAL_SYS_IRQ_CTRL_CMAC2SYS_IRQ_BIT, true);
  if (ret < 0)
    {
      wlerr("CMAC did not boot\n");
      return ret;
    }

  status = getreg32(CMAC_STATUS_REG);
  if ((status & CMAC_STATUS_ERROR_MASK) != 0)
    {
      wlerr("CMAC boot fault, status %08" PRIx32 "\n", status);
      return -EIO;
    }

  modifyreg32(DA1470X_CRG_XTAL_RESET_SYS_IRQ_CTRL, 0,
              CRG_XTAL_SYS_IRQ_CTRL_CMAC2SYS_IRQ_BIT);

  /* The controller waits on main(): fill the tables, convert the
   * pointers it dereferences to its own address view, then release it.
   */

  cmac_fill_tables(bdaddr);

  tcs = (uint32_t *)CMAC_TCS_TABLE;
  tcs[TCS_ATTR_PTR / 4] = cmac_m33_to_cmac_addr(tcs[TCS_ATTR_PTR / 4]);
  tcs[TCS_DATA_PTR / 4] = cmac_m33_to_cmac_addr(tcs[TCS_DATA_PTR / 4]);
  *(uint32_t *)(CMAC_DYNAMIC_TABLE + DYN_TEMPSENS_PTR) =
    cmac_m33_to_cmac_addr(*(uint32_t *)(CMAC_DYNAMIC_TABLE +
                                        DYN_TEMPSENS_PTR));

  cfg[CFG_WAIT_ON_MAIN] = 0;
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_cmac_start
 ****************************************************************************/

int da1470x_cmac_start(const uint8_t bdaddr[6], da1470x_cmac_rx_t rx,
                       void *arg)
{
  struct da1470x_cmac_s *priv = &g_cmac;
  int ret;

  if (priv->running)
    {
      return OK;
    }

  priv->rx_cb    = rx;
  priv->rx_arg   = arg;
  priv->pend_len = 0;

  ret = cmac_boot(priv, bdaddr);
  if (ret >= 0)
    {
      ret = cmac_mbox_init(priv);
    }

  if (ret < 0)
    {
      /* Leave the controller in reset so it cannot interrupt us */

      modifyreg32(DA1470X_CRG_TOP_CLK_RADIO, 0,
                  CRG_TOP_CLK_RADIO_CMAC_SYNCH_RESET);
      modifyreg32(DA1470X_CRG_XTAL_RESET_SYS_IRQ_CTRL, 0,
                  CRG_XTAL_SYS_IRQ_CTRL_CMAC2SYS_IRQ_BIT);
      return ret;
    }

  priv->running = true;
  return OK;
}

/****************************************************************************
 * Name: da1470x_cmac_stop
 ****************************************************************************/

void da1470x_cmac_stop(void)
{
  struct da1470x_cmac_s *priv = &g_cmac;

  if (!priv->running)
    {
      return;
    }

  up_disable_irq(DA1470X_IRQ_CMAC2SYS);
  irq_detach(DA1470X_IRQ_CMAC2SYS);

  modifyreg32(DA1470X_CRG_TOP_CLK_RADIO, 0,
              CRG_TOP_CLK_RADIO_CMAC_SYNCH_RESET);
  modifyreg32(DA1470X_CRG_TOP_CLK_RADIO,
              CRG_TOP_CLK_RADIO_CMAC_CLK_ENABLE |
              CRG_TOP_CLK_RADIO_RFCU_ENABLE, 0);

  da1470x_pd_disable(DA1470X_PD_RAD);
  priv->running = false;
}

/****************************************************************************
 * Name: da1470x_cmac_write
 ****************************************************************************/

int da1470x_cmac_write(const uint8_t *data, size_t len)
{
  struct da1470x_cmac_s *priv = &g_cmac;
  irqstate_t flags;
  size_t n;

  if (!priv->running)
    {
      return -ENODEV;
    }

  flags = enter_critical_section();

  if (priv->pend_len > 0)
    {
      leave_critical_section(flags);
      return -EAGAIN;
    }

  n = cmac_mbox_push(priv, data, len);
  if (n < len)
    {
      priv->pend_buf = data + n;
      priv->pend_len = len - n;
      priv->tx->flags |= CMAC_MBOX_FLAG_WRITE_PEND;
    }
  else
    {
      priv->tx->flags &= ~CMAC_MBOX_FLAG_WRITE_PEND;
    }

  cmac_signal();
  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: da1470x_cmac_write_pending
 ****************************************************************************/

bool da1470x_cmac_write_pending(void)
{
  return g_cmac.pend_len > 0;
}

#endif /* CONFIG_DA1470X_BLE */
