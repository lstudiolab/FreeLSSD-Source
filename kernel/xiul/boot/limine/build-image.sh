#!/bin/sh
# This file has been modified by L,Studio.
# Copyright (c) 2026 L,Studio. All rights reserved.
#
# Build a Limine-bootable ISO using the official, pinned binary release.
set -eu

ARCH="${1:-}"
case "$ARCH" in
    x86_64|arm64) ;;
    *)
        echo "usage: $0 x86_64|arm64" >&2
        exit 2
        ;;
esac

XIUL_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR="$XIUL_DIR/.build"
LIMINE_VERSION="12.9.6"
LIMINE_SHA256="b38aabb042b0c0f9f562b7741be3c1204838d7bbacfa78bb411c674bd5b984ee"
LIMINE_ARCHIVE="$BUILD_DIR/limine-binary.tar.gz"
LIMINE_DIR="$BUILD_DIR/limine-binary"
ISO_ROOT="$BUILD_DIR/iso-root-$ARCH"
IMAGE="$BUILD_DIR/xiul-$ARCH.iso"

for tool in curl sha256sum tar make xorriso; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "missing required image-build tool: $tool" >&2
        exit 1
    }
done

mkdir -p "$BUILD_DIR"
if [ ! -f "$LIMINE_ARCHIVE" ]; then
    curl -fL --retry 3 -o "$LIMINE_ARCHIVE" \
        "https://github.com/Limine-Bootloader/Limine/releases/download/v$LIMINE_VERSION/limine-binary.tar.gz"
fi
printf '%s  %s\n' "$LIMINE_SHA256" "$LIMINE_ARCHIVE" | sha256sum -c -

if [ ! -x "$LIMINE_DIR/limine" ]; then
    rm -rf "$LIMINE_DIR"
    tar -xzf "$LIMINE_ARCHIVE" -C "$BUILD_DIR"
    make -C "$LIMINE_DIR"
fi

rm -rf "$ISO_ROOT"
mkdir -p "$ISO_ROOT/boot/limine" "$ISO_ROOT/EFI/BOOT"
cp "$XIUL_DIR/boot/limine/limine.conf" "$ISO_ROOT/boot/limine/limine.conf"
cp "$BUILD_DIR/$ARCH/xiul.elf" "$ISO_ROOT/boot/xiul.elf"
cp "$LIMINE_DIR/limine-uefi-cd.bin" "$ISO_ROOT/boot/limine/limine-uefi-cd.bin"

if [ "$ARCH" = "x86_64" ]; then
    cp "$LIMINE_DIR/limine-bios.sys" "$ISO_ROOT/boot/limine/limine-bios.sys"
    cp "$LIMINE_DIR/limine-bios-cd.bin" "$ISO_ROOT/boot/limine/limine-bios-cd.bin"
    cp "$LIMINE_DIR/BOOTX64.EFI" "$ISO_ROOT/EFI/BOOT/BOOTX64.EFI"
    xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part -efi-boot-image --protective-msdos-label \
        "$ISO_ROOT" -o "$IMAGE"
    "$LIMINE_DIR/limine" bios-install "$IMAGE"
else
    cp "$LIMINE_DIR/BOOTAA64.EFI" "$ISO_ROOT/EFI/BOOT/BOOTAA64.EFI"
    xorriso -as mkisofs -R -r -J -hfsplus -apm-block-size 2048 \
        --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part \
        -efi-boot-image --protective-msdos-label "$ISO_ROOT" -o "$IMAGE"
fi

echo "XIUL $ARCH Limine ISO: $IMAGE"
