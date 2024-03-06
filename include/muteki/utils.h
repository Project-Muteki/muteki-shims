/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file utils.h
 * @brief Miscellaneous utility syscalls.
 * @details Contains syscalls that cannot be easily grouped with other syscalls.
 */

#ifndef __MUTEKI_UTILS_H__
#define __MUTEKI_UTILS_H__

#include <muteki/common.h>
#include <muteki/ui/canvas.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the descriptor for the currently active framebuffer.
 * @return The framebuffer descriptor.
 */ 
extern lcd_surface_t *GetActiveVRamAddress();

/**
 * @brief Play a beep using the built in piezo speaker.
 * @details This usually plays a 4000Hz tone for 10ms.
 * @retval true @x_term ok
 */
extern bool WarningBeep();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UTILS_H__
