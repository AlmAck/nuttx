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
# The programmer leaves the flash controller out of its memory-mapped mode
# and a plain reset request would start the image with the flash
# unreadable; a reset request also keeps the clock tree of the previous
# firmware, which the boot ROM cannot start from when that was the PLL.
# So the board is reset through the reset pin under the debugger, which
# resets the whole chip and lets the boot ROM set the flash up again.
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

{
  echo "rsettype 2"
  echo "r"
  echo "g"
  echo "qc"
} > "$tmp/reset.jlink"

jlink "$tmp/reset.jlink" > /dev/null
echo "Reset through the reset pin, running"
