# XIUL host tests plus freestanding x86_64/AArch64 platform object builds.
CC ?= cc
CC_ARM64 ?= aarch64-linux-gnu-gcc
BUILD_DIR ?= .build
CPPFLAGS += -Iinclude -Iio
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Werror -pedantic
FREESTANDING_CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -ffreestanding -fno-builtin -fno-stack-protector -fno-pie

.PHONY: all test source-audit arch-build x86_64-build arm64-build clean
all: test arch-build

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test_subsystem: core/subsystem.c tests/test_subsystem.c include/xiul/subsystem.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/subsystem.c tests/test_subsystem.c -o $@

$(BUILD_DIR)/test_stream: io/stream.c tests/test_stream.c io/stream.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) io/stream.c tests/test_stream.c -o $@

$(BUILD_DIR)/test_kernel: core/subsystem.c core/kernel.c tests/test_kernel.c include/xiul/kernel.h include/xiul/subsystem.h include/xiul/platform.h include/xiul/pci.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/subsystem.c core/kernel.c tests/test_kernel.c -o $@

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
	$(CC) $(CPPFLAGS) $(CFLAGS) bsd/vfs/vfs.c bsd/posix/fd.c tests/test_posix_vfs.c -o $@

$(BUILD_DIR)/test_nke: bsd/nke/nke.c tests/test_nke.c include/xiul/nke.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING bsd/nke/nke.c tests/test_nke.c -o $@

$(BUILD_DIR)/test_platform: core/platform.c tests/test_platform.c include/xiul/platform.h include/xiul/boot.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) core/platform.c tests/test_platform.c -o $@

$(BUILD_DIR)/test_device: core/device.c tests/test_device.c include/xiul/device.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/device.c tests/test_device.c -o $@

test: $(BUILD_DIR)/test_subsystem $(BUILD_DIR)/test_stream $(BUILD_DIR)/test_device $(BUILD_DIR)/test_platform $(BUILD_DIR)/test_kernel $(BUILD_DIR)/test_pci $(BUILD_DIR)/test_linux_pci $(BUILD_DIR)/test_net $(BUILD_DIR)/test_vfs $(BUILD_DIR)/test_posix $(BUILD_DIR)/test_posix_vfs $(BUILD_DIR)/test_nke
	./$(BUILD_DIR)/test_subsystem
	./$(BUILD_DIR)/test_stream
	./$(BUILD_DIR)/test_device
	./$(BUILD_DIR)/test_platform
	./$(BUILD_DIR)/test_kernel
	./$(BUILD_DIR)/test_pci
	./$(BUILD_DIR)/test_linux_pci
	./$(BUILD_DIR)/test_net
	./$(BUILD_DIR)/test_vfs
	./$(BUILD_DIR)/test_posix
	./$(BUILD_DIR)/test_posix_vfs
	./$(BUILD_DIR)/test_nke

$(BUILD_DIR)/x86_64:
	mkdir -p $@

$(BUILD_DIR)/arm64:
	mkdir -p $@

x86_64-build: | $(BUILD_DIR)/x86_64
	$(CC) $(CPPFLAGS) $(FREESTANDING_CFLAGS) -m64 -mno-red-zone -mcmodel=kernel -c core/platform.c -o $(BUILD_DIR)/x86_64/platform-core.o
	$(CC) $(CPPFLAGS) $(FREESTANDING_CFLAGS) -m64 -mno-red-zone -mcmodel=kernel -c platform/x86_64/platform.c -o $(BUILD_DIR)/x86_64/platform-arch.o
	$(CC) $(CPPFLAGS) $(FREESTANDING_CFLAGS) -m64 -mno-red-zone -mcmodel=kernel -c platform/x86_64/pci.c -o $(BUILD_DIR)/x86_64/pci-host.o

arm64-build: | $(BUILD_DIR)/arm64
	$(CC_ARM64) $(CPPFLAGS) $(FREESTANDING_CFLAGS) -mgeneral-regs-only -c core/platform.c -o $(BUILD_DIR)/arm64/platform-core.o
	$(CC_ARM64) $(CPPFLAGS) $(FREESTANDING_CFLAGS) -mgeneral-regs-only -c platform/arm64/platform.c -o $(BUILD_DIR)/arm64/platform-arch.o
	$(CC_ARM64) $(CPPFLAGS) $(FREESTANDING_CFLAGS) -mgeneral-regs-only -c platform/arm64/pci.c -o $(BUILD_DIR)/arm64/pci-host.o

arch-build: x86_64-build arm64-build

source-audit:
	@set -eu; \
	for path in mach linux freebsd core include/xiul io compat/linux bsd/net bsd/vfs bsd/posix bsd/nke platform/x86_64 platform/arm64 tests; do \
		test -d "$(CURDIR)/$$path" || { echo "missing XIUL source root: $$path"; exit 1; }; \
	done
	@test -f xiul.toml
	@test -f README.md
	@test -f ARCHITECTURE.md
	@test -f UPSTREAM-SOURCES.txt
	@grep -q '^mach-osfmk ' UPSTREAM-SOURCES.txt
	@grep -q '^linux ' UPSTREAM-SOURCES.txt
	@grep -q '^freebsd ' UPSTREAM-SOURCES.txt
	@echo "XIUL source audit: PASS"

clean:
	rm -rf $(BUILD_DIR)
