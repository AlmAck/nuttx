# Measuring what the DA1470x devkit draws

A Nordic PPK2 in series with the board, driven from the host, so that a
change in the firmware can be turned into a number without anyone reading
a meter.

## Wiring

The PRO-Motherboard feeds the daughterboard through header **J9**:

    USB -> DCDC U28 -> VLDOp -> [2.4 ohm sense, on PMM2] -> VLDOn -> J9 -> VBLDO -> DA1470x

UM-B-148 Table 6 says an external ammeter goes on those jumpers.  Use the
**J9:2-4** pair, which bypasses the PMM2 sense resistor -- going through it
would drop 240 mV at 100 mA and brown the display out.

1. Unplug the daughterboard USB; leave the motherboard USB in (it feeds
   the hub, both debuggers and the console, none of which is measured).
2. Remove every J9 jumper.
3. PPK2 in **ampere meter** mode: VIN to the motherboard-side pin, VOUT to
   the daughterboard-side pin, GND to board ground.

To tell the pins apart without trusting the numbering: with the jumpers
off and the board powered, the pin still at ~3.3 V is VIN, the one at 0 V
is VOUT.  Backwards just reads negative.

For the lowest-noise sleep figures the motherboard's **SW1** isolates the
UART and SWD signals -- but that takes the console with it, so it is for a
final measurement only.

## Two things that will confuse you

* The PPK2 only passes current while a session holds its DUT switch on.
  The board is **dead between script runs**: anything that needs the board
  alive (flashing, SWD) has to happen with `ppkhold.py` running.
* The kit's hub re-enumerates often enough that `ftdi_sio` loses its
  binding and `/dev/ttyUSB0` disappears; rebinding needs root.  `nshf.py`
  sidesteps it by driving the FT2232H over libusb instead.

## Scripts

    ppkhold.py [seconds]      hold the pass-through on, e.g. while flashing
    ppkmeas.py --seconds N    one measurement of whatever state the board is in
    sweep.py   "label:cmd:secs" ...
                              power the board, wait for boot, then walk it
                              through states over the console, reporting the
                              current drawn in each
    nshf.py    "cmd" ...      talk to NSH over libusb

Needs `ppk2-api` and `pyftdi`; a virtualenv is enough.

    python3 -m venv venv && venv/bin/pip install ppk2-api pyftdi

## A reference point worth keeping

With the SoC **held in reset** the board still draws **1.45 mA** -- the LCD
add-on in standby, the level shifters and the pull-ups.  Subtract it before
concluding anything about the DA1470x itself.
