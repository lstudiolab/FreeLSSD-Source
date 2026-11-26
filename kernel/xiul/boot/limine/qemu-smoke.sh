#!/bin/sh
# This file has been modified by L,Studio.
# Copyright (c) 2026 L,Studio. All rights reserved.
set -eu

IMAGE="${1:?usage: qemu-smoke.sh <xiul-x86_64.iso>}"
LOG="${2:-.build/xiul-qemu-smoke.log}"
mkdir -p "$(dirname "$LOG")"

set +e
timeout 20s qemu-system-x86_64 \
    -machine q35 -m 512M -cdrom "$IMAGE" \
    -serial stdio -display none -monitor none \
    -no-reboot -no-shutdown >"$LOG" 2>&1
status=$?
set -e

# A healthy bootstrap deliberately halts after printing its status. QEMU
# therefore remains alive until timeout; an early QEMU exit is a boot failure.
if [ "$status" -ne 124 ]; then
    cat "$LOG"
    echo "XIUL QEMU smoke test exited unexpectedly (status $status)" >&2
    exit 1
fi

if ! grep -Fq "XIUL: core bootstrap completed" "$LOG"; then
    cat "$LOG"
    echo "XIUL did not reach the common kernel bootstrap success path" >&2
    exit 1
fi

echo "XIUL x86_64 Limine/QEMU boot smoke test: PASS"
