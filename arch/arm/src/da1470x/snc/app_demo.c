/****************************************************************************
 * arch/arm/src/da1470x/snc/app_demo.c
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

/* Demo firmware for the sensor node controller.  It exercises every part
 * of the framework and nothing else:
 *
 *   PING     -> PONG with the same payload (the mailbox, both ways)
 *   PERIOD   -> a TICK every so many milliseconds (its own timer, and
 *               waking the M33 on its own initiative)
 *   WATCH    -> an EDGE whenever a GPIO changes, sampled every 10 ms
 *               (peripheral access from the controller)
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "hardware/da1470x_gpio.h"

#include "snc_rt.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define DEMO_BASE_MS    10      /* Timer period while anything is active */

/****************************************************************************
 * Private Data
 ****************************************************************************/

static uint32_t g_period_ms;    /* TICK period, 0 off */
static uint32_t g_elapsed_ms;
static uint32_t g_ticks_sent;

static uint8_t  g_watch_port = 0xff;
static uint8_t  g_watch_pin = 0xff;
static uint8_t  g_watch_level;
static uint32_t g_edges;

static uint8_t  g_buf[SNC_MSG_MAX_PAYLOAD];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static bool watching(void)
{
  return g_watch_pin != 0xff;
}

static uint8_t read_pin(void)
{
  uint32_t data = getreg32(DA1470X_GPIO_P0_DATA + 4 * g_watch_port);

  return (data >> g_watch_pin) & 1;
}

/* Run the timer only while something needs it */

static void update_timer(void)
{
  snc_timer_start(g_period_ms != 0 || watching() ? DEMO_BASE_MS : 0);
}

static void handle_command(uint16_t type, int len)
{
  switch (type)
    {
      case SNC_MSG_PING:
        snc_send(SNC_MSG_PONG, g_buf, len);
        break;

      case SNC_DEMO_PERIOD:
        if (len >= 4)
          {
            g_period_ms = g_buf[0] | g_buf[1] << 8 | g_buf[2] << 16 |
                          (uint32_t)g_buf[3] << 24;
            g_elapsed_ms = 0;
            update_timer();
          }
        break;

      case SNC_DEMO_WATCH:
        if (len >= 2 && g_buf[0] <= 2 && (g_buf[1] < 32 || g_buf[1] == 0xff))
          {
            g_watch_port = g_buf[0];
            g_watch_pin = g_buf[1];
            if (watching())
              {
                g_watch_level = read_pin();
              }

            update_timer();
          }
        break;

      default:
        break;
    }
}

static void handle_tick(void)
{
  if (watching())
    {
      uint8_t level = read_pin();

      if (level != g_watch_level)
        {
          struct snc_demo_edge_s edge;

          g_watch_level = level;
          edge.port = g_watch_port;
          edge.pin = g_watch_pin;
          edge.level = level;
          edge.reserved = 0;
          edge.count = ++g_edges;
          snc_send(SNC_DEMO_EDGE, &edge, sizeof(edge));
        }
    }

  if (g_period_ms != 0)
    {
      g_elapsed_ms += DEMO_BASE_MS;
      if (g_elapsed_ms >= g_period_ms)
        {
          struct snc_demo_tick_s tick;

          g_elapsed_ms = 0;
          tick.count = ++g_ticks_sent;
          tick.heartbeat = g_snc_shared->heartbeat;
          snc_send(SNC_DEMO_TICK, &tick, sizeof(tick));
        }
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(void)
{
  static const char hello[] = "demo firmware up";
  uint32_t ticks_done = 0;

  snc_rt_init();
  snc_rt_ready();
  snc_send(SNC_MSG_LOG, hello, sizeof(hello) - 1);

  for (; ; )
    {
      uint16_t type;
      int len;

      snc_rt_wait();

      /* Take a command only when any reply to it is sure to fit; the rest
       * wait in the ring, and the M33 rings again when it has made room.
       */

      g_snc_cmd_pending = false;
      while (snc_ring_space(&g_snc_shared->to_m33) >=
             sizeof(struct snc_msg_hdr_s) + SNC_MSG_MAX_PAYLOAD &&
             (len = snc_recv(&type, g_buf, sizeof(g_buf))) >= 0)
        {
          handle_command(type, len);
        }

      while (ticks_done != g_snc_timer_ticks)
        {
          ticks_done++;
          handle_tick();
        }
    }

  return 0;
}
