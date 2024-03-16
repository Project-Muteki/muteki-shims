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
#include <muteki/threading.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Process flags for string and character printing.
 * @todo A lot of the bits seem to just print nothing. We need to formally look at the disassembly for this (huge!).
 */
enum print_flag_e {
    /**
     * @brief No extra process.
     */
    PRINT_NONE = 0,
    /**
     * @brief Invert colors.
     * @details Within the bounding box of the glyphs to be rendered, the RGB pixel value is inverted.
     */
    PRINT_INVERT = 0x1,
    /**
     * @brief Seems to be an alternative version of `PRINT_INVERT`.
     */
    PRINT_INVERT_ALT = 0x4,
    /**
     * @brief Draw underscores only.
     * @todo Speculation based on behavior. Seems to be very broken with >16px fonts.
     */
    PRINT_UNDERSCORE_ONLY = 0x40,
};

/**
 * @brief Process flags for string printing only.
 */
enum print_str_flag_e {
    /**
     * @brief Assume text encoding to be BIG5 instead of UTF-16.
     * @todo Seems to be some unknown extension? Limited mojibake test doesn't match iconv result.
     */
    PRINT_STR_ENCODING_BIG5 = 0x100,
    /**
     * @brief Assume text encoding to be GB18030 instead of UTF-16.
     */
    PRINT_STR_ENCODING_GB18030 = 0x200,
    /**
     * @brief Assume text encoding to be TIS-620 instead of UTF-16.
     */
    PRINT_STR_ENCODING_TIS_620 = 0x400,
};

/**
 * @brief Process flags for blit operations.
 * @todo Need to look into this further. 2 and 16 seem to draw the image verbatim like 0. Nothing is being drawn when
 * setting any other flags. (Could it be related to some other properties on the surface descriptor such as transparent
 * color?)
 */
enum blit_flag_e {
    /** @brief Nothing. */
    BLIT_NONE = 0,
    /** @brief Color is inverted. */
    BLIT_INVERT = 0x4,
};

/**
 * @brief Descriptor of an LCD drawing surface or hardware framebuffer.
 * @details This contains format description of the pixel/framebuffer and a pointer to the actual buffer.
 */
