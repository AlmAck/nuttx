===================
DA14706-00HZDEVKT-P
===================

.. tags:: chip:da1470x, chip:da14706

The DA14706-00HZDEVKT-P is the Renesas development kit for the DA14706 with
a 390x390 AMOLED display (E120A390QSR, RM69091 controller), three LEDs, two
push buttons, an on-board J-Link debugger and an FT2232 USB-to-serial
bridge.

Serial Console
==============

UART0 is routed to the first FT2232 channel and is the NSH console at
115200 8N1.

========  =====
Signal    PIN
========  =====
UART0-TX  P0.08
UART0-RX  P2.01
========  =====

UART1 is available on the header with flow control:

========  =====
Signal    PIN
========  =====
UART1-RX  P1.08
UART1-TX  P1.09
UART1-RTS P1.10
UART1-CTS P1.11
========  =====

LEDs and Buttons
================

====  =====
LED   MCU
====  =====
LED1  P0.31
LED2  P1.01
LED3  P1.02
====  =====

LED1 is also exposed as ``/dev/gpio0``.

=======  =====
BUTTON   MCU
=======  =====
K1       P1.22
K2       P1.23
=======  =====

K1 is exposed as the interrupt pin ``/dev/gpio1`` and wakes the chip from
standby through the PDC.

Buses
=====

======  ================================
Bus     Pins
======  ================================
SPI0    SCLK P0.28, MOSI P0.29, MISO P0.30, CS P0.27 (``/dev/spi0``)
I2C0    SCL P0.24, SDA P0.25, open drain (``/dev/i2c0``)
======  ================================

Display and touch
=================

The AMOLED is driven by the LCDC in QSPI mode on P0.14..P0.18 and P0.22,
with tearing effect on P0.10, reset on P0.23, the panel DC/DC enable on
P1.07 and the interface mode straps on P0.24/P1.00.
``CONFIG_DA14706_LCD_E120A390QSR`` registers it as ``/dev/fb0``.  The
driver transfers only the updated rectangle, can hold two frame buffers
(``CONFIG_DA1470X_LCDC_NBUFFERS``) selected through ``FBIOPAN_DISPLAY``,
and synchronises transfers to the tearing-effect line
(``CONFIG_DA1470X_LCDC_TE``).

The display board carries a Zinitix ZT2628 capacitive touch controller
on I2C0 (SCL P1.12, SDA P1.11), interrupt on P1.03, reset on P1.01 and a
supply switch on P0.28.  ``CONFIG_DA14706_TOUCH_ZT2628`` registers it as
``/dev/input0``.

Flashing
========

Build the ``nsh_cpuapp`` configuration and program the OQSPI flash with the
Renesas ``ezFlashCLI`` tool through the on-board J-Link::

  ./tools/configure.sh da14706-00hzdevkt-p:nsh_cpuapp
  make
  ezFlashCLI -j <jlink serial> image_flash nuttx.bin

The tool places the image at flash offset 0x3400 and only programs the
first 512 KiB of the flash.  ``tools/da1470x_flash.sh <serial> nuttx.bin``
wraps it: it writes the part of a larger image past that boundary
separately and then resets the board through the reset pin under the
debugger.  A plain reset request is not enough after programming: the
programmer leaves the flash controller out of its memory-mapped mode,
and a reset request keeps the previous firmware's clock tree, which the
boot ROM cannot start from when that was the PLL.  Always use the script for images above 510 KB.

JDI parallel panel
------------------

``CONFIG_DA14706_LCD_LPM012M134B`` (instead of the AMOLED board) registers
a JDI LPM012M134B 240x240 memory-in-pixel panel on the JDI parallel
interface.  The devkit does not carry one: the signals use the chip's
fixed JDI pads (VCK P0.09, HCK P0.14, HST P0.15, VST P0.16, ENB P0.18,
XRST P0.22, VCOM/FRP P0.19, RED P0.17/P0.23, GREEN P0.24/P1.00, BLUE
P1.01/P0.21) and the panel enable is ``BOARD_JDI_PEN_PIN`` (P1.07).
Untested on hardware.

