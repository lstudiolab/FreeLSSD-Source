# This file has been modified by L,Studio.
# Copyright (c) 2026 L,Studio. All rights reserved.

# XIUL host tests plus complete XIUL-owned freestanding core object builds.
# This does not claim to build imported upstream Mach/Linux/FreeBSD sources.
CC ?= cc
CC_ARM64 ?= aarch64-linux-gnu-gcc
LD_ARM64 ?= aarch64-linux-gnu-ld
BUILD_DIR ?= .build
CPPFLAGS += -Iinclude -Iio
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Werror -pedantic
FREESTANDING_CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -fno-pie
X86_64_CFLAGS = $(FREESTANDING_CFLAGS) -m64 -mno-red-zone -mcmodel=kernel
ARM64_CFLAGS = $(FREESTANDING_CFLAGS) -mgeneral-regs-only

XIUL_COMMON_SOURCES = \
	core/subsystem.c core/device.c core/platform.c core/kernel.c core/memory.c core/entry.c core/pci.c \
	io/stream.c compat/linux/pci.c \
	bsd/net/net.c bsd/vfs/vfs.c bsd/posix/syscall.c bsd/posix/fd.c bsd/nke/nke.c
XIUL_X86_64_SOURCES = $(XIUL_COMMON_SOURCES) platform/x86_64/platform.c platform/x86_64/pci.c
XIUL_ARM64_SOURCES = $(XIUL_COMMON_SOURCES) platform/arm64/platform.c platform/arm64/pci.c
X86_64_OBJECTS = $(patsubst %.c,$(BUILD_DIR)/x86_64/%.o,$(XIUL_X86_64_SOURCES))
ARM64_OBJECTS = $(patsubst %.c,$(BUILD_DIR)/arm64/%.o,$(XIUL_ARM64_SOURCES))

.PHONY: all test source-audit arch-build x86_64-build arm64-build clean
all: test arch-build

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test_subsystem: core/subsystem.c tests/test_subsystem.c include/xiul/subsystem.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/subsystem.c tests/test_subsystem.c -o $@

$(BUILD_DIR)/test_stream: io/stream.c tests/test_stream.c io/stream.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) io/stream.c tests/test_stream.c -o $@

$(BUILD_DIR)/test_memory: core/memory.c tests/test_memory.c include/xiul/memory.h include/xiul/boot.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) core/memory.c tests/test_memory.c -o $@

$(BUILD_DIR)/test_kernel: core/subsystem.c core/kernel.c core/memory.c tests/test_kernel.c include/xiul/kernel.h include/xiul/subsystem.h include/xiul/platform.h include/xiul/pci.h include/xiul/memory.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/subsystem.c core/kernel.c core/memory.c tests/test_kernel.c -o $@

$(BUILD_DIR)/test_pci: core/device.c core/pci.c compat/linux/pci.c tests/test_pci.c include/xiul/pci.h include/xiul/linux_pci.h include/xiul/device.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/device.c core/pci.c compat/linux/pci.c tests/test_pci.c -o $@

$(BUILD_DIR)/test_linux_pci: core/device.c compat/linux/pci.c tests/test_linux_pci.c include/xiul/linux_pci.h include/xiul/device.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/device.c compat/linux/pci.c tests/test_linux_pci.c -o $@

$(BUILD_DIR)/test_net: bsd/net/net.c tests/test_net.c include/xiul/net.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING bsd/net/net.c tests/test_net.c -o $@

$(BUILD_DIR)/test_vfs: bsd/vfs/vfs.c tests/test_vfs.c include/xiul/vfs.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING bsd/vfs/vfs.c tests/test_vfs.c -o $@

$(BUILD_DIR)/test_posix: bsd/posix/syscall.c tests/test_posix.c include/xiul/posix.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING bsd/posix/syscall.c tests/test_posix.c -o $@

$(BUILD_DIR)/test_posix_vfs: bsd/vfs/vfs.c bsd/posix/fd.c tests/test_posix_vfs.c include/xiul/vfs.h include/xiul/posix.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING bsd/vfs/vfs.c bsd/posix/fd.c tests/test_posix_vfs.c -o $@

$(BUILD_DIR)/test_nke: bsd/nke/nke.c tests/test_nke.c include/xiul/nke.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING bsd/nke/nke.c tests/test_nke.c -o $@

$(BUILD_DIR)/test_platform: core/platform.c tests/test_platform.c include/xiul/platform.h include/xiul/boot.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) core/platform.c tests/test_platform.c -o $@

