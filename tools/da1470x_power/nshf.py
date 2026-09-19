#!/usr/bin/env python3
"""Talk to the NSH console over the kit's FT2232H without the kernel driver.

The devkit's hub re-enumerates often enough that ftdi_sio loses its binding
and /dev/ttyUSB0 disappears; rebinding it needs root.  libusb does not, so
this drives channel A of the FT2232H (UART0, the console) directly.

  nshf.py [--wait=S] "cmd" ["cmd" ...]
"""

import sys
import time

from pyftdi.serialext import serial_for_url

URL = "ftdi://ftdi:2232:1:4e/1"


def main():
    args = sys.argv[1:]
    wait = 3.0
    if args and args[0].startswith("--wait="):
        wait = float(args[0].split("=")[1])
        args = args[1:]

    port = serial_for_url(URL, baudrate=115200, timeout=0.2)
    port.reset_input_buffer()

    out = []
    port.write(b"\r\n")
    time.sleep(0.3)
    port.read(4096)

    for cmd in args:
        port.write(cmd.encode() + b"\r\n")
        deadline = time.time() + wait
        while time.time() < deadline:
            data = port.read(4096)
            if data:
                out.append(data.decode("utf-8", "replace"))
            else:
                time.sleep(0.02)

    port.close()
    sys.stdout.write("".join(out))


if __name__ == "__main__":
    main()