Checking the UART without wiring
--------------------------------

``apps/examples/uartloop`` tests the UART when the board's USB serial
adapter cannot be used.  It reads back the divisor the driver programmed
and compares it with the one the requested baud rate needs, then sends a
pattern through the controller's internal loopback.  The loopback cannot
prove the bit rate, since both ends share the divisor, which is why the
divisor is checked directly.  Measured on the devkit::

  9600 baud     divisor 208+5/16   error 0.00%
  115200 baud   divisor 17+6/16    error 0.08%
  921600 baud   divisor 2+3/16     error 0.79%

The receive pin needs a pull-up.  The controller reports busy for as long
as the receive line is low, and while it is busy it refuses the write
that clears the divisor latch access bit; left set, that bit keeps the
transmit register hidden and nothing is sent at all.  The driver checks
the bit and resets the controller rather than run blind, but a floating
or low receive line still stops reception.

Console over the debugger (RTT)
-------------------------------

When the FT2232 console is not available, the NuttShell runs over Segger
RTT through the on-board J-Link.  A configuration needs
``CONFIG_SEGGER_RTT``, ``CONFIG_SERIAL_RTT0`` and
``CONFIG_SERIAL_RTT_CONSOLE`` with "No serial console" selected; the
``lvgl_rtt`` configuration below is built that way.  Raise
``CONFIG_SEGGER_RTT_BUFFER_SIZE_DOWN`` from its 16-byte default, or
anything typed is truncated after 16 characters.

``tools/da1470x_console.sh <serial> [nuttx ELF] [command ...]`` opens it.
It reads the address of the RTT control block from the ELF, so that ELF
has to be the image running on the board.  Without commands it is an
interactive terminal (leave with Ctrl-C); with commands it sends each
one, prints the answer and exits, which is what scripts want::

  tools/da1470x_console.sh 900010639                     # interactive
  tools/da1470x_console.sh 900010639 nuttx "free" "gpu2d"

It never resets the board, so it can also be attached to firmware that
is already running.

Running an example
------------------

Configure, build, flash, then call the example by name on the console::

  cd nuttx
  ./tools/configure.sh da14706-00hzdevkt-p:lvgl_rtt
  make -j

  tools/da1470x_flash.sh 900010639 nuttx.bin
  tools/da1470x_console.sh 900010639

The serial number is the one printed by ``JLinkExe`` for the on-board
debugger.  In the shell that comes up::

  nsh> gpu2d                  # GPU fill, blit and rotation self-test
  nsh> lvgpucheck             # GPU draw unit against LVGL's renderer
  nsh> lvgldemo widgets       # LVGL widgets demo on the panel
  nsh> lvgldemo benchmark     # LVGL benchmark, prints a summary table
  nsh> fb                     # framebuffer rectangles
  nsh> gpio -o 1 /dev/gpio0   # drive LED1

``lvgldemo`` keeps running until the demo ends; the benchmark takes about
four minutes and prints its table at the end.  The graphical ones draw on
the panel, so watch the display, not the console.

Configurations
==============

nsh_cpuapp
----------

NSH on UART0 with GPIO, SPI0, I2C0, RTC, PDC, power management, the OQSPI
flash partition mounted on ``/mnt/flash`` (NXFFS), the framebuffer, the
GPU, the PWM LED driver and Bluetooth LE (``bt`` tool) enabled.

lvgl
----

Everything in ``nsh_cpuapp`` plus LVGL 9.2.1 with the widgets and
benchmark demos (``lvgldemo widgets``), two frame buffers, tearing-effect
sync, the touch panel, the FPU, the 160 MHz PLL and the GPU draw unit.
The image is about 1 MB: flash it with ``tools/da1470x_flash.sh``.  The
console is UART0.

lvgl_rtt
--------

The same as ``lvgl`` but with the console on Segger RTT instead of UART0,
and with ``lvgpucheck`` built in.  Use it when the FT2232 console is not
usable, which is the case on the development machine this port was
written on.
