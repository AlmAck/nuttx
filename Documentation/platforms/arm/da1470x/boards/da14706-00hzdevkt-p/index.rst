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

The display board's ZT2628 touch controller has no driver in this port
yet.

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

The FT2232 console needs a short USB path.  Behind a chain of hubs the
host may reset the kit's internal hub on every access and opening the
serial port fails with an I/O error; plug the kit into a port close to
the machine.

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

P1.13 and P1.14, used by SPI0 on the header, are also data lines of the
PSRAM, so ``CONFIG_DA1470X_SPI0`` and ``CONFIG_DA1470X_PSRAM`` cannot be
enabled together.

Display

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

The display board also carries a Zinitix ZT2628 touch controller; a
driver for it is not part of this port yet.

``CONFIG_DA14706_LCD_LPM012M134B`` (instead of the AMOLED) registers a JDI
LPM012M134B 240x240 memory-in-pixel panel on the JDI parallel interface.
The kit does not carry one: the signals use the chip's fixed JDI pads (VCK
P0.09, HCK P0.14, HST P0.15, VST P0.16, ENB P0.18, XRST P0.22, VCOM/FRP
P0.19, RED P0.17/P0.23, GREEN P0.24/P1.00, BLUE P1.01/P0.21) and the panel
enable is ``BOARD_JDI_PEN_PIN`` (P1.07).  Untested on hardware.

PSRAM
=====

The motherboard carries an APS6404L 8 MiB QSPI PSRAM on the chip's second
QSPI controller.  Its supply and its connection to the chip are both
switched by RAM_PWRON, which with jumper J12 in its default position is
P1.00; the board raises it before initialising the PSRAM.
``CONFIG_DA1470X_PSRAM`` maps it at 0x28000000 and, with
``CONFIG_DA1470X_PSRAM_HEAP``, gives it a heap of its own.

Flashing
========

Program the OQSPI flash with the Renesas ``ezFlashCLI`` tool through the
on-board J-Link::

  ./tools/configure.sh da14706-00hzdevkt-p:nsh
  make
  ezFlashCLI -j <jlink serial> image_flash nuttx.bin

The tool places the image at flash offset 0x3400.  Two things to know:

* ``ezFlashCLI`` programs only the first 512 KiB of the flash in one
  operation.  A larger image has to be written in two parts.
* After programming, reset the board through the reset pin, for example
  with J-Link Commander (``rsettype 2``, ``r``, ``g``).  A reset request
  from the debugger is not enough: the programmer leaves the flash
  controller out of its memory-mapped mode, and a reset request keeps the
  previous firmware's clock tree, which the boot ROM cannot start from
  when that was the PLL.

Console over the debugger (RTT)
-------------------------------

When the FT2232 console is not available, the NuttShell can run over
Segger RTT through the on-board J-Link: enable ``CONFIG_SEGGER_RTT``,
``CONFIG_SERIAL_RTT0`` and ``CONFIG_SERIAL_RTT_CONSOLE`` with "No serial
console" selected.  Raise ``CONFIG_SEGGER_RTT_BUFFER_SIZE_DOWN`` from its
16-byte default, or anything typed is truncated after 16 characters.

Configurations
==============

nsh
---

NSH on UART0 with the RTC, the watchdog, the PDC, power management and
the tickless time base clocked by the 32 kHz crystal.  The rest of the
peripherals are enabled through their ``CONFIG_DA1470X_*`` options.
