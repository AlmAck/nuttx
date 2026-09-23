=============================
Extended sleep (PD_SYS off)
=============================

What is left to build so that ``CONFIG_DA1470X_PM_EXTENDED_SLEEP`` can be
turned on.  Everything below the "Where it stands" section is unbuilt.

Where it stands
===============

The context save and restore exists (``da1470x_deepsleep.S``), the idle
loop uses it when PM_SLEEP has armed it, and the power domain controller
has wake-up entries for the system timer, the RTC alarm and button K1.
The part sleeps and wakes.  What does not work is the resume: every wake-up comes
back as a cold boot, so a board with the option on boot-loops once a
second.

Two reasons, both measured on the kit:

* ``RESET_STAT_REG`` (0x500000BC) reads ``0x06`` after a wake-up and after
  a reset-pin reset alike.  The bits are sticky and nothing clears them,
  so the vendor SDK's "zero means wake-up" test cannot discriminate here.
  It reads 0x06 because ``PMU_CTRL_REG[RESET_ON_WAKEUP]`` routes the
  wake-up through the boot ROM, and that is a reset.
That is the only fault.  An earlier reading of this said the power domain
controller also had a trigger permanently asserted, so the part woke the
instant it stopped.  Measurement says otherwise -- see "What the
registers actually say" below -- and the one-second cycle was simply
``CONFIG_DA1470X_TICKLESS_MAX_SLEEP_MS`` set to 1000 for bring-up doing
exactly what it was asked to, against a boot that takes most of a second.

Why RESET_ON_WAKEUP has to go
=============================

The datasheet is explicit: *"The booter is always executed when a POR, an
HW Reset, or the RESET_ON_WAKEUP feature is configured."*  So clearing it
is what stops the ROM running, and without the ROM there is no reset, so
``RESET_STAT_REG`` keeps the zero that ``goto_deepsleep()`` wrote and the
discrimination the SDK uses becomes valid again.

It also takes the wake-up latency from the ROM's milliseconds to the
56-74 µs the datasheet quotes for a plain wake-up, which for a watch
waking every second is the difference between a rounding error and a
duty cycle.

The cost is that nothing sets the system up for us any more.  The
processor comes back with PD_SYS freshly powered, so the OQSPI
controller is at its reset values and the flash is not memory-mapped.
The resume path therefore cannot be ordinary code in flash: it has to be
in RAM, and address 0 has to point at RAM for long enough to reach it.

The shape of the fix
====================

The image is linked to run at address 0, which is the OQSPI flash as the
boot ROM remapped it.  That does not have to change.  The remap only has
to point somewhere else for the short window in which the processor is
asleep and waking, and during that window nothing executes from flash
anyway::

    ... running from flash at address 0, REMAP_ADR0 = 2 (OQSPI)
     -> call a RAM-resident routine
        -> save context (already written)
        -> REMAP_ADR0 = 5 (SYSRAM3, i.e. address 0 == 0x20010000)
        -> WFI, PD_SYS goes away
        ~~~ wake ~~~
        -> processor fetches SP and PC from address 0, which is RAM
        -> RAM resume stub: reprogram the OQSPI controller,
           REMAP_ADR0 = 2, restore context
     -> returns into the flash-resident caller as if WFI had returned

Nothing needs relinking.  The one new requirement is that the first two
words of the remapped RAM region hold the initial stack pointer and the
address of the resume stub.

Steps
=====

1. Reserve the resume vector
----------------------------

``REMAP_ADR0 = 5`` maps address 0 to ``MEMORY_SYSRAM3_BASE``, which on
this part is ``0x20010000`` -- exactly the ``sram`` origin in
``boards/arm/da1470x/da14706-00hzdevkt-p/scripts/flash_app.ld``.  The
first bytes of that region currently belong to ``.data``.

Add a section ahead of ``.data`` holding two words: the stack pointer the
resume stub should start on, and the address of the stub.  On ARMv8-M the
reset fetch takes SP from address 0 and PC from address 4 and ignores
VTOR, so these two words are what the hardware reads on a wake-up.