typedef struct {
    /**
     * @brief Header magic.
     * @details Always `"PX"` without NUL.
     */
    char magic[2]; // 0:2
    /**
     * @brief Width of the framebuffer.
     */
    short width; // 2:4
    /**
     * @brief Height of the framebuffer.
     */
    short height; // 4:6
    /**
     * @brief Depth of the color in bits.
     * @details 32 for RGBA, 4 for 16 color grayscale. Could be other values depending on the platform and the display
     * it uses.
     */
    short depth; // 6:8
    /**
     * @brief Pixel row size in bytes.
     * @details This is usually 4-bytes aligned.
     */
    short xsize; // 8:10
    /**
     * @brief Purpose unknown.
     * @details Seems to be always 0x2.
     */
    short unk_0xa; // 10:12
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

/**
 * @brief Cursor states usually linked to an LCD descriptor.
 */
typedef struct SYS_ALIGN(2) {
    /** @brief X coordinate of the cursor. */
    short x; // 0x0:0x2 (lcd_t[0x5c:0x5e])
    /** @brief Y coordinate of the cursor. */
    short y; // 0x2:0x4 (lcd_t[0x5e:0x60])
    /** @brief Size of the cursor. */
    unsigned int size; // 0x4:0x8 (lcd_t[0x60:0x64])
    /**
     * @brief Type of the cursor.
     * @todo Document the actual meaning of this.
     */
    short type; // 0x8:0xa (lcd_t[0x64:0x66])
    /** @brief Unknown. */
    short unk_0xa; // 0xa:0xc (lcd_t[0x66:0x68])
    /** @brief Number of times this cursor has been locked/grabbed by other code. */
    short grab_count; // 0xc:0xe (lcd_t[0x68:0x6a])
    /** @brief Unknown. */
    short unk_0xe; // 0xe:0x10 (lcd_t[0x6a:0x6c])
} lcd_cursor_t; // 0x10 bytes

/**
 * @brief Rectangle used to represent usable drawing area in an LCD descriptor.
 */
typedef struct {
    /** @brief @x_term x0 */
    short x0; // (lcd_t[0x6c:0x6e])
    /** @brief @x_term y0 */
    short y0; // (lcd_t[0x6e:0x70])
    /** @brief @x_term x1 */
    short x1; // (lcd_t[0x70:0x72])
    /** @brief @x_term y1 */
    short y1; // (lcd_t[0x72:0x74])
} lcd_rect_t; // 0x8 bytes

/**
 * @brief Drawing routine common states.
 */
typedef struct {
    /** @brief Unknown. */
    int unk_0x0; // 0x0:0x4 (lcd_t[0x14:0x18])
    /** @brief Current foreground color. */
    int fg_color; // 0x4:0x8 (lcd_t[0x18:0x1c])
    /** @brief Unknown. */
    int unk_0x8; // 0x8:0xc (lcd_t[0x1c:0x20])
    /** @brief Unknown. */
    int unk_0xc; // 0xc:0x10 (lcd_t[0x20:0x24])
    /** @brief Unknown. */
    int unk_0x10; // 0x10:0x14 (lcd_t[0x24:0x28])
    /** @brief Unknown. */
    int unk_0x14; // 0x14:0x18 (lcd_t[0x28:0x2c])
    /** @brief Unknown. */
    int unk_0x18; // 0x18:0x1c (lcd_t[0x2c:0x30])
    /** @brief Unknown. */
    int unk_0x1c; // 0x1c:0x20 (lcd_t[0x30:0x34])
} lcd_draw_t; // 0x20 bytes

/**
 * @brief Font rendering routine common states.
 */
typedef struct {
    /**
     * @brief Font type.
     * @see font_type_e
     */
    short type; // 0x0:0x2 (lcd[0x34:0x36])
    /** @brief Height of the font. */
    short height; // 0x2:0x4 (lcd[0x36:0x38])
    /**
     * @brief Unknown.
     * @details Normally the same as `height` but sometimes can be a different value. Touched by SetFontType().
     */
    short unk_0x4; // 0x4:0x6 (lcd[0x38:0x3a])
    /** @brief Unknown. */
    short unk_0x6; // 0x6:0x8 (lcd[0x3a:0x3c])
    /** @brief Unknown. */
    short unk_0x8; // 0x8:0xa (lcd[0x3c:0x3e])
    /**
     * @brief Unknown.
     * @details Seems to be always `0x100`. Touched by SetFontType().
     */
    short unk_0xa; // 0xa:0xc (lcd[0x3e:0x40])
    /**
     * @brief Unknown.
     * @details Seems to be always `0x801`. Touched by SetFontType().
     */
    short unk_0xc; // 0xc:0xe (lcd[0x40:0x42])
    short unk_0xe[13]; // 0xe:0x28 (lcd[0x42:0x5c])
} lcd_font_t; // 0x28 bytes

/**
 * @brief An extended part of the LCD descriptor that implements descriptor locking.
 * @details This may not be present on all versions of Besta RTOS. It's confirmed to exist on BA742 but not BA110.
 * @todo Get a better picture of which systems have this and which do not.
 */
typedef struct {
    /** @brief Unknown. */
    int unk_0x0; // 0x0:0x4 (lcd_thread_safe_t[0x94:0x98])
    /** @brief A critical section descriptor. It's unclear where it is used. */
    critical_section_t *cs; // 0x4:0x8 (lcd_thread_safe_t[0x98:0x9c])
    /** @brief Shortcut to lock the descriptor. */
    void (*lock)(); // 0x8:0xc (lcd_thread_safe_t[0x9c:0xa0])
     /** @brief Shortcut to unlock the descriptor. */
    void (*unlock)(); // 0xc:0x10 (lcd_thread_safe_t[0xa0:0xa4])
    /** @brief Unknown. */
    int unk_0xa4[23]; // 0x10:0x6c (lcd_thread_safe_t[0xa4:0x100])
} lcd_lock_t; // 0x6c bytes

struct lcd_s;
typedef struct lcd_s lcd_t;

/**
 * @brief Callback type for handling canvas rotation.
 * @details When `rotation` is set to `0xfe`, the current rotation value will be returned with no side effect.
 * When `rotation` is set to `0xff`, the canvas will rotate based on lcd_t::rotation.
 * @param self The LCD descriptor this was called from.
 * @param rotation New rotation. This value will be written to lcd_t::rotation and this callback will be called
 * recursively with `0xff` as the rotation value to actually apply the change.
 * @return Current rotation value in effect. This will be the same as `rotation` when no magic value documented above
 * was used.
 * @see lcd_t::rotation
 */
typedef int (*lcd_rotate_callback_t)(lcd_t *self, int rotation);

/**
 * @brief The LCD descriptor.
 */
struct lcd_s {
    /** @brief Surface linked to the LCD. */
    lcd_surface_t *surface; // 0x0:0x4
    /** @brief End address of the pixel/framebuffer. */
    void *pixel_end; // 0x4:0x8
    /** @brief Total size of the pixel/framebuffer in bytes. */
    size_t pixel_size; // 0x8:0xc
    /** @brief Current background color. */
    int bg_color; // 0xc:0x10
    /** @brief Current transparent color. */
    int transparent_color; // 0x10:0x14
    /** @brief Per-LCD states shared by drawing routines. */
    lcd_draw_t draw; // 0x14:0x34
    /** @brief Per-LCD states shared by specifically font rendering routines. */
    lcd_font_t font; // 0x34:0x5c
    /** @brief A copy of the cursor states when the LCD descriptor was created. */
    lcd_cursor_t saved_cursor; // 0x5c:0x6c
    /** @brief Usable drawing area of the LCD. */
    lcd_rect_t drawing_area; // 0x6c:0x74
    /** @brief Unknown. */
    int unk_0x74[3]; // 0x74:0x80
    /** @brief Cursor states. */
    lcd_cursor_t *cursor; // 0x80:0x84
    /** @brief Width of the LCD in pixels. */
    short width; // 0x84:0x86
    /** @brief Height of the LCD in pixels. */
    short height; // 0x86:0x88
    /**
     * @brief Current canvas rotation.
     * @details The value is `90deg * rotation` **counter-clockwise**.
     */
    int rotation; // 0x88:0x8c
    /**
     * @brief Integer size of each pixel in bytes.
     * @note This will be 0 when a pixel takes less than a byte.
     * @todo Verify.
     */
    short depth_bytes; // 0x8c:0x8e
    /**
     * @brief Pixel row size in bytes.
     * @todo Verify.
     */
    short xsize; // 0x8e:0x90
    /**
     * @brief Rotation callback.
     * @see lcd_rotate_callback_t
     */
    lcd_rotate_callback_t rotate; // 0x90:0x94
}; // 0x94 bytes

/**
 * @brief A thread-safe variant of the LCD descriptor used on some versions of Besta RTOS.
 * @details Cast lcd_t to this to access the extra fields if the data exists.
 */
typedef struct {
    /** @brief The common part of the original LCD descriptor. */
    lcd_t lcd; // 0x0:0x94
    /** @brief The added part. */
    lcd_lock_t lock; // 0x94:0x100
} lcd_thread_safe_t; // 0x100 bytes

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
extern void WriteAlignString(short x, short y, const UTF16 *msg, int arg4, int arg5, unsigned int flags);

//extern int GetFontWidth(int32_t type);

/**
 * @brief Get the width of the character `c` used in `font_type`.
 * @param c UTF-16 codepoint of the character.
 * @param font_type The font type.
 * @return The width in pixels of that character.
 */
extern short GetCharWidth(UTF16 c, uint8_t font_type);

/**
 * @brief Get the height of the font `font_type`.
 * @param font_type The font type.
 * @return The height in pixels of that font.
 */
extern int GetFontHeight(uint8_t font_type);

/**
 * @brief Format and draw a string.
 * @details Handles line wraps and screen scrolling automatically.
 * @param format The format string.
 * @param ... Any subsequent values.
 * @x_void_return
 */
extern void Printf(char *format, ...);

/**
 * @brief Format and draw a string aligned to the top left corner at `(x, y)` px.
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param format The format string passed to the built-in sprintf().
 * @param ... Any subsequent values passed to the built-in sprintf().
 * @x_void_return
 */
extern void PrintfXY(short x, short y, const char *format, ...);

/**
 * @brief Draw a surface onto current active LCD.
 * @todo More details.
 * @x_syscall_num `0x10059`
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param surface The surface descriptor.
 * @param flags
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 * @see blit_flag_e Accepted processing flags.
 */
extern int ShowGraphic(short x, short y, lcd_surface_t *surface, unsigned short flags);

/**
 * @brief Calculate the total on-memory size of a surface (including descriptor).
 * @x_syscall_num `0x1005a`
 * @param surface The surface descriptor.
 * @return The total on-memory size of this surface in bytes.
 */
extern size_t SizeofGraphic(lcd_surface_t *surface);

/**
 * @brief Initialize a buffer as an all-in-one surface.
 * @details If a palette is required (`depth` is 8-bit or less), the palette is assumed to follow the descriptor
 * immediately, then the pixel buffer is assumed to start immediately after the palette. Otherwise the pixel buffer is
 * assumed to follow the descriptor immediately (i.e. palette size is 0) and lcd_surface_t::palette will be set to
 * NULL.
 * @note This function does not allocate any memory. The caller is responsible for allocating a buffer that is large
 * enough to hold the descriptor, the palette (if required) and the pixel buffer.
 * @x_syscall_num `0x1005b`
 * @param surface A buffer that will hold the data.
 * @param width Width of the surface in pixels.
 * @param height Height of the surface in pixels.
 * @param depth The pixel bit depth.
 * @return The same buffer.
 * @see GetImageSizeExt A function that can be used to estimate the required size of the `surface` buffer, based on the
 * same parameters that will be passed to this function.
 */
extern lcd_surface_t *InitGraphic(lcd_surface_t *surface, short width, short height, short depth);

/**
 * @brief Get the current font type.
 * @x_syscall_num `0x1004f`
 * @x_void_param
 * @return The current font type.
 * @see font_type_e Valid values for `font_type`.
 */
extern short GetFontType();

/**
 * @brief Set the current font type to `font_type`.
 * @x_syscall_num `0x10051`
 * @param font_type The font type.
 * @x_void_return
 * @see font_type_e Valid values for `font_type`.
 */
extern void SetFontType(short font_type);

/**
 * @brief Draw a UTF-16 character `c` aligned to the top left corner at `(x, y)` px.
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param c UTF-16 codepoint of the character.
 * @param flags Process flags.
 * @x_void_return
 * @see print_flag_e Valid process flags.
 */
extern void WriteChar(short x, short y, UTF16 c, unsigned int flags);

/**
 * @brief Draw a string `s` aligned to the top left corner at `(x, y)` px.
 * @param x X coordinate of the corner.
 * @param y Y coordinate of the corner.
 * @param[in] s String to be drawn. Exact encoding depends on the process flags being used.
 * @param flags Process flags.
 * @x_void_return
 * @see WriteChar Similar function that displays single characters instead.
 * @see print_flag_e Valid process flags.
 * @see print_str_flag_e Encoding conversion flags.
 */
extern void WriteString(short x, short y, const void *s, unsigned int flags);

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
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollUp(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle down by `amount` pixels.
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollDown(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle left by `amount` pixels.
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollLeft(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Move a rectangle right by `amount` pixels.
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @param amount Amount of pixels to move.
 * @x_void_return
 */
extern void ScrollRight(short x0, short y0, short x1, short y1, short amount);

/**
 * @brief Get the cursor position on the current canvas.
 * @param[out] x The x coordinate of the cursor.
 * @param[out] y The y coordinate of the cursor.
 * @x_void_return
 */
extern void GetCursorPosition(short *x, short *y);

/**
 * @brief Move the cursor position on the current canvas to the specified coordinate.
 * @param x The new x coordinate of the cursor.
 * @param y The new y coordinate of the cursor.
 * @x_void_return
 */
extern void SetCursorPosition(short x, short y);

/**
 * @brief Get the type of the cursor on the current canvas.
 * @todo Document the actual type as an enum.
 * @x_void_param
 * @return The cursor type.
 */
extern short GetCursorType();

/**
 * @brief Set the type of the cursor on the current canvas.
 * @todo Document the actual type as an enum.
 * @param new_type The new cursor type.
 * @return The previous cursor type.
 */
extern short SetCursorType(short new_type);

/**
 * @brief Get the size of the cursor on the current canvas.
 * @x_void_param
 * @return The cursor size.
 */
extern unsigned int GetCursorSize();

/**
 * @brief Set the size of the cursor on the current canvas.
 * @param new_size The new cursor size.
 * @return The previous cursor size.
 */
extern unsigned int SetCursorSize(unsigned int new_size);

/**
 * @brief Lock the cursor on the current canvas.
 * @x_void
 */
extern void CursorLock();

/**
 * @brief Unlock the cursor on the current canvas.
 * @x_void
 */
extern void CursorUnock();

/**
 * @brief Create a virtual LCD descriptor.
 * @details Virtual LCDs allow the program to draw using the LCD/canvas API without committing the pixels to the screen
 * immediately.
 * @x_syscall_num `0x10087`
 * @param width Width of the virtual LCD.
 * @param height Height of the virtual LCD.
 * @param width_bytes If set to a value larger than the value calculated from `width`, the pixel buffer will be
 * allocated according to this value instead (i.e. `width_bytes * height`).
 * @return The virtual LCD descriptor.
 */
extern lcd_t *CreateVirtualLCD(short width, short height, short width_bytes);

/**
 * @brief Dispose a previously created virtual LCD descriptor.
 * @x_syscall_num `0x10088`
 * @param lcd Pointer to a virtual LCD descriptor.
 * @x_void_return
 */
extern void DeleteVirtualLCD(lcd_t *lcd);

/**
 * @brief Perform blit operation from `src` surface to `dst` surface.
 * @details This copies a rectangle of size `(xsize, ysize)px` on the `src` surface, with its the top left corner
 * located at `(xsrcoffset, ysrcoffset)px`, to the `dst` surface. The top left corner of that rectangle will be
 * aligned to a point on the `dst` surface at `(xdstoffset, ydstoffset)px`. Optionally one can specify one or more
 * processing `flags` to let the blitter perform certain pixel operations on-the-fly.
 * @x_syscall_num `0x10089`
 * @param dst Destination surface.
 * @param xdstoffset X coordinate of the destination surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param ydstoffset Y coordinate of the destination surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param xsize Number of pixel columns to copy.
 * @param ysize Number of pixel rows to copy.
 * @param src Source surface.
 * @param xsrcoffset X coordinate of the source surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param ysrcoffset Y coordinate of the source surface that align with the top left corner of the blitted image,
 * in pixels.
 * @param flags Process flags.
 * @x_void_return
 * @see blit_flag_e Accepted processing flags.
 */
extern void _BitBlt(
    lcd_surface_t *dst,
    short xdstoffset,
    short ydstoffset,
    short xsize,
    short ysize,
    lcd_surface_t *src,
    int xsrcoffset,
    int ysrcoffset,
    unsigned short flags
);

/**
 * @brief Set an LCD descriptor as active.
 * @x_syscall_num `0x1008b`
 * @param new_lcd Pointer to the new LCD descriptor. If `NULL`, only return the current active LCD descriptor.
 * @return The active LCD descriptor before `new_lcd` replaced it.
 */
extern lcd_t *SetActiveLCD(lcd_t *new_lcd);

/**
 * @brief Get the current active LCD descriptor.
 * @x_syscall_num `0x1008d`
 * @x_void_param
 * @return The current active LCD descriptor.
 */
extern lcd_t *GetActiveLCD();

/**
 * @brief Set the drawing area of the current active LCD.
 * @details The new values will be rejected if they are out of bounds (specifically when `x0` or `x1` are negative or
 * `x1` or `y1` are less than `x0` and `y0` respectively), although `x1` and `y1` will be automatically capped at the
 * *physical* display size if they are larger than that instead of failing outright.
 * @x_syscall_num `0x10073`
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1
 * @param y1 @x_term y1
 * @x_void_return
 */
extern void SetDrawArea(short x0, short y0, short x1, short y1);

/**
 * @brief Get the drawing area of the current active LCD.
 * @x_syscall_num `0x10074`
 * @param[out] x0 @x_term x0
 * @param[out] y0 @x_term y0
 * @param[out] x1 @x_term x1
 * @param[out] y1 @x_term y1
 * @x_void_return
 */
extern void GetDrawArea(short *x0, short *y0, short *x1, short *y1);

/**
 * @brief Copy an LCD descriptor (excluding surface).
 * @details This function allocates a new LCD descriptor and copies everything from the source descriptor to the new
 * one. The new descriptor will not be linked to the source descriptor's lcd_t::surface, and lcd_t::saved_cursor
 * will be updated to the latest cursor states assigned to the current active LCD descriptor. If the target platform
 * supports LCD descriptor locking, a new critical section object will be created for the new descriptor.
 * @warning The new descriptor will not have any surface linked to it by default. It is required to manually link a
 * surface to it with e.g. SetDCObject() before using it to draw or things will break!
 * @x_syscall_num `0x1008e`
 * @param source The source LCD descriptor.
 * @return The new LCD descriptor copied from the source descriptor.
 */
extern lcd_t *CreateCompatibleLCD(lcd_t *source);

/**
 * @brief Link a surface (device context) to an LCD descriptor.
 * @details
 * The drawing area lcd_t::drawing_area will also be reset to cover the entire surface.
 *
 * If the display is portrait as indicated by the lcd_t::rotation value, the LCD descriptor's width/height fields will
 * be updated to reflect the dimension shown to the user (i.e. swap width and height).
 *
 * @x_syscall_num `0x10093`
 * @param lcd The LCD descriptor.
 * @param new_surface The descriptor of the new surface to be linked to `lcd`.
 * @return The descriptor of the surface previously linked to `lcd`.
 */
extern lcd_surface_t *SetDCObject(lcd_t *lcd, lcd_surface_t *new_surface);

/**
 * @brief Calculate the buffer size required for an all-in-one surface of a specific size.
 * @details This assumes the surface will be in the same format as the surface linked to the current active LCD.
 * @x_syscall_num `0x10095`
 * @param width The width of the desired surface.
 * @param height The height of the desired surface.
 * @return The buffer size required to contain the entire surface.
 */
extern size_t GetImageSize(short width, short height);

/**
 * @brief Calculate the buffer size required for an all-in-one surface of a specific pixel format and size.
 * @details This is similar to GetImageSize() but it allows arbitrary pixel bit depth.
 * @x_syscall_num `0x10096`
 * @param width The width of the desired surface.
 * @param height The height of the desired surface.
 * @param depth The pixel bit depth of the desired surface.
 * @return The buffer size required to contain the entire surface.
 * @see InitGraphic For some assumptions made for an all-in-one surface format.
 */
extern size_t GetImageSizeExt(short width, short height, short depth);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_CANVAS_H__
