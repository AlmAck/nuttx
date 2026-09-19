#!/usr/bin/env python3
"""Measure the DA1470x devkit current with a Nordic PPK2.

The PPK2 sits in series on the PRO-Motherboard's J9 header, in place of the
jumper that would feed VBLDO to the daughterboard, so what it reads is the
whole DA1470x system current: core, radio, flash and the display add-on,
which is supplied from VSYS.

  ppkmeas.py [--seconds N] [--mode ampere|source] [--voltage mV] [--raw FILE]

Prints mean / min / max / median in microamps.  With --raw the individual
samples are written one per line so a profile can be plotted later.
"""

import argparse
import statistics
import sys
import time

from ppk2_api.ppk2_api import PPK2_API


def open_ppk2():
    """The PPK2 enumerates two CDC ports and only one answers; try both."""
    ports = PPK2_API.list_devices()
    if not ports:
        sys.exit("No PPK2 found on USB.")

    for entry in sorted(ports):
        port = entry[0] if isinstance(entry, (list, tuple)) else entry
        try:
            ppk = PPK2_API(port, timeout=1, write_timeout=1, exclusive=True)
        except Exception:
            continue

        # A session that was interrupted leaves the device still streaming
        # samples, and the modifier read then trips over them.  Tell it to
        # stop, let the backlog drain, and only then ask.
        for _ in range(4):
            try:
                ppk.stop_measuring()
            except Exception:
                pass

            time.sleep(0.3)

            try:
                while ppk.ser.in_waiting:
                    ppk.ser.read(ppk.ser.in_waiting)
                    time.sleep(0.05)
                ppk.ser.reset_input_buffer()
                ppk.ser.reset_output_buffer()
            except Exception:
                pass

            try:
                if ppk.get_modifiers():
                    return ppk
            except Exception:
                pass

    sys.exit("Found a PPK2 but no port answered; is another tool holding it?")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--seconds", type=float, default=5.0)
    ap.add_argument("--mode", choices=("ampere", "source"), default="ampere")
    ap.add_argument("--voltage", type=int, default=3300,
                    help="source meter output in mV (source mode only)")
    ap.add_argument("--raw", help="write every sample to this file")
    ap.add_argument("--label", default="")
    args = ap.parse_args()

    ppk = open_ppk2()

    # The device wants to know the rail voltage in both modes: in source
    # meter mode it is what it puts out, in ampere meter mode it is what the
    # board's own supply is expected to be.

    ppk.set_source_voltage(args.voltage)

    if args.mode == "source":
        ppk.use_source_meter()
    else:
        ppk.use_ampere_meter()

    ppk.toggle_DUT_power("ON")
    ppk.start_measuring()

    samples = []
    deadline = time.time() + args.seconds
    # Discard the first moments: the ranging amplifier needs to settle.
    settle = time.time() + 0.3
    while time.time() < deadline:
        data = ppk.get_data()
        if data:
            chunk, _ = ppk.get_samples(data)
            if time.time() > settle:
                samples.extend(chunk)
        time.sleep(0.01)

    ppk.stop_measuring()
    if args.mode == "source":
        ppk.toggle_DUT_power("OFF")

    if not samples:
        sys.exit("No samples captured -- is the PPK2 wired in and powered?")

    if args.raw:
        with open(args.raw, "w") as fh:
            fh.writelines("%.3f\n" % s for s in samples)

    label = (args.label + ": ") if args.label else ""
    print("%s%d samples over %.1fs" % (label, len(samples), args.seconds))
    print("  mean   %10.1f uA" % statistics.mean(samples))
    print("  median %10.1f uA" % statistics.median(samples))
    print("  min    %10.1f uA" % min(samples))
    print("  max    %10.1f uA" % max(samples))


if __name__ == "__main__":
    main()