Note the 64 KiB at ``0x20000000`` (RAM0-2) is reserved for the sensor
node controller and is *not* the region ``REMAP_ADR0 = 5`` selects;
``REMAP_ADR0 = 3`` selects that one.  Picking the wrong value points
address 0 at memory the linker knows nothing about.

2. Move the sleep and resume path into RAM
------------------------------------------

The tree already has the mechanism: ``.ramfunc`` is copied from flash by
``__start`` and the OQSPI driver's erase and program routines already run
from it while executing in place.  ``RAMFUNC`` is the attribute.

``goto_deepsleep`` must move there too, not just the resume half: the
instruction after its ``WFI`` executes with the flash not yet mapped.

The pads are the other thing the resume path owes the rest of the system.
The GPIO configuration registers are in PD_SYS and are lost, while the
pads themselves latch and hold the levels they had -- which is what keeps
the panel reset line and the touch supply steady across the sleep.  So
the resume has to reprogram the pin muxing and only then release the
latch through ``Px_RESET_PAD_LATCH``.  Skip the release and every pin
stays frozen: the system resumes perfectly and no I/O works.
``da1470x_gpio.c`` already has the latch helpers.

3. Snapshot and replay the OQSPI controller
-------------------------------------------

This is the part with no prior art in this port.  The controller is
configured by the boot ROM and the port only ever adjusts ``OQSPIF_DIV``;
nothing here knows how to program it from scratch, and the read timing is
per-device (see the warning about the flash's non-volatile dummy-byte
configuration in the board notes).

Do not try to understand the values.  Copy them: read the controller's
registers into retained RAM once at boot, while the ROM's configuration
is still in place, and write them back in the resume stub.  This is what
the vendor SDK calls ``hw_sys_reg_apply_config()``, and its comment says
the configuration "must be applied immediately after wake-up to reduce
current consumption".

The register set is in ``hardware/da1470x_oqspif.h``.  ``CTRLMODE``,
``BURSTCMDA``, ``BURSTCMDB``, ``BURSTBRK``, ``STATUSCMD`` and
``ERASECMDA/B/C`` are the ones that matter for memory-mapped reads; the
``CTR_*`` block is the on-the-fly decryption and is only needed if it is
ever used.  Snapshotting all of them is cheaper than deciding.

Restore order matters: the controller has to be back in memory-mapped
mode before ``REMAP_ADR0`` goes back to OQSPI, and the remap has to be
back before anything branches into flash.

4. Take the clock back
----------------------

A wake-up without the ROM leaves the system on RCHS.  The flash divider
that was set for the PLL is wrong for it, so the snapshot taken in step 3
should be of a configuration that is safe at RCHS -- take it at boot
before ``da1470x_clockconfig()`` speeds anything up -- and
``da1470x_pm_resume()`` then raises the clock in C once flash is
readable again.  It already calls ``pm_restore_clock()``, which compares
against what the hardware reports, so it copes with arriving on RCHS.

5. What the registers actually say
----------------------------------

Dumped on the running board over SWD, with the restored console build and
everything idle.  The PDC table holds five entries::

    0  0x08B3  PERIPHERAL  COMBO      CM33   EN_XTAL
    1  0x0987  PERIPHERAL  TIMER2     CM33   EN_XTAL | EN_TMR
    2  0x089B  PERIPHERAL  RTC_ALARM  CM33   EN_XTAL
    3  0x08D9  P1_GPIO     pin 22     CM33   EN_XTAL      <- button K1
    4  0x10A3  PERIPHERAL  MAC_TIMER  CMAC   EN_XTAL

and ``PDC_PENDING_CM33`` reads zero, repeatedly.  Nothing is asserted, so
the suspicion that a trigger was holding the part awake was wrong.

The wake-up block tells a more interesting story::

    WKUP_SEL_GPIO_P1  = 0x08     only P1.3
    WKUP_POL_P1       = 0x08     only P1.3
    WKUP_SELECT_P1    = 0x00
    P1_DATA bit 22    = 1        K1 idle high behind its pull-up

