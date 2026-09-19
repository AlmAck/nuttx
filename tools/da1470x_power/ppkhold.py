#!/usr/bin/env python3
"""Hold the PPK2's pass-through on so the board stays powered.

In ampere meter mode the PPK2 only lets current through while a session has
the DUT switch on, and it drops again when the process lets go.  Flashing
and debugging therefore need someone holding the switch: run this in the
background for the duration.
"""

import sys
import time

from ppk2_api.ppk2_api import PPK2_API


def open_ppk2():
    ports = PPK2_API.list_devices()
    if not ports:
        sys.exit("No PPK2 found on USB.")

    for entry in sorted(ports):
        port = entry[0] if isinstance(entry, (list, tuple)) else entry
        try:
            ppk = PPK2_API(port, timeout=1, write_timeout=1, exclusive=True)
        except Exception:
            continue

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
            except Exception:
                pass
            try:
                if ppk.get_modifiers():
                    return ppk
            except Exception:
                pass

    sys.exit("Found a PPK2 but no port answered.")


def main():
    seconds = float(sys.argv[1]) if len(sys.argv) > 1 else 600.0

    ppk = open_ppk2()
    ppk.set_source_voltage(3300)
    ppk.use_ampere_meter()
    ppk.toggle_DUT_power("ON")

    # Measuring is what keeps the pass-through alive; the samples are of no
    # interest here, so drain and drop them.
    ppk.start_measuring()
    print("DUT powered, holding for %.0fs" % seconds, flush=True)

    end = time.time() + seconds
    while time.time() < end:
        ppk.get_data()
        time.sleep(0.05)

    ppk.stop_measuring()
    print("released", flush=True)


if __name__ == "__main__":
    main()
