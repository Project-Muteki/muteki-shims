/*
 * Copyright 2024 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file common.h
 * @brief Common data structures and definitions for the UI subsystem.
 */

#ifndef __MUTEKI_UI_COMMON_H__
#define __MUTEKI_UI_COMMON_H__

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
 * @brief String alignment types.
 */
enum str_align_e {
    /**
     * @brief Align to top-left.
     */
    STR_ALIGN_LEFT = 0,
    /**
     * @brief Align to top-right.
     */
    STR_ALIGN_RIGHT,
    /**
     * @brief Align to top-center.
     */
    STR_ALIGN_CENTER,
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
 * @brief Rotation values used by lcd_rotate_callback_t.
 */
enum rotation_value_e {
    /** @brief Set the rotation so the top side of the canvas is facing up. */
    ROTATION_TOP_SIDE_FACING_UP = 0,
    /** @brief Set the rotation so the top side of the canvas is facing left. */
    ROTATION_TOP_SIDE_FACING_LEFT,
    /** @brief Set the rotation so the top side of the canvas is facing down. */
    ROTATION_TOP_SIDE_FACING_DOWN,
    /** @brief Set the rotation so the top side of the canvas is facing right. */
    ROTATION_TOP_SIDE_FACING_RIGHT,
    /** @brief Query the current rotation. Will not change the current location. */
    ROTATION_QUERY = 0xfe,
    /** @brief Actually rotate the buffer. Used internally. */
    ROTATION_APPLY = 0xff,
};

/**
 * @brief Predefined dash patterns.
 * @details The numbers here follows the SVG
 * [stroke-dasharray](https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-dasharray)
 * property format.
 * @todo Add graphic examples.
 */
enum stroke_predefined_dash_e {
    /**
     * @brief Solid line.
     */
    DASH_NONE = 0,
    DASH_8_8_16_0,
    DASH_4,
    DASH_0_4_16_4_4_4,
    DASH_12_4_4_4_4_4,
    DASH_1,
    DASH_2,
    DASH_16,
    DASH_0_1_8_1_8_1_8_1_4_0 = 12,
    DASH_4_1_8_1_7_1_9_1,
    DASH_3_1_7_1_7_1_4_8,
    DASH_3_1_7_1_7_1_7_1_4_0,
    DASH_0_1_6_1_1_1_4_1_3_1_2_1_5_2_3_0,
    DASH_3_2_5_1_2_1_3_1_4_1_1_1_6_1,
    DASH_7_1_6_1_6_1_6_1_3_0,
    DASH_3_1_6_1_6_1_6_1_7_0,
    DASH_24_8 = 22,
    /**
     * @brief Custom dash pattern.
     */
    DASH_CUSTOM = 0x100,
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
     * @brief The actual framebuffer/surface pixel buffer region.
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
 * @details The area is double-inclusive (i.e. `{0, 0, 479, 271}` represents an area of 480x272 px).
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
    /** @brief Dash bit pattern. */
    unsigned int stroke_dash_pattern; // 0x8:0xc (lcd_t[0x1c:0x20])
    /** @brief Size of stroke. */
    short stroke_size; // 0xc:0xe (lcd_t[0x20:0x22])
    /** @brief Predefined dash pattern. */
    short stroke_dash; // 0xe:0x10 (lcd_t[0x22:0x24])
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
 * @details When `rotation` is set to ::ROTATION_QUERY, the current rotation value will be returned
 * with no side effect. When `rotation` is set to ::ROTATION_APPLY, the canvas will rotate based on
 * current value of lcd_t::rotation.
 * @param self The LCD descriptor this was called from.
 * @param rotation New rotation. This value will be written to lcd_t::rotation and this callback will be called
 * recursively with ::ROTATION_APPLY as the rotation value to actually apply the change.
 * @return Current rotation value in effect. This will be the same as `rotation` when no magic value documented above
 * was used.
 * @see lcd_t::rotation Where the location value is stored in the LCD descriptor.
 * @see rotation_value_e Possible rotation values.
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

struct ui_widget_s;
typedef struct ui_widget_s ui_widget_t;

/**
 * @brief Common part of all widgets.
 * @todo Find a way to make the callbacks respect subtypes? Currently type-specific callbacks need to have their `self`
 * argument defined as `ui_widget_t *` (or `void *` if we opt to do that) which is a bit less ideal.
 */
struct ui_widget_s {
    /**
     * @brief Unknown. Set to 0.
     */
    int unk_0x0; // 0x0:0x4
    /**
     * @brief Next widget.
     * @todo Determine what is common and define ui_widget_t in ui/common.h.
     */
    ui_widget_t *next; // 0x4:0x8
    /**
     * @brief Unknown. Set to 0.
     */
    short unk_0x8; // 0x8:0xa
    /**
     * @brief Unknown. Set to 1.
     */
    short unk_0xa; // 0xa:0xc
    /**
     * @brief Button footprint rectangle.
     */
    lcd_rect_t footprint; // 0xc:0x14
    /**
     * @brief Unknown.
     */
    void (*unk_callback_0x14)(); // 0x14:0x18
    /**
     * @brief Unknown.
     */
    void (*unk_callback_0x18)(); // 0x18:0x1c
    /**
     * @brief Widget draw callback.
     */
    void (*on_draw)(ui_widget_t *self); // 0x1c:0x20
    /**
     * @brief Widget event callback.
     * @todo Verify.
     */
    void (*on_event)(ui_widget_t *self); // 0x20:0x24
    /**
     * @brief Unknown.
     */
    char unk_0x24[6]; // 0x24:0x2a;
    /**
     * @brief Unknown. Set to 0x65.
     */
    short unk_0x2a; // 0x2a:0x2c
    /**
     * @brief Unknown.
     */
    int unk_0x2c; // 0x2c:0x30
    /**
     * @brief Unknown. Initialized by AllocBlock().
     */
    short unk_0x30_arg1; // 0x30:0x32
    /**
     * @brief Unknown. Probably padding.
     */
    short unk_0x32; // 0x32:0x34
};

/**
 * @brief Convert separate RGB values to integer RGB representation
 * @param r Red value.
 * @param g Green value.
 * @param b Blue value.
 */
#define RGB_FROM_U8(r, g, b) ((r & 0xff) << 16 | ((g & 0xff) << 8) | (b & 0xff))

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_COMMON_H__