P1.3 is the touch controller's interrupt, and it is configured because
the touch driver arms a GPIO interrupt on it.  Bit 22 is clear: **K1 was
never routed into the wake-up block at all**, because nothing enables a
GPIO interrupt on the buttons.

So the two wake sources this board has are each broken, in opposite
ways:

* **K1** has a PDC entry and no wake-up block configuration, so the
  trigger can never fire.
* **Touch** has the wake-up block configured and no PDC entry, so the
  event never reaches the power domain controller.

Neither would wake the watch, and neither failure is visible until PD_SYS
is actually switched off -- with the domain powered, the touch interrupt
still arrives through the NVIC as usual and nobody notices the missing
half.

This is the argument for the next step in its strongest form: the entry
and the pin configuration are kept in two different places, by two
different pieces of code, and they have already drifted apart in both
directions.

6. Register the wake sources -- done
------------------------------------

Fixed, because the two halves are no longer kept apart:
``da1470x_gpioirq_enable()`` adds the controller entry and
``da1470x_gpioirq_disable()`` gives it back, so a pin that arms an
interrupt is a wake-up source by construction and its polarity is right
because the same call configured it.  Touch needed nothing else.  The
buttons are armed by the board at bring-up, so a press wakes the system
whether or not anything has opened ``/dev/gpio1``; K2 is now exposed as
``/dev/gpio2`` as well.

Read back on the kit::

    PDC     P1.22, P1.23, P1.03 alongside TIMER2, RTC_ALARM, COMBO, MAC
    WKUP_SEL_GPIO_P1 = 0x00C00008    bits 3, 22, 23
    WKUP_POL_P1      = 0x00C00008    falling, i.e. a press
    PDC_PENDING_CM33 = 0             nothing stuck on

Both buttons were then confirmed on the kit end to end: with the handler
printing which pin fired, K1 reported ``P1.22`` and K2 reported ``P1.23``
on their presses.  Worth knowing for anyone repeating it -- the
``gpio -w`` example waits only five seconds, which is too short to
coordinate with a person at the bench, so an instrumented handler and a
long window is the way to test this.

The delivery half is now answered for the state that works today.  With
the panel off and the board asleep at 3.6 mA, a press on K1 reached the
handler and took the system to 20.3 mA -- so the wake-up block latches the
event, the interrupt survives the low-power state and the press is not
lost.  The silicon guarantees this much: the datasheet says the port
interrupt lines "are kept asserted until acknowledged by SW", precisely so
that a processor whose domain was switched off can still take them.

What that leaves is a software question, and an audit answers it.  Four
places clear the wake-up status: the dispatcher clears what it delivers,
attach and enable clear a stale event before arming -- all correct -- and
``da1470x_gpioirq_initialize()`` wipes every port, which is right for a
cold boot and destructive on a resume, because the event it throws away
is the press that caused the wake-up.  The resume path itself touches
nothing in the block.

So the invariant to keep is one of ordering: **the resume check at the top
of __start() must stay ahead of every peripheral initialisation**, not
merely ahead of the data and bss setup.  Break that and button wake-up
fails in a way that looks like a dropped press rather than a start-up
ordering mistake.  Both sites now say so.

What is still unproven is the other half of a press: that the event
survives the resume and reaches the application.  The power domain
controller brings the system back, and the press itself is then an
ordinary GPIO interrupt latched in ``WKUP_STATUS_Px`` -- so the resume
path must not clear that status before the button driver reads it, or the
watch wakes and does nothing, which looks exactly like a dropped press.
That can only be tested once the resume works.

Two things to confirm with a register dump at the same time: whether the
per-pin PDC GPIO trigger needs ``WKUP_SELECT_Px`` as well as
``WKUP_SEL_GPIO_Px`` -- the driver zeroes the former and only uses the
latter, and the entries fire correctly while PD_SYS is powered, which
proves nothing about when it is not -- and whether the wake-up block's own
configuration survives PD_SYS going away or has to be restored next to the
pad latch.

Mechanical buttons also want ``WKUP_CTRL.WKUP_DEB_VALUE`` (0-63 ms) set,
which is still zero: undebounced, one press becomes a burst of wake-ups
and each one costs a full context restore.

