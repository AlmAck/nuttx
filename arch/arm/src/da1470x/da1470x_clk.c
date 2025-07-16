// /****************************************************************************
//  * arch/arm/src/da1470x/da1470x_clk.c
//  *
//  * Licensed to the Apache Software Foundation (ASF) under one or more
//  * contributor license agreements.  See the NOTICE file distributed with
//  * this work for additional information regarding copyright ownership.  The
//  * ASF licenses this file to you under the Apache License, Version 2.0 (the
//  * "License"); you may not use this file except in compliance with the
//  * License.  You may obtain a copy of the License at
//  *
//  *   http://www.apache.org/licenses/LICENSE-2.0
//  *
//  * Unless required by applicable law or agreed to in writing, software
//  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
//  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
//  * License for the specific language governing permissions and limitations
//  * under the License.
//  *
//  ****************************************************************************/

// /****************************************************************************
//  * Included Files
//  ****************************************************************************/

// #include <nuttx/config.h>

// #include <sys/types.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <unistd.h>
// #include <string.h>
// #include <assert.h>
// #include <errno.h>
// #include <debug.h>

// #include <nuttx/irq.h>
// #include <nuttx/arch.h>

// #include "da1470x_clk.h"
// #include "hardware/da1470x_clock.h"
// #include "hardware/da1470x_crg_top.h"

// static sys_clk_t sysclk = SYSCLK_LP;      // Invalidate system clock
// static sys_clk_t sysclk_booter = SYSCLK_LP;
// static sys_clk_t sys_clk_next;

// static inline bool hw_clk_is_xtalm_started(void)
// {
//     return (getreg32(DA1470_CRG_XTAL_XTAL32M_STAT0) & CRG_XTAL_XTAL32M_READY) != 0;
// }

// void cm_enable_xtalm(void)
// {
//     irqstate_t flags = enter_critical_section();

// #if CONFIG_USE_HW_PDC
//     if (xtal32_pdc_entry == HW_PDC_INVALID_LUT_INDEX)
//     {
//         /* Find or create a PDC entry for enabling XTAL32M */
//         xtal32_pdc_entry = get_pdc_xtal32m_entry();

//         if (xtal32_pdc_entry == HW_PDC_INVALID_LUT_INDEX)
//         {
//             xtal32_pdc_entry = hw_pdc_add_entry(HW_PDC_TRIGGER_FROM_MASTER(HW_PDC_MASTER_CM33,
//                                                                            HW_PDC_LUT_ENTRY_EN_XTAL));
//         }

//         DEBUGASSERT(xtal32_pdc_entry != HW_PDC_INVALID_LUT_INDEX);

//         /* Use PDC to start XTAL32M if it is not already started */
//         hw_pdc_set_pending(xtal32_pdc_entry);
//         hw_pdc_acknowledge(xtal32_pdc_entry);

//         /* Allow PDC to disable XTAL32M when going to sleep */
//         hw_clk_disable_sysclk(SYS_CLK_IS_XTAL32M);
//     }
// #endif

//     // xtal32m_settled = hw_clk_is_xtalm_started();

//     if (!hw_clk_is_xtalm_started())
//         {
//             if (!hw_clk_is_enabled_sysclk(SYS_CLK_IS_XTAL32M))
//                 {
// #if CONFIG_USE_HW_PDC
//                     /* Use PDC to start XTAL32M */
//                     hw_pdc_set_pending(xtal32_pdc_entry);
//                     hw_pdc_acknowledge(xtal32_pdc_entry);
// #else
//                     /* Directly enable XTAL32M */
//                     hw_clk_enable_sysclk(SYS_CLK_IS_XTAL32M);
// #endif
//                 }
//         }

//     leave_critical_section(flags);
// }

// static inline bool hw_clk_is_enabled_sysclk(sys_clk_is_t clk)
// {
//     switch (clk) {
//     case SYS_CLK_IS_XTAL32M:
//         return hw_clk_check_xtalm_status();
//     case SYS_CLK_IS_RCHS:
//         return hw_clk_check_rchs_status();
//     case SYS_CLK_IS_PLL:
//         return hw_clk_check_pll_status();
//     default:
//         DEBUGASSERT(0);
//         return false;
//     }
// }

// static inline bool hw_clk_check_xtalm_status(void)
// {
//     return (getreg32(DA1470_CRG_XTAL_XTAL32M_STAT0) & CRG_XTAL_XTAL32M_READY) != 0;
// }

