/****************************************************************************
 * arch/arm/include/da1470x/snc.h
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

/* Talking to the sensor node controller (SNC), the DA1470x's Cortex-M0+.
 *
 * Messages go both ways as {type, len, payload}.  From user space they are
 * written to and read from /dev/snc0, one message per write() or read(),
 * as struct snc_msg_s trimmed to its payload.  Inside the kernel the same
 * messages go through da1470x_snc_send() and a handler per type.
 *
 * This header is also included by the controller's firmware, which is
 * built without NuttX; everything NuttX-specific is guarded.
 */

#ifndef __ARCH_ARM_INCLUDE_DA1470X_SNC_H
#define __ARCH_ARM_INCLUDE_DA1470X_SNC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifndef __ASSEMBLY__
#  include <stdint.h>
#endif

#ifdef __NuttX__
#  include <nuttx/fs/ioctl.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SNC_MSG_MAX_PAYLOAD     256

/* Message types.  0x0000-0x00ff belong to the framework, 0x0100 and up to
 * the firmware application that is loaded.
 */

#define SNC_MSG_PING            0x0001  /* M33 -> SNC: any payload */
#define SNC_MSG_PONG            0x0002  /* SNC -> M33: the same payload */
#define SNC_MSG_LOG             0x0003  /* SNC -> M33: text */

#define SNC_MSG_APP             0x0100

/* The demo firmware (CONFIG_DA1470X_SNC_FW_DEMO) */

#define SNC_DEMO_PERIOD         0x0100  /* M33 -> SNC: uint32_t ms, 0 off */
#define SNC_DEMO_TICK           0x0101  /* SNC -> M33: snc_demo_tick_s */
#define SNC_DEMO_WATCH          0x0102  /* M33 -> SNC: snc_demo_watch_s */
#define SNC_DEMO_EDGE           0x0103  /* SNC -> M33: snc_demo_edge_s */

/* Controller states */

#define SNC_STATE_OFF           0
#define SNC_STATE_BOOTING       1
#define SNC_STATE_READY         2
#define SNC_STATE_FAULT         3

#ifdef __NuttX__

/* ioctls on /dev/snc0.  The base is private to this chip. */

#define _SNCIOCBASE             (0xfe00)
#define _SNCIOC(nr)             _IOC(_SNCIOCBASE, nr)

#define SNCIOC_STATUS           _SNCIOC(1)  /* Arg: struct snc_status_s * */
#define SNCIOC_STOP             _SNCIOC(2)  /* Arg: none */
#define SNCIOC_START            _SNCIOC(3)  /* Arg: none; reload and run */

#endif

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct snc_msg_s
{
  uint16_t type;
  uint16_t len;
  uint8_t  payload[SNC_MSG_MAX_PAYLOAD];
};

struct snc_status_s
{
  uint32_t state;               /* SNC_STATE_* */
  uint32_t heartbeat;           /* Advances while the firmware runs */
  uint32_t fault_pc;            /* Where a fault happened */
  uint32_t fault_lr;
  uint32_t dropped_to_m33;      /* Events the controller could not queue */
  uint32_t dropped_to_snc;      /* Commands the M33 could not queue */
  uint32_t hwstatus;            /* SNC_STATUS_REG: idle, locked, watchdog */
};

struct snc_demo_tick_s
{
  uint32_t count;
  uint32_t heartbeat;
};

struct snc_demo_watch_s
{
  uint8_t  port;                /* 0..2 */
  uint8_t  pin;                 /* 0..31, or 0xff to stop watching */
  uint16_t reserved;
};

struct snc_demo_edge_s
{
  uint8_t  port;
  uint8_t  pin;
  uint8_t  level;
  uint8_t  reserved;
  uint32_t count;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __NuttX__

#ifdef __cplusplus
extern "C"
{
#endif

/* Queue a message for the controller and ring its doorbell.  Returns OK,
 * -EAGAIN when its command ring is full, or -ENODEV when it is not
 * running.
 */

int da1470x_snc_send(uint16_t type, const void *payload, uint16_t len);

/* Have messages of one type delivered to a function, from the low priority
 * work queue, instead of to readers of /dev/snc0.  NULL removes it.
 */

typedef void (*snc_handler_t)(uint16_t type, const void *payload,
                              uint16_t len, void *arg);

int da1470x_snc_register_handler(uint16_t type, snc_handler_t handler,
                                 void *arg);

#ifdef __cplusplus
}
#endif

#endif /* __NuttX__ */

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_INCLUDE_DA1470X_SNC_H */
