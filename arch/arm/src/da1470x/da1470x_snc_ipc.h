/****************************************************************************
 * arch/arm/src/da1470x/da1470x_snc_ipc.h
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

/* The contract between the M33 and the sensor node controller.  Both sides
 * include this file: the NuttX driver and the controller's own firmware,
 * which is built freestanding for the Cortex-M0+, so nothing here may
 * depend on NuttX.
 *
 * The two processors share RAM8.  At its start sits struct snc_shared_s:
 * the controller's state, fault information, and two message rings, one in
 * each direction.  Each ring has a single producer and a single consumer
 * and each of its indices is written by one side only, so no lock is
 * needed; a barrier orders the data before the index that publishes it,
 * and a doorbell interrupt tells the other side to look.
 */

#ifndef __ARCH_ARM_SRC_DA1470X_DA1470X_SNC_IPC_H
#define __ARCH_ARM_SRC_DA1470X_DA1470X_SNC_IPC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifndef __ASSEMBLY__
#  include <stdint.h>
#endif

#include <arch/chip/snc.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SNC_IPC_MAGIC           0x31434e53u   /* 'S','N','C','1' */
#define SNC_IPC_VERSION         1

/* RAM8 in the two address views */

#define SNC_SHARED_M33_BASE     0x20110000u
#define SNC_SHARED_SNC_BASE     0x00030000u
#define SNC_SHARED_SIZE         0x00020000u

/* The image header sits right after the controller's vector table: 16
 * core entries and 32 interrupts.
 */

#define SNC_IMAGE_HDR_OFFSET    0xc0

#define SNC_RING_SIZE           2048          /* Power of two */

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Found at SNC_IMAGE_HDR_OFFSET in every firmware image */

struct snc_image_hdr_s
{
  uint32_t magic;               /* SNC_IPC_MAGIC */
  uint32_t ipc_version;         /* SNC_IPC_VERSION it was built against */
  uint32_t size;                /* Bytes to load, from address zero */
  uint32_t reserved;
};

struct snc_msg_hdr_s
{
  uint16_t type;
  uint16_t len;                 /* Payload bytes that follow */
};

struct snc_ring_s
{
  volatile uint32_t head;       /* Written by the producer only */
  volatile uint32_t tail;       /* Written by the consumer only */
  uint8_t data[SNC_RING_SIZE];
};

struct snc_shared_s
{
  uint32_t magic;               /* Set by the M33 before starting */
  uint32_t version;
  volatile uint32_t state;      /* SNC_STATE_*, set by the controller */
  volatile uint32_t heartbeat;  /* Advanced by the controller */
  volatile uint32_t fault_pc;   /* Filled in by its HardFault handler */
  volatile uint32_t fault_lr;
  volatile uint32_t dropped;    /* Messages it could not queue */
  uint32_t lpclk_hz;            /* Low power clock, set by the M33 */
  struct snc_ring_s to_snc;     /* Commands */
  struct snc_ring_s to_m33;     /* Events */
};

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/* Both processors are little endian and the ring is walked a byte at a
 * time, so a message may wrap and needs no alignment.
 */

static inline void snc_ipc_barrier(void)
{
  __asm__ __volatile__ ("dmb" : : : "memory");
}

static inline uint32_t snc_ring_used(const struct snc_ring_s *r)
{
  return r->head - r->tail;
}

static inline uint32_t snc_ring_space(const struct snc_ring_s *r)
{
  return SNC_RING_SIZE - snc_ring_used(r);
}

static inline void snc_ring_copyin(struct snc_ring_s *r, uint32_t pos,
                                   const void *src, uint32_t len)
{
  const uint8_t *s = (const uint8_t *)src;
  uint32_t i;

  for (i = 0; i < len; i++)
    {
      r->data[(pos + i) & (SNC_RING_SIZE - 1)] = s[i];
    }
}

static inline void snc_ring_copyout(const struct snc_ring_s *r,
                                    uint32_t pos, void *dst, uint32_t len)
{
  uint8_t *d = (uint8_t *)dst;
  uint32_t i;

  for (i = 0; i < len; i++)
    {
      d[i] = r->data[(pos + i) & (SNC_RING_SIZE - 1)];
    }
}

/* Queue one message.  Returns 0, or -1 when it does not fit now. */

static inline int snc_ring_put(struct snc_ring_s *r, uint16_t type,
                               const void *payload, uint16_t len)
{
  struct snc_msg_hdr_s hdr;
  uint32_t head = r->head;

  if (len > SNC_MSG_MAX_PAYLOAD || snc_ring_space(r) < sizeof(hdr) + len)
    {
      return -1;
    }

  hdr.type = type;
  hdr.len  = len;
  snc_ring_copyin(r, head, &hdr, sizeof(hdr));
  snc_ring_copyin(r, head + sizeof(hdr), payload, len);

  snc_ipc_barrier();
  r->head = head + sizeof(hdr) + len;
  return 0;
}

/* Look at the next message without taking it.  Returns its payload length,
 * with the type in *type, or -1 when the ring is empty.
 */

static inline int snc_ring_peek(const struct snc_ring_s *r, uint16_t *type)
{
  struct snc_msg_hdr_s hdr;

  if (snc_ring_used(r) < sizeof(hdr))
    {
      return -1;
    }

  snc_ipc_barrier();
  snc_ring_copyout(r, r->tail, &hdr, sizeof(hdr));
  *type = hdr.type;
  return hdr.len;
}

/* Take the next message.  Returns its payload length, with the type in
 * *type, or -1 when the ring is empty.  A payload longer than the buffer
 * is truncated to it; the message is consumed either way.
 */

static inline int snc_ring_get(struct snc_ring_s *r, uint16_t *type,
                               void *payload, uint16_t size)
{
  struct snc_msg_hdr_s hdr;
  uint32_t tail = r->tail;
  uint16_t n;

  if (snc_ring_used(r) < sizeof(hdr))
    {
      return -1;
    }

  snc_ipc_barrier();
  snc_ring_copyout(r, tail, &hdr, sizeof(hdr));
  n = hdr.len < size ? hdr.len : size;
  snc_ring_copyout(r, tail + sizeof(hdr), payload, n);

  snc_ipc_barrier();
  r->tail = tail + sizeof(hdr) + hdr.len;

  *type = hdr.type;
  return n;
}

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_DA1470X_DA1470X_SNC_IPC_H */
