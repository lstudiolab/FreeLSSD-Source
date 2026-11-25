/*
 * This file has been modified by L,Studio.
 *
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XIUL_STATUS_H
#define XIUL_STATUS_H

#include <stdint.h>

typedef int32_t xiul_status_t;

enum {
    XIUL_OK = 0,
    XIUL_E_INVALID = -1,
    XIUL_E_DUPLICATE = -2,
    XIUL_E_DEPENDENCY = -3,
    XIUL_E_STATE = -4,
    XIUL_E_UNSUPPORTED = -5,
    XIUL_E_NOMEM = -6,
    XIUL_E_NOT_FOUND = -7,
    XIUL_E_CAPACITY = -8,
    XIUL_E_CYCLE = -9,
    XIUL_E_INIT = -10,
    XIUL_E_BUSY = -11
};

#endif
