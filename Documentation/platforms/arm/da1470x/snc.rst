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

The runtime answers ``PING`` with ``PONG`` for every firmware.
``CONFIG_DA1470X_SNC_FW_DEMO`` also sends a
``TICK`` every period it is given and reports the edges of a GPIO it is
told to watch, sampled every 10 ms, and faults on purpose on ``FAULT``.
Measured on the kit:

* 1000 round trips of 16 bytes: 0.70-1.28 ms each; of 256 bytes,
  1.77-2.20 ms.  All echoes correct.
* A flood of 3000 pings: every accepted one answered, the rest refused
  while the ring was full, none lost or corrupted.
* A tick every second while the M33 has nothing else to do: all ticks
  delivered on time, and the M33 in its sleep state for 16 of the 20
  seconds instead of none.

The input firmware
==================

``CONFIG_DA1470X_SNC_FW_INPUT`` watches the pins the M33 configures with
``SNC_INPUT_CONFIG`` (a port, a pin mask, a debounce time), samples them
every 5 ms from TIMER6 while anything is watched, and sends
``SNC_INPUT_EVENT`` -- the settled levels and which changed -- only once
a change has held for the debounce time, 20 ms by default.  It also
reports the levels when a port is first configured.

On the devkit the board uses it for K1 and K2: ``board_button_irq()``
has the controller watch both pins and calls the button upper half's
handler for every settled press and release, from the low priority work
queue.  ``/dev/buttons`` then wakes a reader that sleeps in ``poll()``,
without the M33 polling the buttons or taking an interrupt per bounce.
Without the input firmware the board arms no button interrupt (see
``da1470x_buttons.c`` for why).

Checked on the kit with LED1 (P0.31), which the M33 can drive: every
transition arrived as exactly one event, and a write that did not change
the pin produced none.

Power and deep sleep
====================

The controller's power domain, PD_SNC, also holds the serial interfaces
and the GPADC, the M33's console included.  Stopping the controller
holds it in reset without its clock but leaves the domain on.

``SNC_MSG_SLEEP_POLICY`` lets the controller sleep with ``SLEEPDEEP``.
Before it does, the runtime saves the callee-saved registers and the
NVIC, acknowledges its PDC entries and sets
``CLK_SNC_CTRL_REG[SNC_STATE_RETAINED]``.  While the M33 keeps PD_SNC up
this is an ordinary sleep.  When the domain does go down, the next PDC
wake-up -- TIMER6 or the M33's doorbell, both entries with the controller
as master -- powers it and starts the controller from reset;
``snc_reset()`` sees the retained flag and resumes where it slept, on a
stack of its own.  RAM1, RAM2 and RAM8 are in PD_MEM and keep their
contents.

``SNCIOC_PDTEST`` exercises this: it lets the controller deep-sleep,
switches PD_SNC off for a while and counts what happened.  With the input
firmware watching a pin, 2 s with PD_SNC off gave 401 wake-ups, every one
a resume from a power loss, and the watched pin's configuration and event
count were intact afterwards.  It is a test: the M33's own PD_SNC
peripherals are unpowered meanwhile and only the console UART is set up
again afterwards.

In normal operation the M33 keeps PD_SNC up.  Letting it go down while
the M33 sleeps needs the M33's drivers in that domain -- the console
UART, I2C, SPI, DMA -- to be restored on the way back, which is part of
the work towards the part's extended sleep.

Using it well
=============

The controller is a gatekeeper: it does the frequent, small work that
would otherwise wake the M33, and wakes the M33 only when there is a
result worth acting on.  Each wake of the M33 costs about 39 uC, so the
aim is to turn "the M33 wakes 25-100 times a second" into "a few times a
minute".  A rule of thumb: anything that would wake the M33 more than
about once a second belongs here.

What fits
---------

* High-rate sensor sampling (an accelerometer at 25-100 Hz), batched.
* Event detection on a stream -- steps, wrist-raise, tap, "no motion for
  five minutes", heart-rate peaks -- sending the conclusion, not the
  samples.
* Pin watching: buttons, a charger's status pins (the input firmware).
* Slow housekeeping with thresholds: battery, temperature, pressure
  trends, reported only when a threshold is crossed.

What does not: the UI and the display, Bluetooth, the file system and
flash writes, heavy arithmetic (a Cortex-M0+ without FPU, around 10 MHz
of effective instruction rate, 64 KiB of RAM), and anything that happens
only occasionally -- once a minute on the M33 costs about as little and
is simpler.

Where an M33 driver is already interrupt driven and wakes only on a real
event, moving its interrupt line here gains nothing.  The devkit's touch
controller is such a case: its INT line raises an M33 interrupt only
while a finger is on the glass, and ``poll()`` on ``/dev/input0`` sleeps
until then.  The controller helps with touch only by taking the whole
job -- reading the touch controller over a bus of its own and
recognising gestures -- so that the M33 wakes once per gesture rather
than once per report while a finger moves.

How to build on it
------------------

1. **One firmware, several small jobs.**  Only one firmware runs, so the
   product firmware is one application combining jobs (input watching, a
   sensor hub, battery monitoring), each switched on by a configuration
   message.  Every application has the loop of the demo and input
   firmwares: ``snc_rt_wait()``, take commands while a reply is sure to
   fit, pass the framework's own to ``snc_rt_system_message()``, run the
   jobs whose timer ticked.

2. **Configure from the M33.**  Which pins, what rate, what threshold
   arrive as messages such as ``SNC_INPUT_CONFIG``: the M33 keeps the
   policy, one firmware serves every use, and with nothing configured the
   timer stops and the controller sleeps.

3. **Send conclusions, or batches.**  One message per event; where the
   M33 really needs samples, a batch of them in one message (up to 256
   bytes) a few times a second.  RAM8 has room for larger blocks handed
   over by offset.

4. **Make it look like ordinary devices on the M33.**  A small kernel
   driver registers the usual device -- a uORB topic such as
   ``/dev/uorb/sensor_accel0``, or ``/dev/buttons`` as the board already
   does -- and turns its handler (``da1470x_snc_register_handler()``) and
   its controls (activate, interval) into messages.  Nothing above the
   kernel needs to know the controller exists; ``/dev/snc0`` is for tests
   and tools.

5. **Give it peripherals of its own.**  The M33's drivers and a firmware
   must not share a peripheral; nothing arbitrates between them.  Put the
   controller's devices on a bus the M33 does not register (I2C1 or SPI1,
   say).  TIMER6 is reserved this way already.

Power
-----

* The timer runs only while a job needs it.
* Sample no faster than the job needs, and prefer a sensor's own FIFO
  and interrupt pin: one burst read per batch instead of one wake per
  sample.
* Enable deep sleep (``SNC_MSG_SLEEP_POLICY``) once the part as a whole
  goes into its deepest sleep; the controller then survives PD_SNC going
  down.
* Measure with the debugger detached, and with a current meter whose
  limit allows the panel's inrush when it lights.

Not done yet
============

* **PD_SNC down in normal operation**, as above.
* **Its own clock.** It runs from the system clock branch; it measured
  around 10 MHz of effective instruction rate against the M33 on the bus.
* **Peripheral ownership.** Nothing stops the M33's drivers and a firmware
  from using the same peripheral.  A firmware that takes one (an I2C bus
  for a sensor, say) needs the board to leave it alone.

Measuring it
============

Attaching the debugger wakes the system and swamps the controller's own
current; a clean figure needs two builds compared with no probe attached.
