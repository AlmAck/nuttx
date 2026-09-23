/****************************************************************************
 * arch/arm/src/da1470x/snc/app_input.c
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

/* Input firmware for the sensor node controller: it watches the pins the
 * M33 asks it to -- buttons, a touch controller's interrupt line -- and
 * sends an event only when a change has settled.  The M33 then neither
 * polls them nor takes an interrupt for every contact bounce; it sleeps
 * until something has really happened.
 *
 * The pins are sampled every SAMPLE_MS from the controller's own timer,
 * which runs only while something is being watched.
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

#define SAMPLE_MS           5
#define DEFAULT_DEBOUNCE_MS 20

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct input_port_s
{
  uint32_t pins;        /* Watched */
  uint32_t stable;      /* Levels last reported */
  uint32_t candidate;   /* Levels seen, waiting to settle */
  uint16_t settle;      /* Samples the candidate has held */
  uint16_t needed;      /* Samples it has to hold */
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct input_port_s g_ports[SNC_INPUT_NPORTS];
static uint32_t g_events;
static uint8_t g_buf[SNC_MSG_MAX_PAYLOAD];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static uint32_t read_port(int port)
{
  return getreg32(DA1470X_GPIO_P0_DATA + 4 * port);
}

static bool watching(void)
{
  int i;

  for (i = 0; i < SNC_INPUT_NPORTS; i++)
    {
      if (g_ports[i].pins != 0)
        {
          return true;
        }
    }

  return false;
}

static void report(int port, uint32_t changed)
{
  struct snc_input_event_s ev;

  ev.port = port;
  ev.reserved[0] = ev.reserved[1] = ev.reserved[2] = 0;
  ev.levels = g_ports[port].stable;
  ev.changed = changed;
  ev.count = ++g_events;
  snc_send(SNC_INPUT_EVENT, &ev, sizeof(ev));
}

static void report_change(int port, uint32_t now)
{
  uint32_t changed = now ^ g_ports[port].stable;

  g_ports[port].stable = now;
  g_ports[port].settle = 0;
  report(port, changed);
}

static void configure(const struct snc_input_config_s *cfg)
{
  struct input_port_s *p;
  uint16_t ms;

  if (cfg->port >= SNC_INPUT_NPORTS)
    {
      return;
    }

  p = &g_ports[cfg->port];
  ms = cfg->debounce_ms != 0 ? cfg->debounce_ms : DEFAULT_DEBOUNCE_MS;

  p->pins = cfg->pins;
  p->stable = read_port(cfg->port) & p->pins;
  p->candidate = p->stable;
  p->settle = 0;
  p->needed = (ms + SAMPLE_MS - 1) / SAMPLE_MS;

  /* Tell the M33 where things stand, so it starts from the truth */

  if (p->pins != 0)
    {
      report(cfg->port, 0);
    }

  snc_timer_start(watching() ? SAMPLE_MS : 0);
}

/* One sample, standing for the elapsed ticks since the last one */

static void sample(uint32_t elapsed)
{
  int i;

  for (i = 0; i < SNC_INPUT_NPORTS; i++)
    {
      struct input_port_s *p = &g_ports[i];
      uint32_t now;

      if (p->pins == 0)
        {
          continue;
        }

      now = read_port(i) & p->pins;
      if (now != p->candidate)
        {
          /* Still moving: start settling again */

          p->candidate = now;
          p->settle = 0;
        }
      else if (now != p->stable)
        {
          p->settle += elapsed;
          if (p->settle < p->needed)
            {
              continue;
            }

          report_change(i, now);
        }
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(void)
{
  static const char hello[] = "input firmware up";
  uint32_t ticks_done = 0;

  snc_rt_init();
  snc_rt_ready();
  snc_send(SNC_MSG_LOG, hello, sizeof(hello) - 1);

  for (; ; )
    {
      uint16_t type;
      int len;

      snc_rt_wait();

      g_snc_cmd_pending = false;
      while (snc_ring_space(&g_snc_shared->to_m33) >=
             sizeof(struct snc_msg_hdr_s) + SNC_MSG_MAX_PAYLOAD &&
             (len = snc_recv(&type, g_buf, sizeof(g_buf))) >= 0)
        {
          if (snc_rt_system_message(type, g_buf, len))
            {
              continue;
            }

          if (type == SNC_INPUT_CONFIG &&
              len >= (int)sizeof(struct snc_input_config_s))
            {
              configure((const struct snc_input_config_s *)g_buf);
            }
        }

      /* One sample per wake-up, credited with every tick since the last,
       * so a late wake-up neither loses settling time nor invents it.
       */

      if (ticks_done != g_snc_timer_ticks)
        {
          uint32_t now = g_snc_timer_ticks;

          sample(now - ticks_done);
          ticks_done = now;
        }
    }

  return 0;
}
