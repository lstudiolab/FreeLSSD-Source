# This file has been modified by L,Studio.
# Copyright (c) 2026 L,Studio. All rights reserved.

# XUL host tests plus freestanding builds of XUL-owned code.
# Imported Mach/Linux/FreeBSD source is not yet linked into the kernel.
CC ?= cc
CC_ARM64 ?= aarch64-linux-gnu-gcc
LD_ARM64 ?= aarch64-linux-gnu-ld
BUILD_DIR ?= .build
CPPFLAGS += -Iinclude -Iio
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Werror -pedantic
FREESTANDING_CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding -fno-builtin -fno-stack-protector -fno-pic -fno-pie
X86_64_CFLAGS = $(FREESTANDING_CFLAGS) -m64 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-80387 -mcmodel=kernel
ARM64_CFLAGS = $(FREESTANDING_CFLAGS) -mgeneral-regs-only

XUL_COMMON_SOURCES = \
	core/subsystem.c core/device.c core/platform.c core/kernel.c core/memory.c core/runtime.c core/entry.c core/exception.c core/pci.c \
	boot/limine/entry.c boot/limine/requests.c \
	io/stream.c compat/linux/pci.c compat/linux/net.c \
	bsd/net/net.c bsd/vfs/vfs.c bsd/posix/syscall.c bsd/posix/fd.c bsd/nke/nke.c
XUL_X86_64_SOURCES = $(XUL_COMMON_SOURCES) platform/x86_64/platform.c platform/x86_64/pci.c platform/x86_64/exception.c
XUL_ARM64_SOURCES = $(XUL_COMMON_SOURCES) platform/arm64/platform.c platform/arm64/pci.c
X86_64_OBJECTS = $(patsubst %.c,$(BUILD_DIR)/x86_64/%.o,$(XUL_X86_64_SOURCES))
ARM64_OBJECTS = $(patsubst %.c,$(BUILD_DIR)/arm64/%.o,$(XUL_ARM64_SOURCES))

.PHONY: all test source-audit arch-build x86_64-build arm64-build limine-image-x86_64 limine-image-arm64 boot-images clean
all: test arch-build

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test_subsystem: core/subsystem.c tests/test_subsystem.c include/xul/subsystem.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING core/subsystem.c tests/test_subsystem.c -o $@

$(BUILD_DIR)/test_stream: io/stream.c tests/test_stream.c io/stream.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) io/stream.c tests/test_stream.c -o $@

$(BUILD_DIR)/test_memory: core/memory.c tests/test_memory.c include/xul/memory.h include/xul/boot.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) core/memory.c tests/test_memory.c -o $@

$(BUILD_DIR)/test_kernel: core/subsystem.c core/kernel.c core/memory.c tests/test_kernel.c include/xul/kernel.h include/xul/subsystem.h include/xul/platform.h include/xul/pci.h include/xul/memory.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING core/subsystem.c core/kernel.c core/memory.c tests/test_kernel.c -o $@

$(BUILD_DIR)/test_pci: core/device.c core/pci.c compat/linux/pci.c tests/test_pci.c include/xul/pci.h include/xul/linux_pci.h include/xul/device.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING core/device.c core/pci.c compat/linux/pci.c tests/test_pci.c -o $@

$(BUILD_DIR)/test_linux_pci: core/device.c compat/linux/pci.c tests/test_linux_pci.c include/xul/linux_pci.h include/xul/device.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING core/device.c compat/linux/pci.c tests/test_linux_pci.c -o $@

$(BUILD_DIR)/test_net: bsd/net/net.c bsd/nke/nke.c tests/test_net.c include/xul/net.h include/xul/nke.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING bsd/net/net.c bsd/nke/nke.c tests/test_net.c -o $@

$(BUILD_DIR)/test_linux_net: bsd/net/net.c bsd/nke/nke.c compat/linux/net.c tests/test_linux_net.c include/xul/linux_net.h include/xul/net.h include/xul/nke.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING bsd/net/net.c bsd/nke/nke.c compat/linux/net.c tests/test_linux_net.c -o $@

$(BUILD_DIR)/test_vfs: bsd/vfs/vfs.c tests/test_vfs.c include/xul/vfs.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING bsd/vfs/vfs.c tests/test_vfs.c -o $@

$(BUILD_DIR)/test_posix: bsd/posix/syscall.c tests/test_posix.c include/xul/posix.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING bsd/posix/syscall.c tests/test_posix.c -o $@

$(BUILD_DIR)/test_posix_vfs: bsd/vfs/vfs.c bsd/posix/fd.c tests/test_posix_vfs.c include/xul/vfs.h include/xul/posix.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING bsd/vfs/vfs.c bsd/posix/fd.c tests/test_posix_vfs.c -o $@

$(BUILD_DIR)/test_nke: bsd/nke/nke.c tests/test_nke.c include/xul/nke.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING bsd/nke/nke.c tests/test_nke.c -o $@

$(BUILD_DIR)/test_platform: core/platform.c tests/test_platform.c include/xul/platform.h include/xul/boot.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) core/platform.c tests/test_platform.c -o $@

$(BUILD_DIR)/test_device: core/device.c tests/test_device.c include/xul/device.h include/xul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXUL_TESTING core/device.c tests/test_device.c -o $@