// /****************************************************************************
//  * Name: hw_clk_check_rchs_status
//  *
//  * Description:
//  *   Check if the RCHS is enabled.
//  *
//  * Returned Value:
//  *   true if the RCHS is enabled, else false.
//  *
//  ****************************************************************************/

// static inline bool hw_clk_check_rchs_status(void)
// {
//     return (getreg32(DA1470_CRG_TOP_CLK_RCHS) & CRG_TOP_RCHS_ENABLE) != 0;
// }

// /****************************************************************************
//  * Name: hw_clk_check_pll_status
//  *
//  * Description:
//  *   Check if the system PLL (160MHz) is enabled.
//  *
//  * Returned Value:
//  *   true if the PLL is enabled, else false.
//  *
//  ****************************************************************************/

// static inline bool hw_clk_check_pll_status(void)
// {
//     return (getreg32(DA1470_CRG_XTAL_PLL_SYS_CTRL1) & CRG_XTAL_PLL_EN) != 0;
// }

// // void cm_sys_clk_init(sys_clk_t type)
// // {
// //     DEBUGASSERT(type != SYS_CLK_IS_LP); /* Not Applicable! */

// //     /* Initialize mutex if necessary */
// //     CM_MUTEX_CREATE();

// // #ifdef CONFIG_OS_PRESENT
// //     xEventGroupCM_xtal = OS_EVENT_GROUP_CREATE();
// //     DEBUGASSERT(xEventGroupCM_xtal != NULL);
// // #endif /* CONFIG_OS_PRESENT */

// //     // ahbclk = cm_ahb_get_clock_divider();
// //     // apbclk = cm_apb_get_clock_divider();
// //     // apb_slowclk = cm_apb_slow_get_clock_divider();
// //     // ahb_clk_next = ahbclk;

// //     /* Get current voltage configuration */
// //     HW_PMU_1V2_RAIL_CONFIG rail_config;
// //     hw_pmu_get_1v2_active_config(&rail_config);

// // #if !CONFIG_PMU_ADAPTER
// //     vdd_voltage = rail_config.voltage;
// // #endif /* CONFIG_PMU_ADAPTER */

// //     /* Disable RCHS unless it is the system clock */
// //     hw_clk_disable_sysclk(SYS_CLK_IS_RCHS);

// //     irqstate_t flags = enter_critical_section();
// //     sysclk = sysclk_booter = cm_sys_clk_get();

// //     if (type == SYS_CLK_IS_BOOTER) {
// //         sys_clk_next = sysclk_booter;
// //     } else {
// //         sys_clk_next = type;
// //         if (sys_clk_next == SYS_CLK_IS_RCHS_32 || sys_clk_next == SYS_CLK_IS_RCHS_64 || sys_clk_next == SYS_CLK_IS_RCHS_96)
// //             {
// //                 rchs_speed_t rchs_mode = (sys_clk_next == SYS_CLK_IS_RCHS_32) ? RCHS_32 :
// //                                         (sys_clk_next == SYS_CLK_IS_RCHS_64) ? RCHS_64 : RCHS_96;
// //                 if (sys_clk_next != SYS_CLK_IS_RCHS_32)
// //                     {
// //                         pmu_1v2_set_max_voltage();
// //                     }
// //                 switch_to_rchs(rchs_mode);
// //             }
// //         else
// //         {
// //                 cm_enable_xtalm();
// //                 if (cm_poll_xtalm_ready())
// //                     {
// //                         switch_to_xtal32m();
// //                         hw_clk_disable_sysclk(SYS_CLK_IS_RCHS);
// //                         if (sys_clk_next == SYS_CLK_IS_PLL160)
// //                             {
// //                                 if (hw_clk_is_pll_locked())
// //                                     {
// //                                         switch_to_pll();
// //                                     }
// //                                 else
// //                                     {
// //                                         enable_pll();
// //                                     }
// //                             }
// //                         else
// //                             {
// //                                 disable_pll();
// // #ifdef CONFIG_OS_PRESENT
// //                                 OS_EVENT_GROUP_CLEAR_BITS(xEventGroupCM_xtal, PLL_AVAILABLE);
// // #endif
// //                             }
// //                     }
// //             }
// //     }
// //     sysclk = sys_clk_next;
// //     CM_MUTEX_GET();

