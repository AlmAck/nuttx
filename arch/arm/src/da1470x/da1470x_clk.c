// void cm_sys_clk_init(sys_clk_t type)
// {
//         CM_MUTEX_CREATE();                                  // Create Mutex. Called only once!
// #ifdef OS_PRESENT
//         xEventGroupCM_xtal = OS_EVENT_GROUP_CREATE();       // Create Event Group
//         ASSERT_WARNING(xEventGroupCM_xtal != NULL);
// #endif /* OS_PRESENT */
//         ahbclk = cm_ahb_get_clock_divider();
//         apbclk = cm_apb_get_clock_divider();
//         apb_slowclk = cm_apb_slow_get_clock_divider();

//         ahb_clk_next = ahbclk;

//         ASSERT_WARNING(type != sysclk_LP);                  // Not Applicable!

//         HW_PMU_1V2_RAIL_CONFIG rail_config;
//         hw_pmu_get_1v2_active_config(&rail_config);
// #if (dg_configPMU_ADAPTER == 0)
//         vdd_voltage = rail_config.voltage;
// #endif /* dg_configPMU_ADAPTER */

//         /*
//          * Disable RCHS. RCHS will remain enabled by the hardware as long as it is used
//          * as system clock.
//          */
//         hw_clk_disable_sysclk(SYS_CLK_IS_RCHS);

//         CM_ENTER_CRITICAL_SECTION();

//         sysclk = sysclk_booter = cm_sys_clk_get();

//         if (type == sysclk_BOOTER) {
//                 sys_clk_next = sysclk_booter;
//         } else {
//                 sys_clk_next = type;
//                 if ((sys_clk_next == sysclk_RCHS_32) || (sys_clk_next == sysclk_RCHS_64) || (sys_clk_next == sysclk_RCHS_96)) {
//                         rchs_speed_t rchs_mode;
//                         switch (sys_clk_next) {
//                         case sysclk_RCHS_32:
//                                 rchs_mode = RCHS_32;
//                                 break;
//                         case sysclk_RCHS_64:
//                                 pmu_1v2_set_max_voltage();
//                                 rchs_mode = RCHS_64;
//                                 break;
//                         case sysclk_RCHS_96:
//                                 pmu_1v2_set_max_voltage();
//                                 rchs_mode = RCHS_96;
//                                 break;
//                         default:
//                                 rchs_mode = RCHS_32;
//                                 ASSERT_WARNING(0);
//                         }
//                         switch_to_rchs(rchs_mode);
//                 } else {
//                         cm_enable_xtalm();

//                         /*
//                          * Note: In case the XTAL32M (or PLL) has not settled (or locked) yet, then we
//                          *       simply set the cm_sysclk to the user setting and skip waiting for the
//                          *       XTAL32M to settle. In this case, the system clock will be set to the
//                          *       XTAL32M (or the PLL) when the XTAL32M_RDY_IRQn hits. Every task or Adapter
//                          *       must block until the requested system clock is available. Sleep may have to
//                          *       be blocked as well.
//                          */
//                         if (cm_poll_xtalm_ready()) {
//                                 switch_to_xtal32m();

//                                 hw_clk_disable_sysclk(SYS_CLK_IS_RCHS);

//                                 if (sys_clk_next == sysclk_PLL160) {
//                                         if (hw_clk_is_pll_locked()) {
//                                                 switch_to_pll();
//                                         }
//                                         else {
//                                                 // System clock will be switched to PLL when PLL is locked
//                                                 enable_pll();
//                                         }
//                                 }
//                                 else {
//                                         disable_pll();
// #ifdef OS_PRESENT
//                                         OS_EVENT_GROUP_CLEAR_BITS(xEventGroupCM_xtal, PLL_AVAILABLE);
// #endif
//                                 }
//                         }
//                 }
//         }
//         sysclk = sys_clk_next;

//         CM_MUTEX_GET();
//         if (sys_clk_cnt_ind) {
//                 default_sys_clk_index = index_find_in_prio_list(sysclk);
//         } else {
//                 pll_count = (sys_clk_next == sysclk_PLL160) ? 1 : 0;
//         }
//         CM_MUTEX_PUT();

//         CM_LEAVE_CRITICAL_SECTION();
// }


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
//         //memories_sys_clock_cfg(sysclk_PLL160);