test: $(BUILD_DIR)/test_subsystem $(BUILD_DIR)/test_stream $(BUILD_DIR)/test_device $(BUILD_DIR)/test_platform $(BUILD_DIR)/test_kernel $(BUILD_DIR)/test_memory $(BUILD_DIR)/test_pci $(BUILD_DIR)/test_linux_pci $(BUILD_DIR)/test_net $(BUILD_DIR)/test_linux_net $(BUILD_DIR)/test_vfs $(BUILD_DIR)/test_posix $(BUILD_DIR)/test_posix_vfs $(BUILD_DIR)/test_nke
	./$(BUILD_DIR)/test_subsystem
	./$(BUILD_DIR)/test_stream
	./$(BUILD_DIR)/test_device
	./$(BUILD_DIR)/test_platform
	./$(BUILD_DIR)/test_kernel
	./$(BUILD_DIR)/test_memory
	./$(BUILD_DIR)/test_pci
	./$(BUILD_DIR)/test_linux_pci
	./$(BUILD_DIR)/test_net
	./$(BUILD_DIR)/test_linux_net
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

$(BUILD_DIR)/x86_64/exceptions.o: boot/x86_64/exceptions.S
	@mkdir -p $(dir $@)
	$(CC) $(X86_64_CFLAGS) -c $< -o $@

$(BUILD_DIR)/arm64/entry.o: boot/arm64/entry.S
	@mkdir -p $(dir $@)
	$(CC_ARM64) $(ARM64_CFLAGS) -c $< -o $@

$(BUILD_DIR)/arm64/exceptions.o: boot/arm64/exceptions.S
	@mkdir -p $(dir $@)
	$(CC_ARM64) $(ARM64_CFLAGS) -c $< -o $@

$(BUILD_DIR)/x86_64/xul-core.o: $(X86_64_OBJECTS)
	$(CC) -m64 -nostdlib -r $^ -o $@

$(BUILD_DIR)/arm64/xul-core.o: $(ARM64_OBJECTS)
	$(LD_ARM64) -r $^ -o $@

$(BUILD_DIR)/x86_64/xul.elf: $(BUILD_DIR)/x86_64/entry.o $(BUILD_DIR)/x86_64/exceptions.o $(BUILD_DIR)/x86_64/xul-core.o boot/x86_64/linker.ld
	$(CC) -nostdlib -static -no-pie -Wl,--build-id=none -Wl,-z,max-page-size=0x1000 -Wl,-z,noexecstack -Wl,-T,boot/x86_64/linker.ld $(BUILD_DIR)/x86_64/entry.o $(BUILD_DIR)/x86_64/exceptions.o $(BUILD_DIR)/x86_64/xul-core.o -o $@

x86_64-build: $(BUILD_DIR)/x86_64/xul.elf
	@echo "XUL x86_64 ELF image link: PASS"

$(BUILD_DIR)/arm64/xul.elf: $(BUILD_DIR)/arm64/entry.o $(BUILD_DIR)/arm64/exceptions.o $(BUILD_DIR)/arm64/xul-core.o boot/arm64/linker.ld
	$(LD_ARM64) -static -z max-page-size=0x1000 -z noexecstack -T boot/arm64/linker.ld $(BUILD_DIR)/arm64/entry.o $(BUILD_DIR)/arm64/exceptions.o $(BUILD_DIR)/arm64/xul-core.o -o $@

arm64-build: $(BUILD_DIR)/arm64/xul.elf
	@echo "XUL AArch64 ELF image link: PASS"

arch-build: x86_64-build arm64-build

limine-image-x86_64: x86_64-build
	sh boot/limine/build-image.sh x86_64

limine-image-arm64: arm64-build
	sh boot/limine/build-image.sh arm64

boot-images: limine-image-x86_64 limine-image-arm64

source-audit:
	@set -eu; \
	for path in mach linux freebsd core include/xul io compat/linux bsd/net bsd/vfs bsd/posix bsd/nke platform/x86_64 platform/arm64 boot/x86_64 boot/arm64 boot/limine tests; do \
		test -d "$(CURDIR)/$$path" || { echo "missing XUL source root: $$path"; exit 1; }; \
	done
	@test -f xul.toml
	@test -f README.md
	@test -f ARCHITECTURE.md
	@test -f UPSTREAM-SOURCES.txt
	@test -f boot/README.md
	@test -f boot/limine/limine.conf
	@test -f boot/limine/build-image.sh
	@test -f boot/limine/qemu-smoke.sh
	@test -f boot/limine/entry.c
	@test -f boot/limine/requests.c
	@test -f include/xul/limine.h
	@test -f include/xul/linux_net.h
	@test -f compat/linux/net.c
	@test -f tests/test_linux_net.c
	@test -f boot/x86_64/linker.ld
	@test -f boot/arm64/linker.ld
	@test -f boot/x86_64/exceptions.S
	@test -f boot/arm64/exceptions.S
	@grep -q '^mach-osfmk ' UPSTREAM-SOURCES.txt
	@grep -q '^linux ' UPSTREAM-SOURCES.txt
	@grep -q '^freebsd ' UPSTREAM-SOURCES.txt
	@echo "XUL source audit: PASS"

clean:
	rm -rf $(BUILD_DIR)