Touch remains a product decision rather than a code one.  The ZT2628's
supply and its I2C pull-ups are on a rail the application switches
(``BOARD_TOUCH_PWR_PIN``, P0.28), so either the controller stays powered
in its own low-power mode and touch-to-wake costs its idle current, or the
rail goes away and the interrupt line floats.  Buttons and a wrist-raise
interrupt can stay armed cheaply; touch usually only while the screen is
on.

7. Decouple the wake-up marker from the reset status
----------------------------------------------------

Deferred deliberately, and belongs here rather than with the watchdog,
because it is only testable once the resume path runs.

The resume test is ``RESET_STAT_REG == 0``.  That works because nothing
clears the register and every real reset sets a bit in it, so zero is
reachable only through a sleep.  The cost is that the bits accumulate:
on a board that has been used it reads 0x7F, and
``da1470x_wdt_reset_cause()`` can say only "all of these happened at some
point", which is no use for asking why a watch rebooted in the field.

The two are one problem.  What makes the diagnostics useless is what
makes the marker safe, so clearing the register for diagnostics would
arm a false resume -- a restart that reaches ``__start`` with the
register zero without having slept, restoring a context that is not
there.  A debugger forcing the program counter to the reset vector is the
one path that can do this today.

The fix is to stop overloading the register.  Require both a marker of
our own and a zero reset status::

    resume  iff  g_da1470x_resume_magic == MAGIC  &&  RESET_STAT == 0

``goto_deepsleep()`` writes the magic next to the zero it already writes;
``__start()`` tests both and clears the magic on every other path.  The
magic wants to live beside the retained state block in
``da1470x_deepsleep.S``, which is already ``.bss``: a cold boot clears it
through the normal ``.bss`` initialisation, and a resume branches away
before that happens, so the lifetime is right for free and no linker
change is needed.

It biases the right way.  Failing the magic gives a cold boot, which
loses the session and is clean; failing the other way would restore a
context that does not exist.  And once it is in,
``da1470x_wdt_clear_reset_cause()`` can exist as a plain call, with no
guard and no knowledge of the sleep path.

8. Retention
------------

``RAM_PWR_CTRL`` (0x500000C0) is 0 out of reset, which retains every
bank, so ``.data`` and ``.bss`` survive and no linker work is needed.
That is also why the datasheet's 19.7 µA figure -- quoted for 256 kB
retained -- is a floor rather than a promise: this port retains 1 MiB.
Trimming retention to the banks actually in use is a later refinement,
and needs the heap and stacks placed deliberately.

How to test without bricking the board
======================================

A board that sleeps and never wakes looks identical to a board that
crashed.  Recovery is always the reset pin (J-Link ``rsettype 2`` then
``r``), so nothing is unrecoverable --
but each round trip costs a flash cycle.

* Set ``CONFIG_DA1470X_TICKLESS_MAX_SLEEP_MS`` to 1000 for bring-up.  The
  system then wakes once a second on its own, so a board that resumes
  correctly keeps its console and a board that does not is obvious
  immediately.
* Put a GPIO high at the top of the resume stub and low once it reaches
  C.  On the logic analyser, or as a current step on the PPK2, that
  separates "never woke" from "woke and died in the stub".
* Measure with ``tools/da1470x_power``.  ``sweep.py`` drives the console
  and the PPK2 together, which is necessary because the board is
  unpowered between measurement sessions.
* K1 is the honest end-to-end test once the timer wake works: let the
  board settle into sleep with the maximum sleep time set long, press the
  button, and watch for both the current step and the button arriving at
  the application.  A wake that does not deliver the press is only half
  working.

What success looks like
=======================

The board draws 1.45 mA with the SoC held in reset, so that is the floor
the kit itself imposes.  The datasheet puts extended sleep at 19.7 µA,
against roughly 5.6 mA for the SoC today.  A working resume should show
the panel-off figure fall from about 9 mA to something near the 1.5 mA
board baseline, with the difference between the two the only thing left
to argue about.
