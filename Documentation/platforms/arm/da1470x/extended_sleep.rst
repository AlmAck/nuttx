=============================
Extended sleep (PD_SYS off)
=============================

What is left to build so that ``CONFIG_DA1470X_PM_EXTENDED_SLEEP`` can be
turned on.  Everything below the "Where it stands" section is unbuilt.

Where it stands
===============

The context save and restore exists (``da1470x_deepsleep.S``), the idle
loop uses it when PM_SLEEP has armed it, and the power domain controller
has wake-up entries for the system timer and optionally a pin.  The part
sleeps and wakes.  What does not work is the resume: every wake-up comes
back as a cold boot, so a board with the option on boot-loops once a
second.

Two reasons, both measured on the kit:

* ``RESET_STAT_REG`` (0x500000BC) reads ``0x06`` after a wake-up and after
  a reset-pin reset alike.  The bits are sticky and nothing clears them,
  so the vendor SDK's "zero means wake-up" test cannot discriminate here.
  It reads 0x06 because ``PMU_CTRL_REG[RESET_ON_WAKEUP]`` routes the
  wake-up through the boot ROM, and that is a reset.
* The power domain controller already has a trigger pending when the
  system stops, so it wakes again immediately.  The entry registered in
  ``arm_pminitialize()`` for ``DA1470X_PDC_PERIPH_COMBO`` (VBUS, JTAG,
  CMAC2SYS) is the first suspect and has not been eliminated.

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

5. Find the trigger that wakes it immediately
---------------------------------------------

Independent of the above and worth doing first, because it is cheap and
it is currently masking everything else: read ``PDC_PENDING_CM33`` just
before the ``WFI`` and see which entry is set.  The ``COMBO`` entry
registered "to keep the debugger able to wake the part" covers VBUS,
JTAG and CMAC2SYS, and VBUS is present on the bench.  If that is it, the
entry should not be registered when extended sleep is enabled, or the
debug port should be closed before it is armed.

6. Retention
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
crashed.  Recovery is always the reset pin, which
``tools/da1470x_flash.sh`` already uses, so nothing is unrecoverable --
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

What success looks like
=======================

The board draws 1.45 mA with the SoC held in reset, so that is the floor
the kit itself imposes.  The datasheet puts extended sleep at 19.7 µA,
against roughly 5.6 mA for the SoC today.  A working resume should show
the panel-off figure fall from about 9 mA to something near the 1.5 mA
board baseline, with the difference between the two the only thing left
to argue about.
