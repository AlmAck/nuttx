===============
Renesas DA1470x
===============

The DA1470x family (DA14701, DA14705, DA14706, DA14708) from Renesas (formerly
Dialog Semiconductor) integrates an Arm Cortex-M33 application processor
(CPUAPP), a Cortex-M0+ Bluetooth Low Energy controller (CMAC), a Cortex-M0+
sensor node controller (SNC), a 2.5D GPU (D/AVE 2D), a display controller,
power management with a battery charger, and a rich set of peripherals.

NuttX runs on the CPUAPP core and executes in place (XiP) from the OQSPI
boot flash.  The CMAC core runs the Renesas controller firmware and is used
as a Bluetooth HCI controller.  The SNC runs a firmware built from source
with NuttX and exchanges messages with it; see :doc:`snc`.

Memory Map
==========

DA14706 as seen by the CPUAPP.  RAM3..RAM7 are used by NuttX.  With
``CONFIG_MM_REGIONS`` above 1, RAM8 and RAM9 are added to the heap unless
Bluetooth or the SNC claims them.  RAM10 is reserved for the CMAC firmware.

============ ============= ====== =========================================
Block Name   Start Address Length Notes
============ ============= ====== =========================================
OQSPI flash  0x00000000    8 MiB  XiP window, remapped at 0
SYSRAM       0x20000000    64 KiB RAM1..RAM2, SNC firmware (code and data)
SYSRAM       0x20010000    1 MiB  RAM3..RAM7, NuttX data and heap
RAM8         0x20110000    128 K  Shared with the SNC when it is enabled
RAM9         0x20130000    128 K  CMAC data with BLE
RAM10        0x20150000    192 K  CMAC firmware
CMAC regs    0x40000000           Controller register window
PSRAM        0x28000000    8 MiB  QSPIC2 window, optional external PSRAM
============ ============= ====== =========================================

System RAM is in its own power domain (PD_MEM): switching a peripheral
domain off, PD_SNC included, does not clear it.

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

System time comes from the Cortex-M SysTick or, with
``CONFIG_DA1470X_TICKLESS``, from TIMER2 clocked by the low-power clock,
which keeps counting while the core sleeps and wraps every 512 s.  A write
of its compare value takes a couple of low-power clock cycles to reach the
counter and a second write inside that window is dropped, so the driver
waits for the timer's busy flag around each write and raises the interrupt
itself if the counter has already passed the target; without that, a
missed compare stalls the system until the next wrap.

