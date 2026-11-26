#!/bin/sh
# This file has been modified by L,Studio.
# Copyright (c) 2026 L,Studio. All rights reserved.
set -eu

IMAGE="${1:?usage: qemu-smoke.sh <xul-x86_64.iso>}"
LOG="${2:-.build/xul-qemu-smoke.log}"
mkdir -p "$(dirname "$LOG")"

set +e
timeout 20s qemu-system-x86_64 \
    -machine q35 -m 512M -cdrom "$IMAGE" -boot d \
    -serial stdio -display none -monitor none \
    -no-reboot -no-shutdown >"$LOG" 2>&1
status=$?
set -e

# XUL currently stops after early bootstrap because the Mach scheduler and
# runtime services are not integrated. The timeout is expected; the serial
# log must prove that the memory allocator and PCI phase were reached.
if [ "$status" -ne 124 ]; then
    cat "$LOG"
    echo "XUL QEMU smoke test exited unexpectedly (status $status)" >&2
    exit 1
fi

require_log() {
    if ! grep -Fq "$1" "$LOG"; then
        cat "$LOG"
        echo "XUL QEMU smoke test missing expected boot milestone: $1" >&2
        exit 1
    fi
}

require_log "XUL: core bootstrap completed"
require_log "XUL: physical memory allocator online; total_pages="
require_log "XUL: PCI devices enumerated="
require_log "XUL: runtime services are not yet online; halting safely"

echo "XUL x86_64 Limine/QEMU early-bootstrap smoke test: PASS"
