/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file font.h
 * @brief Font-related API.
 */

#ifndef __MUTEKI_UI_FONT_H__
#define __MUTEKI_UI_FONT_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

//extern int GetFontWidth(int32_t type);

/**
 * @brief Get the width of the character `c` used in `font_type`.
 * @x_syscall_num `0x1004d`
 * @param c UTF-16 codepoint of the character.
 * @param font_type The font type.
 * @return The width in pixels of that character.
 */
extern short GetCharWidth(UTF16 c, uint8_t font_type);

/**
 * @brief Get the height of the font `font_type`.
 * @x_syscall_num `0x1004e`
 * @param font_type The font type.
 * @return The height in pixels of that font.
 */
extern int GetFontHeight(uint8_t font_type);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_FONT_H__
