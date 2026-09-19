#!/usr/bin/env python3
"""Drive the DA1470x through its power states and measure each one.

The PPK2 only passes current to the board while a measurement session is
open, so the board has to be talked to from inside that session: this opens
the PPK2, powers the board, waits for it to boot, then walks it through a
list of phases over the console, timestamping the boundaries and reporting
the current drawn in each.

  sweep.py [--voltage mV] [--boot S] "label:command:seconds" ...

A phase whose command is empty just measures for the given time.
"""

import argparse
import statistics
import sys
import threading
import time

from ppk2_api.ppk2_api import PPK2_API
from pyftdi.serialext import serial_for_url

FTDI_URL = "ftdi://ftdi:2232:1:4e/1"


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


class Sampler(threading.Thread):
    """Drain the PPK2 continuously and keep (timestamp, current) pairs."""

    def __init__(self, ppk):
        super().__init__(daemon=True)
        self.ppk = ppk
        self.samples = []
        self.running = True

    def run(self):
        while self.running:
            data = self.ppk.get_data()
            if data:
                chunk, _ = self.ppk.get_samples(data)
                now = time.time()
                self.samples.extend((now, s) for s in chunk)
            else:
                time.sleep(0.005)

    def between(self, t0, t1):
        return [s for (t, s) in self.samples if t0 <= t <= t1]


def report(label, values):
    if not values:
        print("%-28s no samples" % label)
        return
    print("%-28s mean %8.0f uA   median %8.0f uA   min %8.0f   max %8.0f   n=%d"
          % (label, statistics.mean(values), statistics.median(values),
             min(values), max(values), len(values)))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--voltage", type=int, default=3300)
    ap.add_argument("--boot", type=float, default=6.0,
                    help="seconds to let the board boot before phase one")
    ap.add_argument("--dump", help="write every timestamped sample here")
    ap.add_argument("phases", nargs="+",
                    help="label:command:seconds")
    args = ap.parse_args()

    ppk = open_ppk2()
    ppk.set_source_voltage(args.voltage)
    ppk.use_ampere_meter()
    ppk.toggle_DUT_power("ON")
    ppk.start_measuring()

    sampler = Sampler(ppk)
    sampler.start()

    try:
        print("Powering the board, waiting %.1fs for it to boot..." % args.boot)
        time.sleep(args.boot)

        console = serial_for_url(FTDI_URL, baudrate=115200, timeout=0.2)
        console.reset_input_buffer()
        console.write(b"\r\n")
        time.sleep(0.5)
        banner = console.read(4096)
        print("console: %s" % (banner.decode("utf-8", "replace")
                               .replace("\r\n", " | ").strip() or "(silent)"))

        results = []
        for spec in args.phases:
            label, cmd, secs = spec.split(":", 2)
            secs = float(secs)

            if cmd:
                console.write(cmd.encode() + b"\r\n")
                time.sleep(1.0)          # let the command take effect
                console.read(8192)

            t0 = time.time()
            time.sleep(secs)
            t1 = time.time()
            results.append((label, sampler.between(t0, t1)))

        console.write(b"cat /proc/pm/state\r\n")
        time.sleep(1.5)
        tail = console.read(8192).decode("utf-8", "replace")
        console.close()
    finally:
        sampler.running = False
        time.sleep(0.2)
        try:
            ppk.stop_measuring()
        except Exception:
            pass

    if args.dump:
        with open(args.dump, "w") as fh:
            fh.writelines("%.6f,%.3f\n" % (t, s) for (t, s) in sampler.samples)
        print("raw samples -> %s" % args.dump)

    print()
    for label, values in results:
        report(label, values)
    print()
    print(tail.replace("\r\n", "\n"))


if __name__ == "__main__":
    main()