//         /*
//          * If ultra-fast wake-up mode is used, make sure that the startup state
//          * machine is finished and all power regulation is in order.
//          */
//         while ((getreg32(DA1470_CRG_TOP_SYS_STAT) & (CRG_TOP_POWER_IS_UP)) == 0);

//         /*
//          * Core voltage may have been changed from 0.9V to 1.2V.
//          * Wait for VDD to settle in order to switch system clock to PLL.
//          */
//         while ((getreg32(DA1470_CRG_TOP_ANA_STATUS) & (CRG_TOP_BUCK_DCDC_V12_OK)) == 0);

//         hw_clk_set_sysclk(SYS_CLK_IS_PLL);  /* Set PLL as sys_clk */
//     }
// }

// /**
//  * @brief Return the clock used as the system clock.
//  *
//  * @return The type of the system clock
//  */
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


// // static void memories_sys_clock_cfg(sys_clk_t clk)
// // {
// //         adjust_otp_access_timings();

// // #if (dg_configUSE_HW_OQSPI == 1)
// //         oqspi_automode_sys_clock_cfg(clk);
// // #endif

// // #if (dg_configUSE_HW_QSPI == 1) || (dg_configUSE_HW_QSPI2 == 1)
// //         qspi_automode_sys_clock_cfg(clk);
// // #endif
// // }

// // /**
// //  * \brief Adjust OTP access timings according to the AHB clock frequency.
// //  *
// //  * \warning In mirrored mode, the OTP access timings are left unchanged since the system is put to
// //  *          sleep using the RC32M clock and the AHB divider set to 1, which are the same settings
// //  *          that the system runs after a power-up or wake-up!
// //  */
// // static __RETAINED_CODE void adjust_otp_access_timings(void)
// // {
// // #if (dg_configUSE_HW_OTPC == 1)
// //         if (hw_otpc_is_active()) {
// //                 uint32_t clk_freq = get_clk_freq(sys_clk_next, ahb_clk_next);
// //                 HW_OTPC_SYS_CLK_FREQ freq = hw_otpc_convert_sys_clk_mhz(clk_freq);
// //                 ASSERT_ERROR(freq != HW_OTPC_SYS_CLK_FREQ_INVALID_VALUE);
// //                 hw_otpc_set_speed(freq);
// //         }
// // #endif
// // }

// // /**
// //  * \brief Switch to RCHS.
// //  *
// //  * \details Set RCHS as the system clock.
// //  */
// // static void switch_to_rchs(rchs_speed_t mode)
// // {
// //         hw_clk_enable_sysclk(SYS_CLK_IS_RCHS);

// //         sys_clk_t clk = (mode == RCHS_96) ? sysclk_RCHS_96 :
// //                         (mode == RCHS_32) ? sysclk_RCHS_32 : sysclk_RCHS_64;

// //         /* When switching from lower to higher system clock frequency, the memories and their
// //            controllers must be reconfigured before clock switching  */
// //         if (sysclk <= clk) {
// //                 memories_sys_clock_cfg(clk);
// //         }

// //         hw_clk_set_rchs_mode(mode);
// //         hw_clk_set_sysclk(SYS_CLK_IS_RCHS);     // Set RCHS as sys_clk

// //         /* When switching from higher to lower system clock frequency, the memories and their
// //            controllers must be reconfigured after clock switching  */
// //         if (sysclk > clk) {
// //                 memories_sys_clock_cfg(clk);
// //         }

// //         /*
// //          * Disable RCHS. RCHS will remain enabled by the hardware as long as it is used
// //          * as system clock.
// //          */
// //         hw_clk_disable_sysclk(SYS_CLK_IS_RCHS);
// // }

// // /**
// //  * \brief Set the speed of RCHS output.
// //  *
// //  * \param[in] mode The speed of the RCHS output.
// //  *
// //  * \note Switching to/from 64MHz requires the RCHS to settle, which can be > 100us.
// //  *       Switching 32MHz to/from 96MHz does not require settling.
// //  */
// // void hw_clk_set_rchs_mode(rchs_speed_t mode);

// // /**
// //  * \brief Get the speed of RCHS output.
// //  *
// //  * \return the speed of the RCHS output.
// //  */
// // __STATIC_INLINE rchs_speed_t hw_clk_get_rchs_mode(void)
// // {
// //         rchs_speed_t rchs_speed = REG_GETF(CRG_TOP, CLK_RCHS_REG, RCHS_SPEED);

