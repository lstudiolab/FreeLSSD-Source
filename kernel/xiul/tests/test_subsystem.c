/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#include <assert.h>
#include <stdio.h>
#include "xiul/subsystem.h"

static int sequence[16];
static size_t sequence_count;
static int fail_driver;

static xiul_status_t init_ok(xiul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = 1; return XIUL_OK; }
static xiul_status_t init_platform(xiul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = 2; return XIUL_OK; }
static xiul_status_t init_driver(xiul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = 3; return fail_driver ? XIUL_E_INIT : XIUL_OK; }
static void stop_record(xiul_kernel_context_t *context)
{ (void)context; sequence[sequence_count++] = -1; }

static const char *const platform_deps[] = {"mach"};
static const char *const driver_deps[] = {"platform"};
static const char *const missing_deps[] = {"not-registered"};
static const char *const cycle_a_deps[] = {"cycle-b"};
static const char *const cycle_b_deps[] = {"cycle-a"};

static const xiul_subsystem_descriptor_t mach = {
    XIUL_ABI_VERSION, "mach", XIUL_PHASE_CORE, 0, 0, init_ok, stop_record
};
static const xiul_subsystem_descriptor_t platform = {
    XIUL_ABI_VERSION, "platform", XIUL_PHASE_PLATFORM,
    platform_deps, 1, init_platform, stop_record
};
static const xiul_subsystem_descriptor_t driver = {
    XIUL_ABI_VERSION, "driver", XIUL_PHASE_DRIVERS,
    driver_deps, 1, init_driver, stop_record
};
static const xiul_subsystem_descriptor_t orphan = {
    XIUL_ABI_VERSION, "orphan", XIUL_PHASE_SERVICES,
    missing_deps, 1, init_ok, stop_record
};
static const xiul_subsystem_descriptor_t cycle_a = {
    XIUL_ABI_VERSION, "cycle-a", XIUL_PHASE_CORE,
    cycle_a_deps, 1, init_ok, stop_record
};
static const xiul_subsystem_descriptor_t cycle_b = {
    XIUL_ABI_VERSION, "cycle-b", XIUL_PHASE_CORE,
    cycle_b_deps, 1, init_ok, stop_record
};

static void reset(void)
{
    xiul_subsystem_test_reset();
    sequence_count = 0;
    fail_driver = 0;
}

static void test_dependency_order(void)
{
    reset();
    assert(xiul_subsystem_register(&driver) == XIUL_OK);
    assert(xiul_subsystem_register(&platform) == XIUL_OK);
    assert(xiul_subsystem_register(&mach) == XIUL_OK);
    assert(xiul_subsystem_validate() == XIUL_OK);
    assert(xiul_subsystem_start_all(0) == XIUL_OK);
    assert(sequence_count == 3);
    assert(sequence[0] == 1 && sequence[1] == 2 && sequence[2] == 3);
    xiul_subsystem_stop_all(0);
    assert(sequence_count == 6);
    assert(sequence[3] == -1 && sequence[4] == -1 && sequence[5] == -1);
}

static void test_duplicate_and_missing_dependency(void)
{
    reset();
    assert(xiul_subsystem_register(&mach) == XIUL_OK);
    assert(xiul_subsystem_register(&mach) == XIUL_E_DUPLICATE);
    assert(xiul_subsystem_register(&orphan) == XIUL_OK);
    assert(xiul_subsystem_validate() == XIUL_E_DEPENDENCY);
}

static void test_cycle_detection(void)
{
    reset();
    assert(xiul_subsystem_register(&cycle_a) == XIUL_OK);
    assert(xiul_subsystem_register(&cycle_b) == XIUL_OK);
    assert(xiul_subsystem_validate() == XIUL_E_CYCLE);
}

static void test_startup_rollback(void)
{
    reset();
    fail_driver = 1;
    assert(xiul_subsystem_register(&mach) == XIUL_OK);
    assert(xiul_subsystem_register(&platform) == XIUL_OK);
    assert(xiul_subsystem_register(&driver) == XIUL_OK);
    assert(xiul_subsystem_start_all(0) == XIUL_E_INIT);
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
    puts("XIUL subsystem tests: PASS");
    return 0;
}
