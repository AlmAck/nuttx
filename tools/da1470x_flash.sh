#!/usr/bin/env bash
############################################################################
# tools/da1470x_flash.sh
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
############################################################################
#
# Program a DA1470x boot image with the Renesas ezFlashCLI tool.  The tool
# places the image at flash offset 0x3400 but only programs the first
# 512 KiB of the flash, so the part of the image beyond that boundary is
# written separately.
#
# The flash cache keeps lines from before the programming and the boot ROM
# does not flush them when it re-sizes the cacheable window for the new
# image, so the first fetch of the new code can return stale bytes.  A reset
# request through the debugger also leaves the clock tree as the previous
# firmware left it, and the boot ROM hangs when that was the 160 MHz PLL.
# After programming, the board is therefore reset under the debugger and,
# while the core is still halted at the reset vector, the system clock is
# put back on the internal RC oscillator with the PLL off and the cache RAM
# mux is toggled (which flushes the cache) before the ROM runs.
#
# Usage: da1470x_flash.sh <jlink serial> <nuttx.bin>

set -e

if [ $# -ne 2 ]; then
  echo "Usage: $0 <jlink serial> <nuttx.bin>" >&2
  exit 1
fi

serial=$1
image=$2
imgbase=$((0x3400))
limit=$((0x80000))
maxfirst=$((limit - imgbase))
size=$(stat -c %s "$image")
sysctrl_reg=0x50000024
cacheram_mux=0x400

ezFlashCLI -j "$serial" image_flash "$image"

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

if [ "$size" -gt "$maxfirst" ]; then
  dd if="$image" of="$tmp/tail.bin" bs=4096 skip=$maxfirst iflag=skip_bytes \
     status=none
  echo "Image exceeds 512 KiB: writing $((size - maxfirst)) bytes at 0x80000"
  ezFlashCLI -j "$serial" write_flash $limit "$tmp/tail.bin"
fi

jlink() {
  JLinkExe -SelectEmuBySN "$serial" -device Cortex-M33 -if SWD -speed 4000 \
           -autoconnect 1 -NoGui 1 -CommanderScript "$1"
}

# Registers restored while halted at the reset vector

clk_ctrl_reg=0x50000014
clk_ctrl_rchs=0x1
clk_amba_reg=0x50000000
clk_amba_default=0x1040
pll_ctrl_reg=0x50050460
pll_ctrl_off=0xE8A0

{
  echo "r"
  echo "mem32 $sysctrl_reg 1"
  echo "qc"
} > "$tmp/read.jlink"

sysctrl=$(jlink "$tmp/read.jlink" | sed -n 's/^50000024 = \([0-9A-F]*\).*/\1/p')

if [ -z "$sysctrl" ]; then
  echo "Could not halt the core at reset; power-cycle the board" >&2
  exit 1
fi

clr=$(printf '0x%08X' $(( 0x$sysctrl & ~cacheram_mux )))
set=$(printf '0x%08X' $(( 0x$sysctrl | cacheram_mux )))

{
  echo "r"
  echo "w4 $clk_ctrl_reg $clk_ctrl_rchs"
  echo "w4 $clk_amba_reg $clk_amba_default"
  echo "w4 $pll_ctrl_reg $pll_ctrl_off"
  echo "w4 $sysctrl_reg $clr"
  echo "w4 $sysctrl_reg $set"
  echo "g"
  echo "qc"
} > "$tmp/flush.jlink"

jlink "$tmp/flush.jlink" > /dev/null
echo "Clocks restored and cache flushed at reset, running"
