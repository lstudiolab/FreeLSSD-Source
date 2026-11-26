# This file has been modified by L,Studio.
# Copyright (c) 2026 L,Studio. All rights reserved.

CC ?= cc
BUILD_DIR ?= .build
CPPFLAGS += -Iinclude
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Werror -pedantic

.PHONY: all test clean
all: test

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/test_elf64: loader/elf64.c tests/test_elf64.c include/fboot/loader.h include/fboot/elf64.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) loader/elf64.c tests/test_elf64.c -o $@

test: $(BUILD_DIR)/test_elf64
	./$(BUILD_DIR)/test_elf64

clean:
	rm -rf $(BUILD_DIR)
