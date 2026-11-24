# XIUL host-side core tests and source-layout validation.
# Upstream Mach/Linux/FreeBSD kernel builds remain architecture-specific ports.
CC ?= cc
BUILD_DIR ?= .build
CPPFLAGS += -Iinclude -Iio
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Werror -pedantic

.PHONY: all test source-audit clean
all: test

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test_subsystem: core/subsystem.c tests/test_subsystem.c include/xiul/subsystem.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -DXIUL_TESTING core/subsystem.c tests/test_subsystem.c -o $@

$(BUILD_DIR)/test_stream: io/stream.c tests/test_stream.c io/stream.h include/xiul/status.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) io/stream.c tests/test_stream.c -o $@

test: $(BUILD_DIR)/test_subsystem $(BUILD_DIR)/test_stream
	./$(BUILD_DIR)/test_subsystem
	./$(BUILD_DIR)/test_stream

source-audit:
	@set -eu; \
	for path in mach linux freebsd core include/xiul io compat/linux bsd/net bsd/vfs bsd/posix bsd/nke tests; do \
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
