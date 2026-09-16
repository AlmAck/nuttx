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

Display
=======

The AMOLED is driven by the LCDC in QSPI mode on P0.14..P0.18 and P0.22,
with tearing effect on P0.10, reset on P0.23 and the panel DC/DC enable on
P1.07.  ``CONFIG_DA14706_LCD_E120A390QSR`` registers it as ``/dev/fb0``.

Flashing
========

Build the ``nsh_cpuapp`` configuration and program the OQSPI flash with the
Renesas ``ezFlashCLI`` tool through the on-board J-Link::

  ./tools/configure.sh da14706-00hzdevkt-p:nsh_cpuapp
  make
  ezFlashCLI -j <jlink serial> image_flash nuttx.bin

The tool places the image at flash offset 0x3400 and only programs the
first 512 KiB of the flash, so ``nuttx.bin`` must stay below about
510 KB; a larger image boots with a truncated ``.data`` section.

Debugging without the UART
--------------------------

When the FT2232 console is not available, the Segger RTT console works
through the on-board J-Link: enable ``CONFIG_SEGGER_RTT``,
``CONFIG_SERIAL_RTT0`` and ``CONFIG_SERIAL_RTT_CONSOLE`` and select "No
serial console".  Then run ``JLinkExe -device Cortex-M33 -if SWD
-RTTTelnetPort 19021``, give it ``exec SetRTTAddr <address of
_SEGGER_RTT>`` (from ``nm nuttx``) before ``connect``, and attach
``telnet localhost 19021`` to get NSH.

Configurations
==============

nsh_cpuapp
----------

NSH on UART0 with GPIO, SPI0, I2C0, RTC, PDC, power management, the OQSPI
flash partition mounted on ``/mnt/flash`` (NXFFS), the framebuffer, the
GPU, the PWM LED driver and Bluetooth LE (``bt`` tool) enabled.
