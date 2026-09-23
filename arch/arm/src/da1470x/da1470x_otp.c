/****************************************************************************
 * arch/arm/src/da1470x/da1470x_otp.c
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

/* Read-only access to the one-time-programmable memory.  The array holds
 * the factory configuration script, which carries the analogue trim values
 * of this particular die.  Programming is deliberately not implemented:
 * the cells cannot be rewritten and a mistake is permanent.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <debug.h>

#include "arm_internal.h"
#include "da1470x_otp.h"
#include "da1470x_clockconfig.h"
#include "hardware/da1470x_otpc.h"
#include "hardware/da1470x_crg_top.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Controller modes, OTPC_MODE_REG.OTPC_MODE_MODE */

#define OTP_MODE_DSTBY         0  /* Deep standby, cell powered down */
#define OTP_MODE_STBY          1  /* Standby, cell powered up */
#define OTP_MODE_READ          2  /* Array readable through its window */

/* How long the controller may take to report the requested mode.  The
 * transition is a few cell cycles; the loop count only has to be large
 * enough to cover it at the highest clock.
 */

#define OTP_MODE_RETRIES       100000

/* Timing parameters that do not depend on the clock, see OTPC_TIM1_REG and
 * OTPC_TIM2_REG.  These are the power-up and program pulse widths given in
 * microseconds, so they are the same at any AMBA clock.
 */

#define OTP_TIM1_FIXED         (OTPC_TIM1_TIM1_US_T_PL(9) | \
                                OTPC_TIM1_TIM1_US_T_CS(9) | \
                                OTPC_TIM1_TIM1_US_T_CSP(9))

#define OTP_TIM2_FIXED         (OTPC_TIM2_TIM2_US_T_PW(9)   | \
                                OTPC_TIM2_TIM2_US_T_PWI(0)  | \
                                OTPC_TIM2_TIM2_US_T_PPR(4)  | \
                                OTPC_TIM2_TIM2_US_T_PPS(4)  | \
                                OTPC_TIM2_TIM2_US_T_VDS(0)  | \
                                OTPC_TIM2_TIM2_US_T_PPH(4)  | \
                                OTPC_TIM2_TIM2_US_T_SAS(1)  | \
                                OTPC_TIM2_TIM2_US_ADD_CC_EN)

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Nesting count of da1470x_otp_acquire() */

static uint8_t g_otp_refs;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: otp_amba_clock
 *
 * Description:
 *   Return the frequency of the clock the controller runs on, in MHz,
 *   rounded up.  That is the AMBA high speed clock, the system clock
 *   divided by CLK_AMBA_REG.HCLK_DIV.
 *
 ****************************************************************************/

static uint32_t otp_amba_clock(void)
{
  uint32_t regval = getreg32(DA1470X_CRG_TOP_CLK_AMBA);
  uint32_t div    = (regval & CRG_TOP_CLK_AMBA_HCLK_DIV_MASK) >>
                    CRG_TOP_CLK_AMBA_HCLK_DIV_SHIFT;
  uint32_t hclk   = da1470x_get_sysclk() >> div;

  return (hclk + 999999) / 1000000;
}

/****************************************************************************
 * Name: otp_set_timings
 *
 * Description:
 *   Program the cell timings for the current AMBA clock.  The three clock
 *   dependent fields count controller cycles for a 1us, a 20ns and a 120ns
 *   interval; the rest is expressed in microseconds and is fixed.
 *
 ****************************************************************************/

static void otp_set_timings(void)
{
  uint32_t mhz = otp_amba_clock();
  uint32_t t1us;
  uint32_t t20ns;
  uint32_t trd;

  t1us  = mhz - 1;
  t20ns = ((mhz + 49) / 50) - 1;
  trd   = ((mhz + 15) / 16) - 1;

  putreg32(OTP_TIM1_FIXED |
           OTPC_TIM1_TIM1_CC_T_1US(t1us & 0xff) |
           OTPC_TIM1_TIM1_CC_T_20NS(t20ns & 0x7) |
           OTPC_TIM1_TIM1_CC_T_RD(trd & 0xf),
           DA1470X_OTPC_TIM1);

  putreg32(OTP_TIM2_FIXED, DA1470X_OTPC_TIM2);
}

