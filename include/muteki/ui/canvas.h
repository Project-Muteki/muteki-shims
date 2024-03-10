/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file canvas.h
 * @brief Display canvas (LCD) API.
 * @details Note that the term "display canvas", "canvas" and "LCD" may be used interchangeably below. This may get
 * rectified in a future release.
 */

#ifndef __MUTEKI_UI_CANVAS_H__
#define __MUTEKI_UI_CANVAS_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Descriptor of an LCD drawing surface or hardware framebuffer.
 * @details This contains format description of the pixel/framebuffer and a pointer to the actual buffer.
 */
typedef struct {
    /**
     * @brief Header magic.
     * @details Always @p "PX" without NUL.
     */
    char magic[2]; // 0:2
    /**
     * @brief Width of the framebuffer.
     */
    unsigned short width; // 2:4
    /**
     * @brief Height of the framebuffer.
     */
    unsigned short height; // 4:6
    /**
     * @brief Depth of the color in bits.
     * @details 32 for RGBA, 4 for 16 color grayscale. Could be other values depending on the platform and the display
     * it uses.
     */
    unsigned short depth; // 6:8
    /**
     * @brief Pixel row size in bytes.
     * @details This is usually 4-bytes aligned.
     */
    unsigned short xsize; // 8:10
    /**
     * @brief Purpose unknown.
     * @details Seems to be always 0x2.
     */
    unsigned short unk_0xa; // 10:12
    /**
     * @brief Palette used to pack RGBA color into pixels.
     * @details Probably only make sense when using pixfmt other than RGBA (i.e. depth of 32). It should be NULL in
     * that case.
     */
    int *palette; // 12:16
    /**
     * @brief The actual framebuffer/VRAM region.
     * @details The actual format varies and depends on the parameters provided above.
     */
    void *buffer; // 16:20
} lcd_surface_t; // 20 bytes

/** Cursor struct usually embedded in LCD descriptor. */
typedef struct SYS_ALIGN(2) {
    /** X coordinate of the cursor. */
    short x; // 0x0:0x2
    /** Y coordinate of the cursor. */
    short y; // 0x2:0x4
    /** Size of the cursor. */
    unsigned int size; // 0x4:0x8
    /**
     * @brief Type of the cursor.
     * @todo Document the actual meaning of this.
     */
    short type; // 0x8:0xa
    /** Unknown. */
    short unk_0xa; // 0xa:0xc
    /** Number of times this cursor has been locked/grabbed by other code. */
    short grab_count; // 0xc:0xe
    /** Unknown. */
    short unk_0xe; // 0xe:0x10
} lcd_cursor_t; // 0x10 bytes

/** Rectangle used to represent usable drawing area in an LCD descriptor. */
typedef struct {
    /** Minimum X coordinate. */
    short xmin;
    /** Minimum Y coordinate. */
    short ymin;
    /** Maximum X coordinate. */
    short xmax;
    /** Maximum Y coordinate. */
    short ymax;
} lcd_rect_t;

/**
 * @brief The LCD descriptor.
 */
typedef struct {
    /** Surface linked to the LCD. */
    lcd_surface_t *surface; // 0x0:0x4
    /** End address of the pixel/framebuffer. */
    void *pixel_end; // 0x4:0x8
    /** Total size of the pixel/framebuffer in bytes. */
    size_t pixel_size; // 0x8:0xc
    /** Current background color. */
    int bg_color; // 0xc:0x10
    /** Unknown. */
    int unk_0x10; // 0x10:0x14
    /** Unknown. */
    int unk_0x14; // 0x14:0x18
    /** Current foreground color. */
    int fg_color; // 0x18:0x1c
    /** Unknown. */
    int unk_0x1c; // 0x1c:0x20
    /** Unknown. */
    int unk_0x20; // 0x20:0x24
    /** Unknown. */
    int unk_0x24; // 0x24:0x28
    /** Unknown. */
    int unk_0x28; // 0x28:0x2c
    /** Unknown. */
    int unk_0x2c; // 0x2c:0x30
    /** Unknown. */
    int unk_0x30; // 0x30:0x34
    /** Unknown. */
    int unk_0x34[10]; // 0x34:0x5c
    /** A copy of the cursor states when the LCD descriptor was created. */
    lcd_cursor_t saved_cursor; // 0x5c:0x6c
    /** Usable drawing area of the LCD. */
    lcd_rect_t rect; // 0x6c:0x74
    /** Unknown. */
    int unk_0x74[3]; // 0x74:0x80
    /** Cursor states. */
    lcd_cursor_t *cursor; // 0x80:0x84
    /** Width of the LCD in pixels. */
    short width; // 0x84:0x86
    /** Height of the LCD in pixels. */
    short height; // 0x86:0x88
    // ...a version-specific section starts somewhere after this...
    /** Unknown. */
    int unk_0x88[4]; // 0x88:0x98
    /** A critical section descriptor. It's unclear where it is used. */
    critical_section_t *cs; // 0x98:0x9c
    /** Unknown. */
    int unk_0x9c; // 0x9c:0xa0
    /** Unknown. */
    int unk_0xa0; // 0xa0:0xa4
    /** Unknown. */
    int unk_0xa4[23]; // 0xa4:0x100
} lcd_t; // 0x100 bytes

