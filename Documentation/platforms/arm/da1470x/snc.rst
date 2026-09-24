==============================
Sensor node controller (SNC)
==============================

The third processor on the part: a Cortex-M0+ at up to 32 MHz that reaches
every peripheral at the same addresses as the M33 and keeps running while
the M33 sleeps, which is the whole point of it.

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

Overview
========

``CONFIG_DA1470X_SNC`` builds a firmware for the controller from
``arch/arm/src/da1470x/snc/`` as part of the NuttX build, embeds it in the
image, loads and starts it at boot, and carries messages between the two
processors.  User space reaches it through ``/dev/snc0``; kernel code
through ``da1470x_snc_send()`` and ``da1470x_snc_register_handler()``
(``<arch/chip/snc.h>``).

Memory
======

============ ============= ============= ======= ==========================
Cells        SNC address   M33 address   Size    Use
============ ============= ============= ======= ==========================
RAM1 + RAM2  0x00000000    0x20000000    64 KiB  Firmware code, data, stack
RAM8         0x00030000    0x20110000    128 KiB Shared area and mailbox
============ ============= ============= ======= ==========================

The board's linker script keeps RAM1 and RAM2 out of NuttX's way, and with
the SNC configured RAM8 is kept out of the heap.  In the shared area, an
SNC address is the M33 address minus ``0x200E0000``.

Firmware
========

A firmware is ``snc_start.S`` (vector table, image header, reset), the
runtime ``snc_rt.c`` and one application, chosen with
``CONFIG_DA1470X_SNC_FW_*``.  It is compiled for the Cortex-M0+ with the
same toolchain, freestanding, linked with ``snc/snc.ld`` for address zero,
turned into a binary and included with ``.incbin``
(``da1470x_snc_image.S``).  It can use the chip's register headers
(``hardware/*.h``) and ``<arch/chip/snc.h>``.

The image carries a header right after the vector table, at offset
``0xC0``: magic ``SNC1``, the version of the mailbox protocol it was built
against, and its size.  The loader refuses an image whose header does not
match.

The M33 starts it by holding it in reset, freezing its watchdog, clearing
the shared area and filling in the low-power clock frequency, copying the
image to RAM1, giving it a clock and releasing the reset, then waiting up
to 100 ms for it to report ``READY``.  A fault on the controller is
recorded with its PC and LR in the shared area and reported by the driver.

An application's ``main()`` calls ``snc_rt_init()`` and ``snc_rt_ready()``
and then loops on ``snc_rt_wait()``, which sleeps in WFI until the M33 rings
or its timer (TIMER6, clocked by the low-power clock) ticks.  TIMER6 is the
controller's; nothing on the M33 may use it.

Mailbox
=======

At the start of RAM8 sits ``struct snc_shared_s``
(``da1470x_snc_ipc.h``): the controller's state, a heartbeat, fault
information and two 2 KiB rings, one per direction.  A message is
``{type, len, payload}`` with up to 256 bytes of payload.  Each ring has a
single producer and a single consumer and each index is written by one side
only, so no lock is needed; a barrier orders the data before the index
that publishes it.

The doorbells are ``SYS2SNC`` and ``SNC2SYS`` in ``CRG_XTAL
SET_/RESET_SYS_IRQ_CTRL``.  The SNC rings the M33's ``SNC2SYS``
interrupt, which hands the ring to the low priority work queue; there each
message goes to the kernel handler registered for its type, or into a queue
for ``/dev/snc0`` (``CONFIG_DA1470X_SNC_RXQUEUE`` messages).  With the PDC
enabled ``SNC2SYS`` is also a wake-up source of the M33.

Nothing is dropped on the way: when the M33's queue is full the rest stays
in the ring, the controller sees its ring fill and holds back, and the
M33 rings ``SYS2SNC`` whenever it has made room.  Only a firmware that sends
events faster than they are read loses them, and counts the loss.

Message types ``0x0000``-``0x00ff`` belong to the framework (``PING``,
``PONG``, ``LOG``); ``0x0100`` and up to the application.

``/dev/snc0``
-------------

One message per ``write()`` and per ``read()``, as ``struct snc_msg_s``
trimmed to its payload; ``poll()`` reports ``POLLIN`` when one is waiting.
``SNCIOC_STATUS`` returns the state, heartbeat, fault PC and LR and the
drop counters; ``SNCIOC_STOP`` and ``SNCIOC_START`` stop and reload it.

The demo firmware
=================

``CONFIG_DA1470X_SNC_FW_DEMO`` answers ``PING`` with ``PONG``, sends a
``TICK`` every period it is given and reports the edges of a GPIO it is
told to watch, sampled every 10 ms.  Measured on the kit:

* 1000 round trips of 16 bytes: 0.70-1.28 ms each; of 256 bytes,
  1.77-2.20 ms.  All echoes correct.
* A flood of 3000 pings: every accepted one answered, the rest refused
  while the ring was full, none lost or corrupted.
* A tick every second while the M33 has nothing else to do: all ticks
  delivered on time, and the M33 in its sleep state for 16 of the 20
  seconds instead of none.

Power
=====

The controller's power domain, PD_SNC, also holds the serial interfaces
and the GPADC, the M33's console included, and is kept up.  Stopping the
controller holds it in reset without its clock but leaves the domain on.

Not done yet
============

* **Deep sleep of the controller.** It sleeps in WFI with PD_SNC up.
  Saving its state, setting ``CLK_SNC_CTRL_REG[SNC_STATE_RETAINED]`` and
  letting PD_SNC go down, with PDC entries to wake it, would let the whole
  part reach its extended sleep while the controller keeps time.
* **Its own clock.** It runs from the system clock branch; it measured
  around 10 MHz of effective instruction rate against the M33 on the bus.
* **Peripheral ownership.** Nothing stops the M33's drivers and a firmware
  from using the same peripheral.  A firmware that takes one (an I2C bus
  for a sensor, say) needs the board to leave it alone.

Measuring it
============

Attaching the debugger wakes the system and swamps the controller's own
current; a clean figure needs two builds compared with no probe attached.