// //         if (rchs_speed & RCHS_64) {
// //                 return RCHS_64;
// //         } else {
// //                 return rchs_speed;
// //         }
// // }

// /**
//  * \brief Set System clock.
//  *
//  * \param[in] mode The new system clock.
//  *
//  * \note System clock switch to PLL is only allowed when current system clock is XTAL32M.
//  * System clock switch from PLL is only allowed when new system clock is XTAL32M.
//  */
// static inline void hw_clk_set_sysclk(sys_clk_is_t mode)
// {
//         /* Make sure a valid sys clock is requested */
//         DEBUGASSERT(mode <= SYS_CLK_IS_PLL);

//         /* Switch to PLL is only allowed when current system clock is XTAL32M */
//         DEBUGASSERT(mode != SYS_CLK_IS_PLL ||
//                 (getreg32(DA1470_CRG_TOP_CLK_CTRL) & (CRG_TOP_RUNNING_AT_XTAL32M)) ||
//                 (getreg32(DA1470_CRG_TOP_CLK_CTRL) & (CRG_TOP_RUNNING_AT_PLL)));

//         /* Switch from PLL is only allowed when new system clock is XTAL32M */
//         DEBUGASSERT(!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & (CRG_TOP_RUNNING_AT_PLL)) ||
//                 mode == SYS_CLK_IS_XTAL32M ||
//                 mode == SYS_CLK_IS_PLL);

//         if (mode == SYS_CLK_IS_XTAL32M && ((regval & RUNNING_AT_RCHS) != 0)) {

//                 putreg32(0x01, DA1470_CRG_TOP_CLK_SWITCH2XTAL);

//         } else {

//                 regval = getreg32(DA1470_CRG_TOP_CLK_CTRL);             // Read current register value
//                 regval &= ~CRG_TOP_SYS_CLK_SEL_MASK;                          // Clear the bits for SYS_CLK_SEL
//                 regval |= (mode << CRG_TOP_SYS_CLK_SEL_POS) & CRG_TOP_SYS_CLK_SEL_MASK;  // Set the new mode value in SYS_CLK_SEL
//                 putreg32(regval, DA1470_CRG_TOP_CLK_CTRL);            // Write back the modified register value

//         }

//         /* Wait until the switch is done! */
//         switch (mode) {
//         case SYS_CLK_IS_XTAL32M:
//                 while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & (CRG_TOP_RUNNING_AT_XTAL32M))) {
//                 }
//                 return;

//         case SYS_CLK_IS_RCHS:
//                 while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & (CRG_TOP_RUNNING_AT_RCHS))) {
//                 }
//                 return;

//         case SYS_CLK_IS_RCLP:
//                 while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & (CRG_TOP_RUNNING_AT_RCLP))) {
//                 }
//                 return;

//         case SYS_CLK_IS_PLL:
//                 while (!(getreg32(DA1470_CRG_TOP_CLK_CTRL) & (CRG_TOP_RUNNING_AT_PLL))) {
//                 }
//                 return;
//         default:
//                 DEBUGASSERT(0);
//         }
// }

// /**
//  * \brief Deactivate a System clock.
//  *
//  * \param[in] clk The clock to deactivate.
//  */
// __STATIC_INLINE void hw_clk_disable_sysclk(sys_clk_is_t clk)
// {
//         switch (clk) {
//         case SYS_CLK_IS_XTAL32M:
//                 hw_clk_disable_xtalm();
//                 return;
//         case SYS_CLK_IS_RCHS:
//                 hw_clk_disable_rchs();
//                 return;
//         case SYS_CLK_IS_PLL:
//                 hw_clk_pll_sys_off();
//                 return;
//         default:
//                 /* An invalid clock is requested */
//                 ASSERT_WARNING(0);
//         }
// }

// static void switch_to_xtal32m(void)
// {
//         if (hw_clk_get_sysclk() != SYS_CLK_IS_XTAL32M) {
//                 // ASSERT_WARNING(hw_clk_is_xtalm_started());

//                 hw_clk_set_sysclk(SYS_CLK_IS_XTAL32M);          // Set XTAL32 as sys_clk
//                 if (sysclk > sysclk_XTAL32M) {                 // fast --> slow clock switch
//                         memories_sys_clock_cfg(sysclk_XTAL32M);
//                 }
//         }
// }