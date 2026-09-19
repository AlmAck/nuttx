#!/usr/bin/env bash
############################################################################
# tools/da1470x_snc_fw.sh
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
# Assemble firmware for the sensor node controller and print it as the C
# array da1470x_snc.c carries.
#
# The controller is a Cortex-M0+ and the rest of this tree is built for a
# Cortex-M33, so its code cannot come out of the same compiler invocation:
# it is assembled here, by hand, and the result is pasted in.  The same is
# true of the radio controller's firmware, which arrives as a blob for the
# same reason.
#
# The image is linked against the controller's own addresses, where RAM1
# ("SNC code") is at 0 and RAM2 ("SNC data") at 0x8000.
#
# Usage: da1470x_snc_fw.sh <source.S>

set -e

if [ $# -ne 1 ]; then
  echo "Usage: $0 <source.S>" >&2
  exit 1
fi

src=$1
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

arm-none-eabi-gcc -c -mcpu=cortex-m0plus -mthumb -o "$tmp/fw.o" "$src"

cat > "$tmp/fw.ld" <<'LDS'
SECTIONS
{
  . = 0;
  .vectors : { *(.vectors) }
  .text    : { *(.text) }
}
LDS

arm-none-eabi-ld -T "$tmp/fw.ld" -o "$tmp/fw.elf" "$tmp/fw.o"
arm-none-eabi-objcopy -O binary "$tmp/fw.elf" "$tmp/fw.bin"

size=$(stat -c %s "$tmp/fw.bin")
echo "/* $size bytes from $(basename "$src") */"
od -An -tx4 -v "$tmp/fw.bin" |
  awk '{ for (i = 1; i <= NF; i++) printf "0x%s, ", $i; print "" }' |
  sed -e 's/^/  /' -e 's/, $//'
