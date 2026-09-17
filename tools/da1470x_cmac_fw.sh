#!/usr/bin/env bash
############################################################################
# tools/da1470x_cmac_fw.sh
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
# Extract the CMAC (BLE controller) firmware image from the Renesas
# SmartSnippets SDK library libble_stack_da1470x.a.  The image lives in the
# .cmi_fw_area section of cmac_cpu_lld.c.obj: a 36-byte header followed by
# the code+data image that is copied to RAM10 by the host.
#
# Usage: da1470x_cmac_fw.sh <path/to/libble_stack_da1470x.a> <output.bin>

set -e

if [ $# -ne 2 ]; then
  echo "Usage: $0 <libble_stack_da1470x.a> <output.bin>" >&2
  exit 1
fi

archive=$1
output=$2
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

CROSS=${CROSSDEV:-arm-none-eabi-}

(cd "$tmp" && ${CROSS}ar x "$archive" cmac_cpu_lld.c.obj)
${CROSS}objcopy -O binary --only-section=.cmi_fw_area \
  "$tmp/cmac_cpu_lld.c.obj" "$tmp/area.bin"

# The whole area (header included) is what the driver expects: it reads the
# image size from the header itself.

cp "$tmp/area.bin" "$output"
echo "$(stat -c %s "$output") bytes written to $output"
