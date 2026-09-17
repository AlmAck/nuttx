#!/usr/bin/env bash
############################################################################
# tools/da1470x_console.sh
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
#
# NuttShell console of a DA1470x board over SEGGER RTT, for configurations
# built with CONFIG_SERIAL_RTT_CONSOLE (no UART needed).  Starts a J-Link
# session that serves RTT channel 0 on a local telnet port, then attaches
# a terminal to it.  The running firmware is not reset.
#
# Usage: da1470x_console.sh <jlink serial> [nuttx ELF] [command ...]
#
#   With no command the terminal is interactive (leave with Ctrl-C).
#   With commands, each is sent to the shell, the output printed, and the
#   script exits:  da1470x_console.sh 900010639 nuttx "free" "gpu2d"
#
# The RTT control block address is taken from the ELF, which therefore has
# to be the image that is running on the board.

set -e

if [ $# -lt 1 ]; then
  echo "Usage: $0 <jlink serial> [nuttx ELF] [command ...]" >&2
  exit 1
fi

serial=$1
elf=${2:-nuttx}
shift
[ $# -gt 0 ] && shift
port=${DA1470X_RTT_PORT:-19021}

addr=$(arm-none-eabi-nm "$elf" | awk '/ _SEGGER_RTT$/ {print "0x" $1}')
if [ -z "$addr" ]; then
  echo "$elf has no _SEGGER_RTT symbol: not an RTT console build" >&2
  exit 1
fi

tmp=$(mktemp -d)
mkfifo "$tmp/cmd"

# Hold the command pipe open ourselves so J-Link Commander stays alive

exec 3<>"$tmp/cmd"

JLinkExe -SelectEmuBySN "$serial" -device Cortex-M33 -if SWD -speed 4000 \
         -autoconnect 1 -NoGui 1 -RTTTelnetPort "$port" \
         <&3 > "$tmp/jlink.log" 2>&1 &
jlink=$!

cleanup() {
  echo "qc" >&3 2>/dev/null || true
  sleep 0.3
  kill "$jlink" 2>/dev/null || true
  exec 3>&-
  rm -rf "$tmp"
}
trap cleanup EXIT

echo "exec SetRTTAddr $addr" >&3

for _ in $(seq 1 50); do
  if (exec 4<>"/dev/tcp/127.0.0.1/$port") 2>/dev/null; then
    break
  fi
  if ! kill -0 "$jlink" 2>/dev/null; then
    echo "J-Link session ended:" >&2
    tail -5 "$tmp/jlink.log" >&2
    exit 1
  fi
  sleep 0.2
done

if [ $# -eq 0 ]; then
  echo "RTT console on J-Link $serial (control block $addr), Ctrl-C to leave"
  JLinkRTTClient -RTTTelnetPort "$port"
  exit 0
fi

# Scripted use: send each command, print what comes back until the shell
# has been quiet for two seconds.

exec 4<>"/dev/tcp/127.0.0.1/$port"
drain() {
  while IFS= read -r -t "$1" -u 4 line; do
    printf '%s\n' "${line%$'\r'}"
  done
}

drain 1 > /dev/null
printf '\r\n' >&4
drain 1 > /dev/null

for cmd in "$@"; do
  printf '%s\r\n' "$cmd" >&4
  drain "${DA1470X_RTT_QUIET:-2}"
done