/**
 * @brief Font types.
 * @details The naming follows the following format, joined with underscores (_):
 *
 * `<style>_[variant]_[size]_<cjk>_[cjkvariant]_[cjksize]_[index]`
 *
 * Where
 * - `style` is one of `SANS`, `SERIF` or `MONOSPACE`.
 * - `variant` or `cjkvariant` is one of `BOLD` (equivalent to `REGULAR` for CJK), `ITALIC`, `BOLDITALIC`
 *   (equivalent to `ITALIC` for CJK) or `REGULAR` (omitted for `variant`). For `cjkvariant`, if it is the same as
 *   `variant`, it will be omitted.
 * - `size` or `cjksize` is one of `TINY` (8px height, not available for CJK), `SMALL` (12px), `NORMAL` (16px,
 *   omitted for `size`), `LARGE` (20px) or `HUGE` (22px, not available for CJK). For `cjksize`, if it is the
 *   same as `size`, it will be omitted.
 * - `cjk` is either `CJK` (supports CJK character rendering) or `NOCJK` (does not support CJK character rendering).
 * - `index` is the current index (only used when all the above fields collide).
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

/**
 * @brief Convert separate RGB values to integer RGB representation
 * @param r Red value.
 * @param g Green value.
 * @param b Blue value.
 */
#define RGB_FROM_U8(r, g, b) ((r & 0xff) << 16 | ((g & 0xff) << 8) | (b & 0xff))

/**
 * @brief Set display canvas transparent color.
 * @details This color will be removed from the image and replaced with transparency effect.
 * @x_syscall_num `0x10065`
 * @param color The integer RGB value representing a new color.
 * @return The integer RGB value representing the old color.
 */
extern int SetTransparentColor(int color);

/**
 * @brief Set display canvas background fill color.
 * @x_syscall_num `0x10067`
 * @param color The integer RGB value representing a new color.
 * @return The integer RGB value representing the old color.
 */
extern int rgbSetBkColor(int color);

/**
 * @brief Set display canvas foreground fill color.
 * @x_syscall_num `0x10068`
 * @param color The integer RGB value representing a new color.
 * @return The integer RGB value representing the old color.
 */
extern int rgbSetColor(int color);

/**
 * @brief Fill the current display canvas with background color, effectively clearing it.
 * @param fill_with_fg Fill with foreground color instead of background color.
 * @x_void_return
 */
extern void ClearScreen(bool fill_with_fg);

// TODO this does not work properly. Figure out why
extern void WriteAlignString(short x, short y, const char *msg, uint16_t max_width);

//extern int GetFontWidth(int32_t type);

/**
 * @brief Get the width of the character @p c used in @p font_type.
 * @param c UTF-16 codepoint of the character.
 * @param font_type The font type.
 * @return The width in pixels of that character.
 */
extern short GetCharWidth(UTF16 c, uint8_t font_type);

/**
 * @brief Get the height of the font @p font_type.
 * @param font_type The font type.
 * @return The height in pixels of that font.
 */
extern int GetFontHeight(uint8_t font_type);

/**
 * @brief Format and draw a string aligned to the top left corner at (@p x, @p y) px.
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param format The format string passed to the built-in sprintf().
 * @param ... Any subsequent values passed to the built-in sprintf().
 * @x_void_return
 */
extern void PrintfXY(short x, short y, const char *format, ...);

