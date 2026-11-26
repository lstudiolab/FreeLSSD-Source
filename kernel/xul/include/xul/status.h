/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 */

#ifndef XUL_STATUS_H
#define XUL_STATUS_H

#include <stdint.h>

typedef int32_t xul_status_t;

enum {
    XUL_OK = 0,
    XUL_E_INVALID = -1,
    XUL_E_DUPLICATE = -2,
    XUL_E_DEPENDENCY = -3,
    XUL_E_STATE = -4,
    XUL_E_UNSUPPORTED = -5,
    XUL_E_NOMEM = -6,
    XUL_E_NOT_FOUND = -7,
    XUL_E_CAPACITY = -8,
    XUL_E_CYCLE = -9,
    XUL_E_INIT = -10,
    XUL_E_BUSY = -11,
    XUL_E_DROPPED = -12
};

#endif
