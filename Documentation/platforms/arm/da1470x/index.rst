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
and ``tools/da1470x_flash.sh`` resets through the reset pin.  From a
debugger use ``rsettype 2`` before ``r`` (J-Link), or write ``0x1`` to
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

Display controller
------------------

``CONFIG_DA1470X_LCDC`` registers the display controller as ``/dev/fb0``.
A board describes its panel in a ``struct da1470x_lcdc_panel_s`` and
selects one of two interfaces:

* ``DA1470X_LCDC_IF_QSPI``: quad SPI with DCS commands, partial window
  updates, optional tearing-effect synchronisation, RGB565 or RGBA8888.
  Verified on the devkit's AMOLED panel.
* ``DA1470X_LCDC_IF_JDI_PARALLEL``: JDI memory-in-pixel panels.  The
  controller generates XRST, VST, VCK, HST, HCK and ENB and shifts two
  bits per colour with two rows per VCK; the pulse widths follow the
  vendor driver's derivation from the line length, the porches come from
  the panel descriptor, and the VCOM/FRP square wave comes from the
  ``LCD_EXT_CTRL`` divider of the 32 kHz clock so it keeps running in
  sleep.  Frames are always whole-screen; the frame buffer is RGB332.
  **This mode is implemented from the datasheet and the SDK sources and
  has been compiled only: no JDI panel was available to test it.**

With two frame buffers (``CONFIG_DA1470X_LCDC_NBUFFERS=2``) and
``CONFIG_DA1470X_LCDC_ASYNC`` an update taken from another buffer than the
previous one returns at once and a driver thread sends the frame, so a
double-buffering client such as LVGL draws its next frame during the
transfer and the tearing-effect wait.  Updates of the same buffer stay
synchronous.  LVGL's refresh timer sleeps in system ticks: with the
default 10 ms tick its 16 ms period becomes 30 ms, so the ``lvgl``
configuration uses a 1 ms tick.

GPU
---

``CONFIG_DA1470X_GPU`` drives the D/AVE 2D core directly: ``/dev/gpu0``
accepts a rectangle fill with an ARGB colour (blended by its alpha), a
rectangle copy from an RGB565 or ARGB8888 surface (blended by the pixel
alpha times an opacity) and a texture-mapped box (any affine mapping of
such a surface, bilinear filtering and up to four edge limiters, which
draw rotated and scaled images with anti-aliased edges).  The register
recipes (blend factors, the two colour registers carrying the opacity,
texture addressing) were recovered from the display lists the vendor
library builds and verified pixel by pixel; ``apps/examples/gpu2d``
checks them.  The core is a bus master without
the CPU's address remapping and without the flash cache in its path, so
the driver translates addresses in the execute-in-place window: the
region base and offset the boot ROM left in ``CACHE_FLASH_REG`` (the image
starts 0x3400 into the flash) plus the flash controller's second window,
0x20000000 above the first.  Reads from flash are latency bound (the
controller fetches short bursts), so assets the GPU should blit fast
belong in RAM.

``CONFIG_LV_USE_NUTTX_DA1470X_GPU`` adds an LVGL draw unit
(``apps/graphics/lvgl/port/lv_draw_da1470x.c``) that claims blended
rectangle fills and untransformed, fully opaque RGB565/ARGB8888 image
and layer blits; opaque fills stay on the CPU, which stores them as fast
as the GPU.  The
application calls ``lv_draw_da1470x_init()`` after ``lv_init()``; any
task the GPU declines or fails is redrawn by the software renderer.
Rotated and scaled images go through the texture-mapped box; its limiters
fade the last pixel of each edge, so edges look about half a pixel
smaller than LVGL's hard-cut ones.  ``apps/examples/lvgpucheck`` renders
a set of rotated, scaled and translucent images, from RAM and from flash,
with both renderers into a memory display and compares them pixel by
pixel; it also prints the time each took.

Supported Boards
================

.. toctree::
   :glob:
   :maxdepth: 1

   boards/*/*
