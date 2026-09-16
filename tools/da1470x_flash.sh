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

if [ "$size" -gt "$maxfirst" ]; then
  tail=$(mktemp)
  trap 'rm -f "$tail"' EXIT
  dd if="$image" of="$tail" bs=4096 skip=$maxfirst iflag=skip_bytes status=none
  echo "Image exceeds 512 KiB: writing $((size - maxfirst)) bytes at 0x80000"
  ezFlashCLI -j "$serial" write_flash $limit "$tail"
fi