/****************************************************************************
 * Name: otp_set_mode
 *
 * Description:
 *   Switch the controller to the given mode and wait until it reports the
 *   cell ready.
 *
 * Returned Value:
 *   Zero on success, -ETIMEDOUT if the controller never becomes ready.
 *
 ****************************************************************************/

static int otp_set_mode(uint32_t mode)
{
  int retries;

  putreg32(OTPC_MODE_MODE_MODE(mode), DA1470X_OTPC_MODE);

  for (retries = OTP_MODE_RETRIES; retries > 0; retries--)
    {
      if ((getreg32(DA1470X_OTPC_STAT) & OTPC_STAT_STAT_MRDY) != 0)
        {
          return OK;
        }
    }

  return -ETIMEDOUT;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: da1470x_otp_acquire
 ****************************************************************************/

int da1470x_otp_acquire(void)
{
  irqstate_t flags;
  uint32_t regval;
  int ret;

  flags = enter_critical_section();

  if (g_otp_refs > 0)
    {
      g_otp_refs++;
      leave_critical_section(flags);
      return OK;
    }

  /* Clock the controller.  Only the OTP gate is touched: the same register
   * carries the flash divider, which must not change.
   */

  regval  = getreg32(DA1470X_CRG_TOP_CLK_AMBA);
  regval |= CRG_TOP_CLK_AMBA_OTP_ENABLE;
  putreg32(regval, DA1470X_CRG_TOP_CLK_AMBA);

  /* Power the cell up, program the timings for the current clock, then
   * open the read window.
   */

  ret = otp_set_mode(OTP_MODE_STBY);
  if (ret >= 0)
    {
      otp_set_timings();
      ret = otp_set_mode(OTP_MODE_READ);
    }

  if (ret < 0)
    {
      putreg32(OTPC_MODE_MODE_MODE(OTP_MODE_DSTBY), DA1470X_OTPC_MODE);

      regval  = getreg32(DA1470X_CRG_TOP_CLK_AMBA);
      regval &= ~CRG_TOP_CLK_AMBA_OTP_ENABLE;
      putreg32(regval, DA1470X_CRG_TOP_CLK_AMBA);

      _err("ERROR: OTP controller not ready: %d\n", ret);
    }
  else
    {
      g_otp_refs = 1;
    }

  leave_critical_section(flags);
  return ret;
}

/****************************************************************************
 * Name: da1470x_otp_release
 ****************************************************************************/

void da1470x_otp_release(void)
{
  irqstate_t flags;
  uint32_t regval;

  flags = enter_critical_section();

  if (g_otp_refs > 0 && --g_otp_refs == 0)
    {
      putreg32(OTPC_MODE_MODE_MODE(OTP_MODE_DSTBY), DA1470X_OTPC_MODE);

      regval  = getreg32(DA1470X_CRG_TOP_CLK_AMBA);
      regval &= ~CRG_TOP_CLK_AMBA_OTP_ENABLE;
      putreg32(regval, DA1470X_CRG_TOP_CLK_AMBA);
    }

  leave_critical_section(flags);
}

/****************************************************************************
 * Name: da1470x_otp_read32
 ****************************************************************************/

uint32_t da1470x_otp_read32(unsigned int index)
{
  DEBUGASSERT(index < DA1470X_OTP_NWORDS);

  return getreg32(DA1470X_OTPC_MEM_BASE + (index << 2));
}

/****************************************************************************
 * Name: da1470x_otp_read
 ****************************************************************************/

int da1470x_otp_read(unsigned int index, uint32_t *dest, size_t nwords)
{
  size_t i;
  int ret;

  if (index + nwords > DA1470X_OTP_NWORDS)
    {
      return -EINVAL;
    }

  ret = da1470x_otp_acquire();
  if (ret < 0)
    {
      return ret;
    }

  for (i = 0; i < nwords; i++)
    {
      dest[i] = da1470x_otp_read32(index + i);
    }

  da1470x_otp_release();
  return OK;
}
