/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file datetime.h
 * @brief Datetime operations.
 */

#ifndef __MUTEKI_DATETIME_H__
#define __MUTEKI_DATETIME_H__

#include <muteki/common.h>

/**
 * @brief The datetime struct used by GetSysTime() and SetSysTime().
 */
typedef struct {
    /** Year. */
    short year;
    /** Month of year. */
    short month;
    /** Day of week (0..6). 0 is Sunday. */
    short weekday;
    /** Day of month. */
    short day;
    /** Number of hours past midnight. */
    short hour;
    /** Number of minutes after the hour. */
    short minute;
    /** Number of seconds after the minute. */
    short second;
    /** Number of milliseconds after the second. */
    short millis;
} datetime_t;

/**
 * @brief Read current time of the day and store it in @p dt.
 *
 * @param dt Pointer to an empty datetime struct.
 */
extern void GetSysTime(datetime_t *dt);

#endif // __MUTEKI_DATETIME_H__
