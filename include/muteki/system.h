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
 * @x_syscall_num `0x1002c`
 * @param frequency Frequency of the tone in Hz.
 * @param duration Duration of the tone in milliseconds.
 * @retval true @x_term ok
 */
extern bool Buzzer(int frequency, short duration);

/**
 * @brief Set Timer1 interrupt handler.
 * @details The callback function passed to this will be called once every time the timer fires. The exact interval
 * can be manually specified.
 * @note On all Arm-based machines I tested this is always emulated using a thread instead of using real hardware
 * timers. For TLCS-900-based machines this is likely different and it's unknown whether the callback will be called
 * in a way that's safe to the rest of the OS (e.g. calling the callback directly in timer ISR could break the OS).
 * @x_syscall_num `0x1002e`
 * @param callback The callback function called when timer fires, or `NULL` to unset a previously set callback.
 * @param interval An interval value in 10 milliseconds increments (1 means 10ms, 2 means 20, etc.), or defaulting to
 * 1 when set to 0.
 * @x_void_param
 */
extern void SetTimer1IntHandler(timer1_callback_t callback, short interval);

/**
 * @brief Get the Timer1 interrupt handler and custom interval.
 * @x_syscall_num `0x10030`
 * @param[out] interval A pointer to a variable that will hold the timer interval. Must not be NULL.
 * @return The callback function previously registered with SetTimer1IntHandler().
 */
extern timer1_callback_t GetTimer1IntHandler(short *interval);

/**
 * @brief Get the DOS 8.3 path to the system configuration directory on user data partition.
 * @details This function will also ensure that the path exists before returning it. Although if the function fails,
 * no directory will be created.
 * @x_syscall_num `0x1010e`
 * @param buffer The buffer to hold the path.
 * @param size The size of the buffer.
 * @return The buffer that now contains the path, or `NULL` if the function fails.
 */
extern char *_GetSystemDirectory(char *buffer, size_t size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_SYSTEM_H__