In a tickless kernel ``CLOCK_MONOTONIC`` is the scheduler tick count, which
advances only when a timer expires somewhere in the system: it is as fresh
as the last expiry, and stands still across a busy loop with no timer due.
``CLOCK_BOOTTIME`` reads the timer itself; use it to time short intervals.

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
TIMER       Yes     Tickless time base (TIMER2), PWM (TIMER3..5), SNC (TIMER6)
RTC         Yes     Calendar, alarm, ``/dev/rtc0``
WDT         Yes     System watchdog as ``/dev/watchdog0``
PDC         Yes     Wake-up LUT, entries for CM33, CMAC and SNC
PM          Yes     CONFIG_PM lower half, clock lowering and deep sleep
OQSPI       Yes     Boot flash as MTD, XiP-safe program/erase from RAM
QSPIC2      Yes     QSPI PSRAM at 0x28000000, optional heap of its own
LCDC        Yes     Framebuffer, QSPI and JDI panels, brightness, always-on
GPU         Yes     Register-level D/AVE 2D fill, blit, texture mapping
PWMLED      Yes     Three LED sinks as a PWM lower half
GPADC       Yes     Supply channels and four pins, in millivolts
Charger     Yes     Battery charger lower half, ``/dev/charger0``
Audio       Yes     Microphone capture and voice activity detector
OTP/TCS     Yes     Factory trim values, applied at boot and for the radio
BLE         Yes     CMAC firmware loader and HCI transport over mailbox
SNC         Yes     Firmware built from source, mailbox, deep sleep
USB         No
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
RC oscillator and enters Cortex-M deep sleep; any NVIC interrupt (RTC
alarm, GPIO wake-up, UART, the SNC's doorbell) wakes the core, and the
clock is restored when the system returns to NORMAL.  SLEEP behaves as
STANDBY unless ``CONFIG_DA1470X_PM_EXTENDED_SLEEP`` is set, which lets the
PMU switch PD_SYS off with the core context saved in retained RAM; that
is experimental, see :doc:`extended_sleep`.

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
in ``da1470x_cmac.c``.  The factory radio trim values from the OTP
configuration script (``CONFIG_DA1470X_TCS``) are handed to the
controller.

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

While the panel is lit the driver holds the system at PM_NORMAL, because
the interface clock derives from the system clock; ``FBIOSET_POWER`` 0
releases it.  The interface clock divider is chosen again for each frame,
so frames stay within the panel's limit when the power manager has
lowered or restored the system clock.

Brightness
~~~~~~~~~~

Panels with a brightness control (the devkit's AMOLED, through DCS
``WRDISBV``) are driven with ``da1470x_lcdc_set_brightness(level)``
(``<arch/chip/lcdc.h>``).  The call waits for a frame in flight to finish,
since a command sent in the middle of one would land in the pixel stream.
The level is kept across power cycles: a level set while the panel is off
is applied when it comes back on.  Level 0 does not switch an AMOLED off
and should not be used to blank it; ``FBIOSET_POWER`` or the always-on
mode below are the ways to rest the panel.

Always-on display
~~~~~~~~~~~~~~~~~

``CONFIG_DA1470X_LCDC_AOD`` lets the driver keep a clock on the panel
while everything above it sleeps.  On ``DA1470X_FBIOC_AOD_IDLE`` it draws
a face for the given time on the whole screen, puts the panel in its DCS
idle mode and releases the PM_NORMAL hold; a low priority work item then
redraws, just after each minute boundary, only the rows the digits occupy.
Any frame from the client, ``DA1470X_FBIOC_AOD_RESUME``, or switching the
panel off leaves the mode again.

A face is a recipe rather than a picture: an AODF v1 record of 1-bit
glyph masks, RGB222 colours and up to 16 items, generated by the
stm32_stream_bridge project's ``tools/aod_face_gen.py`` and validated in
full, with a CRC, before use.  One is compiled in
(``CONFIG_DA1470X_LCDC_AOD_BUILTIN_FACE``); others are uploaded with the
``DA1470X_FBIOC_AOD_FACE_*`` ioctls and kept in RAM.  v1 faces are
240 x 240, drawn centred on larger panels.  On the devkit, parked for
three minutes across midnight, the face was redrawn every minute and the
system spent about 93% of the time in its sleep state.

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
controller fetches short bursts, and a rotation reads texels in random
order), so the LVGL draw unit copies flash-resident images to a RAM cache
on first use (``CONFIG_LV_DA1470X_GPU_TEXCACHE_KB``, 64 KiB by default,
least recently used copies dropped first).

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

External PSRAM
--------------

``CONFIG_DA1470X_PSRAM`` brings up an AP Memory APS6404 QSPI PSRAM on the
second quad-SPI controller (QSPIC2), in PD_CTRL, and maps it at
0x28000000.  The part is identified and its data and address lines are
tested before use; ``CONFIG_DA1470X_PSRAM_HEAP`` gives it a heap of its
own (``da1470x_psram_malloc()``), kept apart from the system heap.  The
window is cacheable: anything a DMA engine reads from it has to be
cleaned from the cache first.

Sensor node controller
----------------------

``CONFIG_DA1470X_SNC`` runs a firmware on the Cortex-M0+ -- built from
``arch/arm/src/da1470x/snc/`` with the same toolchain and embedded in the
image -- and carries messages both ways through RAM8 (``/dev/snc0`` and
``<arch/chip/snc.h>``).  It keeps running while the M33 sleeps and wakes it
only when something has happened; its deep sleep survives PD_SNC being
switched off.  See :doc:`snc`.

.. toctree::
   :maxdepth: 1

   snc

Work in progress
================

.. toctree::
   :maxdepth: 1

   extended_sleep

Supported Boards
================

.. toctree::
   :glob:
   :maxdepth: 1

   boards/*/*