// //     if (sys_clk_cnt_ind) {
// //         default_sys_clk_index = index_find_in_prio_list(sysclk);
// //     } else {
// //         pll_count = (sys_clk_next == SYS_CLK_IS_PLL160) ? 1 : 0;
// //     }

// //     CM_MUTEX_PUT();
// //     leave_critical_section(flags);
// // }


// /**
//  * \brief Switch to PLL.
//  *
//  * \details XTAl32M needs to be the running system clock in order to switch to PLL.
//  */

// static void switch_to_pll(void)
// {
// #if defined(CONFIG_HW_USB) && defined(CONFIG_USB_ENUMERATION)

//     /* USB device should not be in use */

//     DEBUGASSERT(!hw_usb_active());
// #endif /* CONFIG_HW_USB */

//     if (hw_clk_get_sysclk() == SYS_CLK_IS_XTAL32M)
//     {
//         /* Ensure power regulation is in order before switching */

//         while ((getreg32(DA1470_CRG_TOP_SYS_STAT) & CRG_TOP_POWER_IS_UP) == 0);

//         /* Wait for VDD to settle before switching system clock to PLL */

//         while ((getreg32(DA1470_CRG_TOP_ANA_STATUS) & CRG_TOP_BUCK_DCDC_V12_OK) == 0);

//         hw_clk_set_sysclk(SYS_CLK_IS_PLL); /* Set PLL as system clock */
//     }
// }

// /****************************************************************************
//  * Name: hw_clk_get_sysclk
//  *
//  * Description:
//  *   Return the clock used as the system clock.
//  *
//  * Returned Value:
//  *   The type of the system clock
//  *
//  ****************************************************************************/
// static inline sys_clk_is_t hw_clk_get_sysclk(void)
// {
//     static const uint32_t freq_msk = CRG_TOP_RUNNING_AT_RCLP |
//                                      CRG_TOP_RUNNING_AT_RCHS |
//                                      CRG_TOP_RUNNING_AT_XTAL32M |
//                                      CRG_TOP_RUNNING_AT_PLL;

//     static const sys_clk_is_t clocks[] = {
//         SYS_CLK_IS_RCLP,        /* 0b000 */
//         SYS_CLK_IS_RCHS,        /* 0b001 */
//         SYS_CLK_IS_XTAL32M,     /* 0b010 */
//         SYS_CLK_IS_INVALID,
//         SYS_CLK_IS_PLL          /* 0b100 */
//     };

//     /* Drop bit0 to reduce the size of clocks[] */

//     uint32_t index = (getreg32(DA1470_CRG_TOP_CLK_CTRL) & freq_msk) >> (CRG_TOP_RUNNING_AT_RCLP + 1);

//     DEBUGASSERT(index <= 4);

//     sys_clk_is_t clk = clocks[index];
//     DEBUGASSERT(clk != SYS_CLK_IS_INVALID);

//     return clk;
// }


// // // static void memories_sys_clock_cfg(sys_clk_t clk)
// // // {
// // //         adjust_otp_access_timings();

// // // #if (dg_configUSE_HW_OQSPI == 1)
// // //         oqspi_automode_sys_clock_cfg(clk);
// // // #endif

// // // #if (dg_configUSE_HW_QSPI == 1) || (dg_configUSE_HW_QSPI2 == 1)
// // //         qspi_automode_sys_clock_cfg(clk);
// // // #endif
// // // }

// // // /**
// // //  * \brief Adjust OTP access timings according to the AHB clock frequency.
// // //  *
// // //  * \warning In mirrored mode, the OTP access timings are left unchanged since the system is put to
// // //  *          sleep using the RC32M clock and the AHB divider set to 1, which are the same settings
// // //  *          that the system runs after a power-up or wake-up!
// // //  */
// // // static __RETAINED_CODE void adjust_otp_access_timings(void)
// // // {
// // // #if (dg_configUSE_HW_OTPC == 1)
// // //         if (hw_otpc_is_active()) {
// // //                 uint32_t clk_freq = get_clk_freq(sys_clk_next, ahb_clk_next);
// // //                 HW_OTPC_SYS_CLK_FREQ freq = hw_otpc_convert_sys_clk_mhz(clk_freq);
// // //                 ASSERT_ERROR(freq != HW_OTPC_SYS_CLK_FREQ_INVALID_VALUE);
// // //                 hw_otpc_set_speed(freq);
// // //         }
// // // #endif
// // // }

