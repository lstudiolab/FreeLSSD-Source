/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xul/subsystem.h"

static int sequence[16];
static size_t sequence_count;
static int fail_driver;

static xul_status_t init_ok(xul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = 1; return XUL_OK; }
static xul_status_t init_platform(xul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = 2; return XUL_OK; }
static xul_status_t init_driver(xul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = 3; return fail_driver ? XUL_E_NOMEM : XUL_OK; }
static void stop_record(xul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = -1; }

static const char *const platform_deps[] = {"mach"};
static const char *const driver_deps[] = {"platform"};
static const char *const missing_deps[] = {"not-registered"};
static const char *const cycle_a_deps[] = {"cycle-b"};
static const char *const cycle_b_deps[] = {"cycle-a"};

static const xul_subsystem_descriptor_t mach = {
    XUL_ABI_VERSION, "mach", XUL_PHASE_CORE, 0, 0, init_ok, stop_record
};
static const xul_subsystem_descriptor_t platform = {
    XUL_ABI_VERSION, "platform", XUL_PHASE_PLATFORM,
    platform_deps, 1, init_platform, stop_record
};
static const xul_subsystem_descriptor_t driver = {
    XUL_ABI_VERSION, "driver", XUL_PHASE_DRIVERS,
    driver_deps, 1, init_driver, stop_record
};
static const xul_subsystem_descriptor_t orphan = {
    XUL_ABI_VERSION, "orphan", XUL_PHASE_SERVICES,
    missing_deps, 1, init_ok, stop_record
};
static const xul_subsystem_descriptor_t cycle_a = {
    XUL_ABI_VERSION, "cycle-a", XUL_PHASE_CORE,
    cycle_a_deps, 1, init_ok, stop_record
};
static const xul_subsystem_descriptor_t cycle_b = {
    XUL_ABI_VERSION, "cycle-b", XUL_PHASE_CORE,
    cycle_b_deps, 1, init_ok, stop_record
};

static void reset(void)
{
    xul_subsystem_test_reset();
    sequence_count = 0;
    fail_driver = 0;
}

static void test_dependency_order(void)
{
    reset();
    assert(xul_subsystem_register(&driver) == XUL_OK);
    assert(xul_subsystem_register(&platform) == XUL_OK);
    assert(xul_subsystem_register(&mach) == XUL_OK);
    assert(xul_subsystem_validate() == XUL_OK);
    assert(xul_subsystem_start_all(0) == XUL_OK);
    assert(sequence_count == 3);
    assert(sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3);
    xul_subsystem_stop_all(0);
    assert(sequence_count == 6);
    assert(sequence[3] == -1 && sequence[4] == -1 && sequence[5] == -1);
}

static void test_duplicate_and_missing_dependency(void)
{
    reset();
    assert(xul_subsystem_register(&mach) == XUL_OK);
    assert(xul_subsystem_register(&mach) == XUL_E_DUPLICATE);
    assert(xul_subsystem_register(&orphan) == XUL_OK);
    assert(xul_subsystem_validate() == XUL_E_DEPENDENCY);
}

static void test_cycle_detection(void)
{
    reset();
    assert(xul_subsystem_register(&cycle_a) == XUL_OK);
    assert(xul_subsystem_register(&cycle_b) == XUL_OK);
    assert(xul_subsystem_validate() == XUL_E_CYCLE);
}

static void test_startup_rollback(void)
{
    reset();
    fail_driver = 1;
    assert(xul_subsystem_register(&mach) == XUL_OK);
    assert(xul_subsystem_register(&platform) == XUL_OK);
    assert(xul_subsystem_register(&driver) == XUL_OK);
    assert(xul_subsystem_start_all(0) == XUL_E_NOMEM);
    assert(sequence_count == 5);
    assert(sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3);
    assert(sequence[3] == -1 && sequence[4] == -1);
}

int main(void)
{
    test_dependency_order();
    test_duplicate_and_missing_dependency();
    test_cycle_detection();
    test_startup_rollback();
    puts("XUL subsystem tests: PASS");
    return 0;
}