/**
 * @brief Set the current font type to @p font_type.
 * @param font_type The font type.
 * @x_void_return
 */
extern void SetFontType(int8_t font_type);

/**
 * @brief Draw a character @p c aligned to the top left corner at (@p x, @p y) px.
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param c UTF-16 codepoint of the character.
 * @param invert Whether or not to invert the color for that character.
 * @x_void_return
 */
extern void WriteChar(short x, short y, UTF16 c, bool invert);

/**
 * @brief Draw a UTF-16 string @p s aligned to the top left corner at (@p x, @p y) px.
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param s UTF-16 encoded string to be drawn.
 * @param invert Whether or not to invert the color for that character.
 * @x_void_return
 * @see WriteChar Similar function that displays single characters instead.
 */
extern void WriteString(short x, short y, const UTF16 *s, bool invert);

/**
 * @brief Get X coordinate of the rightmost visible pixels on the current canvas.
 * @details This is usually tied to the physical resolution of the display. Therefore it can be used to determine the
 * display size. The display size can be calculated by adding 1 to the return values of both GetMaxScr* calls.
 * @x_void_param
 * @return The X coordinate of the rightmost visible pixels.
 * @see GetMaxScrY Returns the Y boundary instead.
 */
extern short GetMaxScrX();

/**
 * @brief Get Y coordinate of the bottom-most visible pixels on the current canvas.
 * @details This is usually tied to the physical resolution of the display. Therefore it can be used to determine the
 * display size. The display size can be calculated by adding 1 to the return values of both `GetMaxScr*` calls.
 * @x_void_param
 * @return The Y coordinate of the bottom-most visible pixels.
 * @see GetMaxScrX Returns the X boundary instead.
 */
extern short GetMaxScrY();

/**
 * @brief Move a rectangle up by `amount` pixels.
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
void ScrollUp(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle down by `amount` pixels.
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
void ScrollDown(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle left by `amount` pixels.
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
void ScrollLeft(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle right by `amount` pixels.
 * @param x0 X coordinate of the top-left corner, in pixels.
 * @param y0 Y coordinate of the top-left corner, in pixels.
 * @param x1 X coordinate of the bottom-right corner, in pixels.
 * @param y1 Y coordinate of the bottom-right corner, in pixels.
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
void ScrollRight(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Get the cursor position on the current canvas.
 * @param[out] x The x coordinate of the cursor.
 * @param[out] y The y coordinate of the cursor.
 * @x_void_return
 */
void GetCursorPosition(short *x, short *y);

/**
 * @brief Move the cursor position on the current canvas to the specified coordinate.
 * @param x The new x coordinate of the cursor.
 * @param y The new y coordinate of the cursor.
 * @x_void_return
 */
void SetCursorPosition(short x, short y);

/**
 * @brief Get the type of the cursor on the current canvas.
 * @todo Document the actual type as an enum.
 * @x_void_param
 * @return The cursor type.
 */
short GetCursorType();

/**
 * @brief Set the type of the cursor on the current canvas.
 * @todo Document the actual type as an enum.
 * @param new_type The new cursor type.
 * @return The previous cursor type.
 */
short SetCursorType(short new_type);

/**
 * @brief Get the size of the cursor on the current canvas.
 * @x_void_param
 * @return The cursor size.
 */
unsigned int GetCursorSize();

/**
 * @brief Set the size of the cursor on the current canvas.
 * @param new_size The new cursor size.
 * @return The previous cursor size.
 */
unsigned int SetCursorSize(unsigned int new_size);

/**
 * @brief Lock the cursor on the current canvas.
 * @x_void
 */
void CursorLock();

/**
 * @brief Unlock the cursor on the current canvas.
 * @x_void
 */
void CursorUnock();

/**
 * @brief Create a virtual LCD descriptor.
 * @details Virtual LCDs allow the program to draw using the LCD/canvas API without committing the pixels to the screen
 * immediately.
 * @param width Width of the virtual LCD.
 * @param height Height of the virtual LCD.
 * @param width_bytes If set to a value larger than the value calculated from `width`, the pixel buffer will be
 * allocated according to this value instead (i.e. `width_bytes * height`).
 * @return The virtual LCD descriptor.
 */
lcd_t *CreateVirtualLCD(short width, short height, short width_bytes);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_CANVAS_H__
