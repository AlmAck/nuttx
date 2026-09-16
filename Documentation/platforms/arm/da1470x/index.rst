===============
Renesas DA1470x
===============

The DA1470x family (DA14701, DA14705, DA14706, DA14708) from Renesas (formerly
Dialog Semiconductor) integrates an Arm Cortex-M33 application processor
(CPUAPP), a Cortex-M0+ Bluetooth Low Energy controller (CMAC), a Cortex-M0+
sensor node controller (SNC), a 2.5D GPU (D/AVE 2D), a display controller,
power management and a rich set of peripherals.

NuttX runs on the CPUAPP core and executes in place (XiP) from the OQSPI
boot flash.  The CMAC core runs the Renesas controller firmware and is used
as a Bluetooth HCI controller; the SNC is not used.

Memory Map
==========

DA14706 as seen by the CPUAPP.  RAM3..RAM7 are used by NuttX, RAM8/RAM9 are
added to the heap unless Bluetooth is enabled, and RAM10 is reserved for the
CMAC firmware.

============ ============= ====== ==============================
Block Name   Start Address Length Notes
============ ============= ====== ==============================
OQSPI flash  0x00000000    8 MiB  XiP window, remapped at 0
SYSRAM       0x20000000    64 KiB RAM0..RAM2, ROM booter / retention
SYSRAM       0x20010000    1 MiB  RAM3..RAM7, NuttX text copy, data, heap
RAM8         0x20110000    128 K  Shared, heap without BLE
RAM9         0x20130000    128 K  CMAC data with BLE, heap otherwise
RAM10        0x20150000    192 K  CMAC firmware
CMAC regs    0x40000000           Controller register window
============ ============= ====== ==============================

Clock Configuration
===================

The system clock is chosen with Kconfig: the 32 MHz crystal
(``CONFIG_DA1470X_CLOCK_XTAL32M_SRC``, default and required for the RTC,
the PDC ``EN_XTAL`` flag and the BLE controller), the system PLL at
160 MHz fed by the crystal (``CONFIG_DA1470X_CLOCK_PLL160_SRC``, the
choice for graphics work; the core runs at 1.2 V and the flash
controller at 80 MHz with the read settings the boot ROM programmed) or
the RC high-speed oscillator at 32, 64 or 96 MHz.  The peripheral clock
(DIVN) is always 32 MHz.  The low-power clock defaults to the internal
RCLP; select ``XTAL32K`` for an accurate RTC and for controller sleep.

A core reset request (``up_systemreset``, or ``r`` in a debugger) does
not reset the clock tree, and the boot ROM hangs when it starts with the
PLL selected.  ``board_reset`` therefore resets through the watchdog,
and ``tools/da1470x_flash.sh`` puts the clocks back before running the
ROM.  When resetting from a debugger by hand, write ``0x1`` to
``CLK_CTRL_REG`` (0x50000014), ``0x1040`` to ``CLK_AMBA_REG``
(0x50000000) and ``0xE8A0`` to ``PLL_SYS_CTRL1_REG`` (0x50050460) while
the core is halted at the reset vector.

The system tick is the Cortex-M SysTick fed by the system clock.

Peripheral Support
==================

==========  ======= ========================================================
Peripheral  Support Notes
==========  ======= ========================================================
GPIO        Yes     Three ports, WKUP controller interrupts, pad latches
UART        Yes     UART0..UART2, interrupt driven, RTS/CTS on UART1/2
SPI         Yes     SPI0..SPI2 masters, polled FIFO exchange
I2C         Yes     I2C0..I2C2 masters, interrupt driven, 7/10-bit
DMA         Yes     Arch-private channel API, used by peripherals
TIMER       No
RTC         Yes     Calendar, alarm, ``/dev/rtc0``
PDC         Yes     Wake-up LUT, master entries for CM33 and CMAC
PM          Yes     CONFIG_PM lower half, standby with XTAL32M and WFI
OQSPI       Yes     Boot flash as MTD, XiP-safe program/erase from RAM
LCDC        Yes     Framebuffer driver, QSPI panels, tearing-effect sync
GPU         Yes     Register-level D/AVE 2D fill and blit, ``/dev/gpu0``
PWMLED      Yes     Three LED sinks as a PWM lower half
BLE         Yes     CMAC firmware loader and HCI transport over mailbox
SNC         No
USB         No
GPADC       No
Audio       No
Charger     No
==========  ======= ========================================================

Flash Controller (OQSPI)
------------------------

The boot flash (MX25U6432 on the devkit) is exposed as an MTD device by
``CONFIG_DA1470X_OQSPI_MTD``.  A partition starting at
``CONFIG_DA1470X_OQSPI_MTD_OFFSET`` is registered as ``/dev/mtd0``.  Because
the code runs from the same flash, all routines that leave the controller's
auto mode live in ``.ramfunc`` and run with interrupts disabled: every
sector erase blocks interrupts for tens of milliseconds.  Keep the
partition away from the NuttX image.

Power Management
----------------

``CONFIG_DA1470X_PM`` registers a CONFIG_PM lower half.  IDLE is a plain
WFI.  STANDBY switches the system clock to XTAL32M, stops the PLL and the
RC oscillator and enters Cortex-M deep sleep; any NVIC interrupt (RTC alarm, GPIO wake-up,
UART) wakes the core.  SLEEP behaves as STANDBY unless
``CONFIG_DA1470X_PM_EXTENDED_SLEEP`` is set, which is reserved for a full
PD_SYS power-off with context restore and is not yet implemented.

The PDC driver (``CONFIG_DA1470X_PDC``) manages the wake-up look-up table;
the board adds entries for the RTC alarm and the push buttons.

Bluetooth LE
------------

``CONFIG_DA1470X_BLE`` loads the CMAC controller firmware into RAM10 and
registers a NuttX Bluetooth network driver.  The firmware area is not part
of the NuttX tree; extract it from the SDK once with::

  ./tools/da1470x_cmac_fw.sh <sdk>/sdk/interfaces/ble/binaries/DA1470x-Release/libble_stack_da1470x.a <sdk>/cmac_fw.bin

and point ``CONFIG_DA1470X_CMAC_FW_PATH`` (relative to the NuttX directory)
at the result.  The host/controller protocol (boot handshake, two 504-byte
rings, H4 framing) was recovered from the SDK objects for firmware
"CMAC v1.0.0" of SDK 10.2.6.49; other SDK releases move the table addresses
in ``da1470x_cmac.c``.  Radio trim values from OTP are not applied yet.

Supported Boards
================

.. toctree::
   :glob:
   :maxdepth: 1

   boards/*/*
