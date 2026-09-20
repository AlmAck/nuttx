==============================
Sensor node controller (SNC)
==============================

The third processor on the part: a Cortex-M0+ with the PD_SNC
peripherals wired to it -- the UARTs, the SPI and I2C controllers, the
DMA -- and two RAM cells of its own.  It keeps running while the rest of
the system sleeps, which is the whole point of it.

Why it matters here
===================

A wake of the application processor was measured on this board at about
39 uC.  That converts directly:

=====================  =========================
Wake rate              Cost on the M33
=====================  =========================
once a minute          0.65 uA
1 Hz                   39 uA
100 Hz                 3.9 mA
=====================  =========================

Against an extended sleep specified at 19.7 uA, a sensor polled at a
hundred hertz from the M33 costs more than two orders of magnitude more
than the sleep it interrupts.  The same poll on this core costs a
fraction of it, and the M33 is told only when something has happened.

What is done
============

``CONFIG_DA1470X_SNC`` builds ``da1470x_snc.c``, which loads an image and
starts the core.  Three registers, all in ``CLK_SNC_CTRL_REG``
(0x5000002C): power PD_SNC, set ``SNC_CLK_ENABLE``, clear
``SNC_RESET_REQ``.  The controller's watchdog drives its NMI and is
frozen on the way in (``GPREG SET_FREEZE`` bit 11), so a firmware that
never heard of it is not reset by it.

Memory, which is the part worth remembering::

    RAM1   32 KiB   "SNC code"   M33 0x20000000   SNC 0x000000
    RAM2   32 KiB   "SNC data"   M33 0x20008000   SNC 0x008000
    RAM8  128 KiB   shared       M33 0x20110000   SNC 0x030000

The core starts at its own address zero, so the first two words of RAM1
are its stack pointer and reset vector.  The board's linker script
already keeps RAM1 and RAM2 out of NuttX's way.

Its firmware is a different architecture from the rest of the build and
cannot come out of the same compiler invocation -- the same reason the
radio controller's firmware arrives as a blob.
``tools/da1470x_snc_fw.sh`` assembles a source file and prints the C
array to paste in.

``CONFIG_DA1470X_SNC_SMOKETEST`` runs 36 bytes built that way: a firmware
that signs its own RAM and then counts, so that "it ran" can be told
apart from "the RAM happened to hold something".  Measured on the kit,
16015 iterations of a three instruction loop in 10 ms.

What is not done
================

**Inter-processor communication.**  There is no convention yet for the
two processors to talk.  The hardware for it is RAM8, which both can
reach, and the ``SYS2SNC`` and ``SNC2SYS`` triggers in the power domain
controller, which let either wake the other.  Until that exists the
controller can only be started and inspected, which is enough to prove it
runs and not enough to use it.

**Its clock.**  The measured rate works out around 10 MHz once bus
arbitration against the M33 is allowed for, against a 32 MHz maximum.
Where the divider sits, and whether it is worth moving, has not been
established.

**State retention across PD_SNC going down.**  The datasheet gives the
controller a retained flag for exactly this
(``CLK_SNC_CTRL_REG[SNC_STATE_RETAINED]``) and says the firmware is
responsible for saving and restoring itself around it.  Nothing here uses
it, so today the controller only survives while its domain does.

A note on measuring it
======================

The current a running SNC draws could not be measured by starting and
stopping it over SWD: attaching a debugger wakes the system and pins it
at its active current, which swamps what is being looked for.  A clean
number needs two builds measured separately, with no probe attached.