// /****************************************************************************
//  * Name: switch_to_rchs
//  *
//  * Description:
//  *   Switch to RCHS.
//  *
//  * Input Parameters:
//  *   mode - The speed of the RCHS output.
//  *
//  ****************************************************************************/
// static void switch_to_rchs(rchs_speed_t mode)
// {
//     hw_clk_enable_sysclk(SYS_CLK_IS_RCHS);

//     sys_clk_t clk = (mode == RCHS_96) ? SYSCLK_RCHS_96 :
//                     (mode == RCHS_32) ? SYSCLK_RCHS_32 : SYSCLK_RCHS_64;

//     // /* When switching from lower to higher system clock frequency, the memories and their
//     //    controllers must be reconfigured before clock switching */

//     // if (hw_clk_get_sysclk() <= clk) {
//     //     memories_sys_clock_cfg(clk);
//     // }

//     hw_clk_set_rchs_mode(mode);
//     hw_clk_set_sysclk(SYS_CLK_IS_RCHS); /* Set RCHS as system clock */

//     // /* When switching from higher to lower system clock frequency, the memories and their
//     //    controllers must be reconfigured after clock switching */

//     // if (hw_clk_get_sysclk() > clk) {
//     //     memories_sys_clock_cfg(clk);
//     // }

//     /* Disable RCHS. It will remain enabled as long as it is the system clock */
//     hw_clk_disable_sysclk(SYS_CLK_IS_RCHS);
// }

// /****************************************************************************
//  * Name: hw_clk_set_rchs_mode
//  *
//  * Description:
//  *   Set the speed of the RCHS output.
//  *
//  * Input Parameters:
//  *   mode - The speed of the RCHS output.
//  *
//  * Notes:
//  *   Switching to/from 64MHz requires the RCHS to settle, which can take more
//  *   than 100 microseconds. Switching between 32MHz and 96MHz does not require
//  *   settling.
//  *
//  ****************************************************************************/

// void hw_clk_set_rchs_mode(rchs_speed_t mode)
// {
//     uint32_t reg_value;

//     /* Initialize reg_value with default reset values for the CLK_RCHS_REG fields */

//     reg_value = ((0x5  << CRG_TOP_RCHS_INIT_DTC_POS)  |
//                 (0x2  << CRG_TOP_RCHS_INIT_DTCF_POS) |
//                 (0x80 << CRG_TOP_RCHS_INIT_DEL_POS)  |
//                 (0x1  << CRG_TOP_RCHS_INIT_RANGE_POS));

//     /* Determine the appropriate trim value based on the mode */

//     if ((mode & RCHS_64) != 0)
//         {
//         if (rchs_64_mode_trim_value)
//             {
//             reg_value = rchs_64_mode_trim_value;
//             }
//         }
//     else
//         {
//         reg_value = rchs_32_96_mode_trim_value;
//         }

//     /* Set the RCHS speed */

//     reg_value |= (mode << CRG_TOP_RCHS_SPEED_POS);

//     /* Disable global interrupts before modifying the clock settings */

//     irqstate_t flags = enter_critical_section();

//     /* Ensure V12 level voltage is set to 1.2V before setting RCHS at 64MHz/96MHz */

//     DEBUGASSERT((mode == RCHS_32) ||
//                 (getreg32(CRG_TOP_POWER_LVL_REG) & POWER_LVL_REG_V12_LEVEL_MASK) == POWER_LVL_REG_V12_LEVEL_1P2V);

//     /* Apply the new RCHS mode settings */

//     modifyreg32(DA1470_CRG_TOP_CLK_RCHS,
//                 (RCHS_REG_TRIM | CRG_TOP_RCHS_SPEED_MASK),
//                 reg_value);

//     /* Restore global interrupts */

//     leave_critical_section(flags);
//  }

// /****************************************************************************
//  * Name: hw_clk_set_sysclk
//  *
//  * Description:
//  *   Set the system clock.
//  *
//  * Input Parameters:
//  *   mode - The new system clock.
//  *
//  * Notes:
//  *   System clock switch to PLL is only allowed when the current system clock
//  *   is XTAL32M. System clock switch from PLL is only allowed when the new
//  *   system clock is XTAL32M.
//  *
//  ****************************************************************************/

// static inline void hw_clk_set_sysclk(sys_clk_is_t mode)
// {
//     /* Make sure a valid sys clock is requested */

//     DEBUGASSERT(mode <= SYS_CLK_IS_PLL);

