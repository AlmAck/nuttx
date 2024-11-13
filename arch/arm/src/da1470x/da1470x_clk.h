/**
 * \addtogroup CLOCK_TYPES
 * \{
 */

/**
 * \brief The type of the system clock
 */
typedef enum sys_clk_is_type {
        SYS_CLK_IS_XTAL32M = 0,
        SYS_CLK_IS_RCHS,
        SYS_CLK_IS_RCLP,
        SYS_CLK_IS_PLL,
        SYS_CLK_IS_INVALID
} sys_clk_is_t;

/**
 * \brief The system clock type
 *
 * \note Must only be used with functions cm_sys_clk_init(), cm_sys_clk_set(),
 *      cm_sys_clk_request/release()
 */
typedef enum sysclk_type {
        sysclk_RCHS_32 = 0,     //!< RCHS 32MHz
        sysclk_XTAL32M = 2,     //!< 32MHz
        sysclk_RCHS_64 = 4,     //!< RCHS 64MHz
        sysclk_RCHS_96 = 6,     //!< RCHS 96MHz
        sysclk_PLL160  = 10,    //!< 160MHz
        sysclk_BOOTER  = 11,    //!< leave clock decision up to the booter (defined by CS content)
        sysclk_LP      = 255,   //!< not applicable
} sys_clk_t;

/**
 * \brief The RCHS speed output
 *
 * \note Must only be used with functions hw_clk_set/get_rchs_mode()
 */
typedef enum rchs_speed_type {
        RCHS_32 = 0,    //!< 32MHz
        RCHS_96,        //!< 96MHz
        RCHS_64,        //!< 64MHz
} rchs_speed_t;

/**
 * \brief The CPU clock type (speed)
 */
typedef enum cpu_clk_type {
        cpuclk_2M   = 2,    //!< 2 MHz
        cpuclk_4M   = 4,    //!< 4 MHz
        cpuclk_6M   = 6,    //!< 6 MHz
        cpuclk_8M   = 8,    //!< 8 MHz
        cpuclk_10M  = 10,   //!< 10 MHz
        cpuclk_12M  = 12,   //!< 12 MHz
        cpuclk_16M  = 16,   //!< 16 MHz
        cpuclk_20M  = 20,   //!< 20 MHz
        cpuclk_24M  = 24,   //!< 24 MHz
        cpuclk_32M  = 32,   //!< 32 MHz
        cpuclk_40M  = 40,   //!< 40 MHz
        cpuclk_48M  = 48,   //!< 48 MHz
        cpuclk_64M  = 64,   //!< 64 MHz
        cpuclk_80M  = 80,   //!< 80 MHz
        cpuclk_96M  = 96,   //!< 96 MHz
        cpuclk_160M = 160   //!< 160 MHz
} cpu_clk_t;