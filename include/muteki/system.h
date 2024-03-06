/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file system.h
 * @brief System management functions.
 */

#ifndef __MUTEKI_SYSTEM_H__
#define __MUTEKI_SYSTEM_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback type for Timer1 interrupt
 */
typedef void (*timer1_callback_t)(void);

/**
 * @brief Play a tone using the built in piezo speaker.
 * @details This will block the execution.
 * @param frequency Frequency of the tone in Hz.
 * @param duration Duration of the tone in milliseconds.
 * @retval true @x_term ok
 */
extern bool Buzzer(int frequency, short duration);

/**
 * @brief Set Timer1 interrupt handler.
 * @details The callback function passed to this will be called once every time the timer fires. The exact interval
 * can be manually specified.
 * @param callback The callback function called when timer fires.
 * @param interval An interval value in OSSleep() unit, or defaulting to 10 when set to 0.
 * @x_void_param
 */
extern void SetTimer1IntHandler(timer1_callback_t callback, short interval);

/**
 * @brief Get the Timer1 interrupt handler and custom interval.
 * @param[out] interval A pointer to a variable that will hold the timer interval. Must not be NULL.
 * @return The callback function previously registered with SetTimer1IntHandler().
 */
extern timer1_callback_t GetTimer1IntHandler(short *interval);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_SYSTEM_H__
