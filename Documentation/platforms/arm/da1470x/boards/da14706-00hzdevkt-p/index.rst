===================
DA14706-00HZDEVKT-P
===================

.. tags:: chip:da1470x, chip:da14706

The DA14706-00HZDEVKT-P is the Renesas development kit for the DA14706: a
motherboard with an on-board J-Link debugger, an FT2232 USB-to-serial
bridge, three LEDs, two push buttons, a charger for a Li-ion cell and an
8 MiB QSPI PSRAM, with a daughterboard carrying a 390x390 AMOLED display
(E120A390QSR, RM69091 controller) and a capacitive touch controller.

How It Fits Together
====================

The board's parts reach NuttX through the chip's blocks as follows (see
the chip page's system overview for the blocks themselves).

Buttons K1 and K2
-----------------

Two separate things happen with a press:

* **Waking the chip.**  Bring-up arms a falling-edge interrupt on both
  pins with a handler that does nothing.  Arming it adds a PDC entry for
  the pin, so a press wakes the M33 from its sleep states.
* **Reporting the press.**  ``/dev/buttons`` (``CONFIG_INPUT_BUTTONS_LOWER``)
  reads the pins through ``board_buttons()``.  With the SNC's input
  firmware (``CONFIG_DA1470X_SNC_FW_INPUT``) the controller watches both
  pins, samples them every 5 ms, and sends an event for each press and
  release that has held for 20 ms; the board hands those to the button
  driver, which wakes readers blocked in ``poll()``.  Without it no
  button interrupt is armed for the driver -- one has been seen to wedge
  the system on the first press -- and readers have to poll.

::

   K1/K2 --> GPIO --> PDC entry ---------------------> wakes the M33
            |
            +--> SNC samples, debounces --> SNC_INPUT_EVENT --> SNC2SYS
                 --> LPWORK --> board_button_irq() handler
                 --> button upper half --> poll() on /dev/buttons returns

Touch
-----

The ZT2628's INT line (P1.03) raises an M33 GPIO interrupt only while a
finger is on the glass.  The driver disables it, reads the report over
I2C0 from the low priority work queue, clears and re-enables it, and
wakes readers of ``/dev/input0``.  An idle touch panel therefore costs the
M33 nothing; a reader should block in ``poll()`` without a timeout while
no finger is down.

Display
-------

The LCDC sends frames from the frame buffer to the AMOLED over quad SPI.
While the panel is lit the driver holds the system at PM_NORMAL; the
application dims it with ``da1470x_lcdc_set_brightness()`` and switches it
off with ``FBIOSET_POWER`` 0, which releases the hold.  In always-on mode
the panel keeps showing its own memory in idle mode, the hold is
released, and the driver redraws the clock digits once a minute from a
work item.  The GPU draws into the same frame buffers.

Battery and supplies
--------------------

The charger (``/dev/charger0``) and the ADC (``/dev/adc0``) are M33
drivers.  Watching them with thresholds and waking the M33 only on a
change -- plugged in, unplugged, low battery -- is the kind of job the SNC
is meant for; it is not done yet.

The SNC
-------

At boot the board starts the SNC firmware chosen in Kconfig and registers
``/dev/snc0``.  The firmware runs from RAM1..RAM2, talks to the M33
through RAM8 and the two doorbells, uses TIMER6, and can wake the M33 at
any time; the M33 can wake it the same way.  See :doc:`../../snc`.

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

=========  =====
Signal     PIN
=========  =====
UART1-RX   P1.08
UART1-TX   P1.09
UART1-RTS  P1.10
UART1-CTS  P1.11
=========  =====

LEDs and Buttons
================

====  =====
LED   MCU
====  =====
LED1  P0.31
LED2  P1.01
LED3  P1.02
====  =====

LED1 is also exposed as ``/dev/gpio0``, and the three LEDs as
``/dev/userleds``.

=======  =====
BUTTON   MCU
=======  =====
K1       P1.22
K2       P1.23
=======  =====

K1 is exposed as the interrupt pin ``/dev/gpio1``.  Both buttons are
active low and wake the chip from its sleep states through the PDC.

With ``CONFIG_INPUT_BUTTONS_LOWER`` they are ``/dev/buttons``.  A reader
is woken by a press only when the sensor node controller runs its input
firmware (``CONFIG_DA1470X_SNC_FW_INPUT``): the controller watches both
pins, debounces them and reports each settled press and release, and
the board passes those to the button driver.  Without it no button
interrupt is armed, because arming one on K1/K2 has been seen to wedge
the system on the first press, and readers have to poll.

Buses
=====

======  ================================
Bus     Pins
======  ================================
SPI0    SCLK P0.28, MOSI P0.29, MISO P0.30, CS P0.27 (``/dev/spi0``)
I2C0    SCL P0.24, SDA P0.25, open drain (``/dev/i2c0``)
======  ================================

PSRAM
=====

The motherboard carries an APS6404L 8 MiB QSPI PSRAM (U2) on the chip's
second quad-SPI controller: D0 P1.15, D1 P1.14, D2 P1.13, D3 P1.20, CLK
P1.19, CS P1.24.  Its supply and its connection to the chip are both
switched by RAM_PWRON, which with jumper J12 in its default position is
P1.00; the board raises it before initialising the PSRAM.  P1.13 and
P1.14 are also SPI0's pins on the header, so ``CONFIG_DA1470X_SPI0`` and
``CONFIG_DA1470X_PSRAM`` cannot be enabled together.

Battery and supplies
====================

``CONFIG_DA1470X_CHARGER`` registers the charger as ``/dev/charger0``.  It
is registered disabled and only charges once enabled.  The devkit's
battery connector has no thermistor, so ``CONFIG_DA1470X_CHARGER_NTC``
must stay off unless one is fitted.  ``CONFIG_DA1470X_GPADC`` registers
``/dev/adc0`` with four channels, in millivolts: the battery, the system
supply, the USB supply and pin P0.5.  The driver itself can also read the
internal rails.

Display and touch
=================

The AMOLED is driven by the LCDC in QSPI mode on P0.14..P0.18 and P0.22,
with tearing effect on P0.10, reset on P0.23, the panel DC/DC enable on
P1.07 and the interface mode straps on P0.24/P1.00.
``CONFIG_DA14706_LCD_E120A390QSR`` registers it as ``/dev/fb0``.  The
driver transfers only the updated rectangle, can hold two frame buffers
(``CONFIG_DA1470X_LCDC_NBUFFERS``) selected through ``FBIOPAN_DISPLAY``,
and synchronises transfers to the tearing-effect line
(``CONFIG_DA1470X_LCDC_TE``).  Brightness is set with
``da1470x_lcdc_set_brightness()``, and ``CONFIG_DA1470X_LCDC_AOD`` adds
the always-on mode, in which the panel shows a clock from its idle mode
while the system sleeps.  Do not fade the AMOLED to brightness 0: it
goes black and, after a while, shows artefacts and update bands; use a
low non-zero level, the always-on mode, or ``FBIOSET_POWER`` 0.

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
boot ROM cannot start from when that was the PLL.  Always use the script
for images above 510 KB.

If programming fails with ``Download failed with code: @38000000``, or the
script hangs, erase the chip first and program again::

  ezFlashCLI -j <jlink serial> erase_flash
  tools/da1470x_flash.sh <jlink serial> nuttx.bin

When nothing answers at all, power-cycle the board, erase, power-cycle
again and then program.  The reset cause of an unexpected restart is in
``RESET_STAT_REG`` (0x500000BC), whose bits are sticky: clear it over
SWD before a test so that what it shows afterwards belongs to that test.

JDI parallel panel
------------------

``CONFIG_DA14706_LCD_LPM012M134B`` (instead of the AMOLED board) registers
a JDI LPM012M134B 240x240 memory-in-pixel panel on the JDI parallel
interface.  The devkit does not carry one: the signals use the chip's
fixed JDI pads (VCK P0.09, HCK P0.14, HST P0.15, VST P0.16, ENB P0.18,
XRST P0.22, VCOM/FRP P0.19, RED P0.17/P0.23, GREEN P0.24/P1.00, BLUE
P1.01/P0.21) and the panel enable is ``BOARD_JDI_PEN_PIN`` (P1.07).
Untested on hardware.

The FT2232 console needs a short USB path.  Behind a chain of hubs the
kernel resets the devkit's internal hub on every access and opening
``/dev/ttyUSB0`` fails with an I/O error; the chip itself answers USB
control transfers normally in that state, so the failure is easy to
mistake for a broken adapter or broken firmware.  Plugged into a port
closer to the machine, both directions carry data and the shell runs at
115200 baud.

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
  ./tools/configure.sh da14706-00hzdevkt-p:lvgl
  make -j

  tools/da1470x_flash.sh 900010639 nuttx.bin
  picocom -b 115200 /dev/ttyUSB0      # or any terminal program

The serial number is the one printed by ``JLinkExe`` for the on-board
debugger.  Build ``lvgl_rtt`` instead and use
``tools/da1470x_console.sh 900010639`` when the FT2232 console cannot be
opened.  In the shell that comes up::

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

NSH on UART0 with GPIO, SPI0, I2C0, RTC, the watchdog, PDC, power
management with the tickless time base on the 32 kHz crystal, the charger,
the OQSPI flash partition mounted on ``/mnt/flash`` (NXFFS), the
framebuffer, the GPU, the PWM LED driver and Bluetooth LE (``bt`` tool)
enabled.

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
