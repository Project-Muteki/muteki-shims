/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file canvas.h
 * @brief Display canvas API.
 */

#ifndef __MUTEKI_UI_CANVAS_H__
#define __MUTEKI_UI_CANVAS_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Font types.
 *
 * The naming follows the following format, joined with underscores (_):
 *
 * <style> [variant] [size] <cjk> [cjkvariant] [cjksize] [index]
 *
 * Where
 * - @p style is one of @p SANS, @p SERIF or @p MONOSPACE.
 * - @p variant or @p cjkvariant is one of @p BOLD (equivalent to @p REGULAR for CJK), @p ITALIC, @p BOLDITALIC (equivalent to @p ITALIC for CJK) or @p REGULAR (omitted for @p variant). For @p cjkvariant, if it is the same as @p variant, it will be omitted.
 * - @p size or @p cjksize is one of @p TINY (8px height, not available for CJK), SMALL (12px), NORMAL (16px, omitted for @p size), LARGE (20px) or HUGE (22px, not available for CJK). For @p cjksize, if it is the same as @p size, it will be omitted.
 * - @p cjk is either @p CJK (supports CJK character rendering) or @p NOCJK (does not support CJK character rendering).
 * - @p index is the current index (only used when all the above fields collide).
 */
enum font_type_e {
    SANS_TINY_CJK_NORMAL = 0,
    SANS_BOLD_TINY_CJK_NORMAL,
    SANS_ITALIC_TINY_CJK_REGULAR_NORMAL,
    SANS_BOLDITALIC_TINY_CJK_REGULAR_NORMAL,
    SERIF_SMALL_CJK,
    SERIF_BOLD_SMALL_CJK,
    SERIF_ITALIC_SMALL_CJK_REGULAR,
    SERIF_BOLDITALIC_SMALL_CJK_REGULAR,
    SERIF_CJK,
    SERIF_BOLD_CJK,
    SERIF_ITALIC_CJK_REGULAR,
    SERIF_BOLDITALIC_CJK_REGULAR,
    SERIF_LARGE_CJK,
    SERIF_BOLD_LARGE_CJK,
    SERIF_ITALIC_LARGE_CJK,
    SERIF_BOLDITALIC_LARGE_CJK,
    MONOSPACE_CJK,
    MONOSPACE_TINY_NOCJK,
    SERIF_CJK_18,
    SERIF_BOLD_CJK_19,
    SERIF_SMALL_CJK_20,
    SANS_TINY_NOCJK,
    SANS_BOLD_TINY_NOCJK,
    SANS_ITALIC_TINY_NOCJK,
    SANS_BOLDITALIC_TINY_NOCJK,
    SERIF_SMALL_CJK_25,
    SERIF_BOLD_SMALL_CJK_26,
    SERIF_ITALIC_SMALL_CJK_REGULAR_27,
    SERIF_BOLDITALIC_SMALL_CJK_REGULAR_28,
    SERIF_CJK_29,
    SERIF_BOLD_CJK_30,
    SERIF_ITALIC_CJK_REGULAR_31,
    SERIF_BOLDITALIC_CJK_REGULAR_32,
    SERIF_HUGE_CJK_LARGE,
    SANS_HUGE_CJK_LARGE,
    SERIF_ITALIC_HUGE_CJK_LARGE,
    SANS_ITALIC_HUGE_CJK_LARGE,
};

// TODO verify this on a color screen device
/**
 * @brief Convert separate RGB values to integer RGB representation
 *
 * @param r Red value.
 * @param g Green value.
 * @param b Blue value.
 */
#define RGB_FROM_U8(r, g, b) ((r & 0xff) << 16 | ((g & 0xff) << 8) | (b & 0xff))

/**
 * @brief Set display canvas background fill color.
 *
 * @param rgb The integer RGB value.
 */
extern void rgbSetBkColor(int rgb);

/**
 * @brief Set display canvas foreground fill color.
 *
 * @param rgb The integer RGB value.
 */
extern void rgbSetColor(int rgb);

/**
 * @brief Fill the current display canvas with background color, effectively clearing it.
 *
 * @param fill_with_fg Fill with foreground color instead of background color.
 */
extern void ClearScreen(bool fill_with_fg);

// TODO this does not work properly. Figure out why
extern void WriteAlignString(short x, short y, const char *msg, uint16_t max_width);

//extern int GetFontWidth(int32_t type);

/**
 * @brief Get the width of the character @p c used in @p font_type.
 *
 * @param c UTF-16 codepoint of the character.
 * @param font_type The font type.
 * @return The width in pixels of that character.
 */
extern short GetCharWidth(UTF16 c, uint8_t font_type);

/**
 * @brief Get the height of the font @p font_type.
 *
 * @param font_type The font type.
 * @return The height in pixels of that font.
 */
extern int GetFontHeight(uint8_t font_type);

/**
 * @brief Format and draw a string aligned to the top left corner at (@p x, @p y) px.
 *
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param format The format string passed to the built-in sprintf().
 * @param ... Any subsequent values passed to the built-in sprintf().
 */
extern void PrintfXY(short x, short y, const char *format, ...);

/**
 * @brief Set the current font type to @p font_type.
 *
 * @param font_type The font type.
 */
extern void SetFontType(int8_t type);

/**
 * @brief Draw a character @p c aligned to the top left corner at (@p x, @p y) px.
 *
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param c UTF-16 codepoint of the character.
 * @param invert Whether or not to invert the color for that character.
 */
extern void WriteChar(short x, short y, UTF16 c, bool invert);

/**
 * @brief Draw a UTF-16 string @p s aligned to the top left corner at (@p x, @p y) px.
 *
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param s UTF-16 encoded string to be drawn.
 * @param invert Whether or not to invert the color for that character.
 * @see WriteChar Similar function that displays single characters instead.
 */
extern void WriteString(short x, short y, const UTF16 *s, bool invert);

/**
 * @brief Get X coordinate of the rightmost visible pixels on the current canvas.
 *
 * This is usually tied to the physical resolution of the display. Therefore it can be used to determine the display size. The display size can be calculated by adding 1 to the return values of both GetMaxScr* calls.
 *
 * @return The X coordinate of the rightmost visible pixels.
 * @see GetMaxScrY Returns the Y boundary instead.
 */
extern short GetMaxScrX();

/**
 * @brief Get Y coordinate of the bottom-most visible pixels on the current canvas.
 *
 * This is usually tied to the physical resolution of the display. Therefore it can be used to determine the display size. The display size can be calculated by adding 1 to the return values of both GetMaxScr* calls.
 *
 * @return The Y coordinate of the bottom-most visible pixels.
 * @see GetMaxScrX Returns the X boundary instead.
 */
extern short GetMaxScrY();

/**
 * @brief Move a rectangle up by @p amount pixels.
 *
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 */
void ScrollUp(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle down by @p amount pixels.
 *
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 */
void ScrollDown(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle left by @p amount pixels.
 *
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 */
void ScrollLeft(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle right by @p amount pixels.
 *
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 */
void ScrollRight(short x0, short y0, short x1, short y1, short amount);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_CANVAS_H__