//     /* Switch to PLL is only allowed when current system clock is XTAL32M */

//     DEBUGASSERT(mode != SYS_CLK_IS_PLL ||
//                 (getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_XTAL32M) ||
//                 (getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_PLL));

//     /* Switch from PLL is only allowed when new system clock is XTAL32M */

//     DEBUGASSERT(!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_PLL) ||
//                 mode == SYS_CLK_IS_XTAL32M ||
//                 mode == SYS_CLK_IS_PLL);

//     uint32_t regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);

//     if (mode == SYS_CLK_IS_XTAL32M && (regval & CRG_TOP_RUNNING_AT_RCHS) != 0)
//         {
//             putreg32(0x01, CRG_TOP_SWITCH2XTAL);
//         }
//     else
//         {
//             regval &= ~CRG_TOP_SYS_CLK_SEL_MASK; /* Clear the bits for SYS_CLK_SEL */
//             regval |= (mode << CRG_TOP_SYS_CLK_SEL_POS) & CRG_TOP_SYS_CLK_SEL_MASK; /* Set new mode */
//             putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);
//         }

//     /* Wait until the switch is done */

//     switch (mode)
//     {
//         case SYS_CLK_IS_XTAL32M:
//             while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_XTAL32M));
//             return;

//         case SYS_CLK_IS_RCHS:
//             while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_RCHS));
//             return;

//         case SYS_CLK_IS_RCLP:
//             while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_RCLP));
//             return;

//         case SYS_CLK_IS_PLL:
//             while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & CRG_TOP_RUNNING_AT_PLL));
//             return;
//         default:
//             DEBUGASSERT(0);
//     }
// }

// // /**
// //  * \brief Deactivate a System clock.
// //  *
// //  * \param[in] clk The clock to deactivate.
// //  */

// // static inline void hw_clk_disable_sysclk(sys_clk_is_t clk)
// // {
// //     switch (clk)
// //     {
// //         case SYS_CLK_IS_XTAL32M:
// //             hw_clk_disable_xtalm();
// //             return;
// //         case SYS_CLK_IS_RCHS:
// //             hw_clk_disable_rchs();
// //             return;
// //         case SYS_CLK_IS_PLL:
// //             hw_clk_pll_sys_off();
// //             return;
// //         default:

// //             /* An invalid clock is requested */

// //             DEBUGASSERT(0);
// //     }
// // }

// static void switch_to_xtal32m(void)
// {
//     if (hw_clk_get_sysclk() != SYS_CLK_IS_XTAL32M)
//     {
//         ASSERT_WARNING(hw_clk_is_xtalm_started());

//         hw_clk_set_sysclk(SYS_CLK_IS_XTAL32M);          // Set XTAL32 as sys_clk
//         if (sysclk > SYS_CLK_IS_XTAL32M)                // fast --> slow clock switch
//         {
//                 // memories_sys_clock_cfg(SYS_CLK_IS_XTAL32M);
//         }
//     }
// }

// /****************************************************************************
//  * XTAL32M and System PLL Control Functions
//  ****************************************************************************/

// /**
//  * @brief Check if the XTAL32M is enabled.
//  *
//  * @return true if the XTAL32M is enabled, else false.
//  */
// static bool hw_clk_check_xtalm_status(void)
// {
//   /* Read the XTAL32M status register and test the READY bit */
//   return (getreg32(DA1470_CRG_XTAL_XTAL32M_STAT0) & CRG_XTAL_XTAL32M_READY) != 0;
// }

// /**
//  * @brief Activate the XTAL32M.
//  */
// static void hw_clk_enable_xtalm(void)
// {
//   /* Do nothing if XTAL32M is already running */
//   if (hw_clk_check_xtalm_status())
//     {
//       return;
//     }

//   /* In a complete design the power supply for XTAL32M must be checked.
//    * That check is assumed to be performed elsewhere.
//    */

//   GLOBAL_INT_DISABLE();
//   /* Enable XTAL32M by setting the ENABLE bit in the control register.
//    * According to the header, CRG_XTAL_XTAL32M_ENABLE is defined as (1U << 8).
//    */
//   putreg32(getreg32(DA1470_CRG_XTAL_XTAL32M_CTRL) | CRG_XTAL_XTAL32M_ENABLE,
//            DA1470_CRG_XTAL_XTAL32M_CTRL);
//   GLOBAL_INT_RESTORE();
// }