$(BUILD_DIR)/test_device: core/device.c tests/test_device.c include/xiul/device.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/device.c tests/test_device.c -o $@

test: $(BUILD_DIR)/test_subsystem $(BUILD_DIR)/test_stream $(BUILD_DIR)/test_device $(BUILD_DIR)/test_platform $(BUILD_DIR)/test_kernel $(BUILD_DIR)/test_memory $(BUILD_DIR)/test_pci $(BUILD_DIR)/test_linux_pci $(BUILD_DIR)/test_net $(BUILD_DIR)/test_vfs $(BUILD_DIR)/test_posix $(BUILD_DIR)/test_posix_vfs $(BUILD_DIR)/test_nke
	./$(BUILD_DIR)/test_subsystem
	./$(BUILD_DIR)/test_stream
	./$(BUILD_DIR)/test_device
	./$(BUILD_DIR)/test_platform
	./$(BUILD_DIR)/test_kernel
	./$(BUILD_DIR)/test_memory
	./$(BUILD_DIR)/test_pci
	./$(BUILD_DIR)/test_linux_pci
	./$(BUILD_DIR)/test_net
	./$(BUILD_DIR)/test_vfs
	./$(BUILD_DIR)/test_posix
	./$(BUILD_DIR)/test_posix_vfs
	./$(BUILD_DIR)/test_nke

$(BUILD_DIR)/x86_64/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(X86_64_CFLAGS) -c $< -o $@

$(BUILD_DIR)/arm64/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC_ARM64) $(CPPFLAGS) $(ARM64_CFLAGS) -c $< -o $@

$(BUILD_DIR)/x86_64/entry.o: boot/x86_64/entry.S
	@mkdir -p $(dir $@)
	$(CC) $(X86_64_CFLAGS) -c $< -o $@

$(BUILD_DIR)/arm64/entry.o: boot/arm64/entry.S
	@mkdir -p $(dir $@)
	$(CC_ARM64) $(ARM64_CFLAGS) -c $< -o $@

$(BUILD_DIR)/x86_64/xiul-core.o: $(X86_64_OBJECTS)
	$(CC) -m64 -nostdlib -r $^ -o $@

$(BUILD_DIR)/arm64/xiul-core.o: $(ARM64_OBJECTS)
	$(LD_ARM64) -r $^ -o $@

$(BUILD_DIR)/x86_64/xiul.elf: $(BUILD_DIR)/x86_64/entry.o $(BUILD_DIR)/x86_64/xiul-core.o boot/x86_64/linker.ld
	$(CC) -nostdlib -no-pie -Wl,--build-id=none -Wl,-T,boot/x86_64/linker.ld $(BUILD_DIR)/x86_64/entry.o $(BUILD_DIR)/x86_64/xiul-core.o -o $@

x86_64-build: $(BUILD_DIR)/x86_64/xiul.elf
	@echo "XIUL x86_64 ELF image link: PASS"

$(BUILD_DIR)/arm64/xiul.elf: $(BUILD_DIR)/arm64/entry.o $(BUILD_DIR)/arm64/xiul-core.o boot/arm64/linker.ld
	$(LD_ARM64) -T boot/arm64/linker.ld $(BUILD_DIR)/arm64/entry.o $(BUILD_DIR)/arm64/xiul-core.o -o $@

arm64-build: $(BUILD_DIR)/arm64/xiul.elf
	@echo "XIUL AArch64 ELF image link: PASS"

arch-build: x86_64-build arm64-build

source-audit:
	@set -eu; \
	for path in mach linux freebsd core include/xiul io compat/linux bsd/net bsd/vfs bsd/posix bsd/nke platform/x86_64 platform/arm64 boot/x86_64 boot/arm64 tests; do \
		test -d "$(CURDIR)/$$path" || { echo "missing XIUL source root: $$path"; exit 1; }; \
	done
	@test -f xiul.toml
	@test -f README.md
	@test -f ARCHITECTURE.md
	@test -f UPSTREAM-SOURCES.txt
	@test -f boot/README.md
	@test -f boot/x86_64/linker.ld
	@test -f boot/arm64/linker.ld
	@grep -q '^mach-osfmk ' UPSTREAM-SOURCES.txt
	@grep -q '^linux ' UPSTREAM-SOURCES.txt
	@grep -q '^freebsd ' UPSTREAM-SOURCES.txt
	@echo "XIUL source audit: PASS"

clean:
	rm -rf $(BUILD_DIR)
