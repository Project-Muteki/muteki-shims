/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file button.h
 * @brief Button widget API.
 */

#ifndef __MUTEKI_UI_WIDGETS_BUTTON_H__
#define __MUTEKI_UI_WIDGETS_BUTTON_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ui_button_s;
typedef ui_button_s ui_button_t;

struct ui_button_s {
    ui_widget_t widget; // 0x0:0x34
    /**
     * @brief Unknown.
     */
    void (*unk_callback_0x34)(); // 0x34:0x38
    /**
     * @brief Unknown.
     */
    void (*unk_callback_0x38)(); // 0x38:0x3c
    /**
     * @brief Key binding.
     * @details Setting this to 0 disables key binding.
     * @see keycode_e
     */
    int key_binding; // 0x3c:0x40
    /**
     * @brief Set to arg7 of CreateButton().
     */
    int unk_0x40_arg7; // 0x40:0x44
    /**
     * @brief Button label text.
     */
    const UTF16 *label; // 0x44:0x48
    /**
     * @brief Unknown. Set to 0.
     */
    int unk_0x48; // 0x48:0x4c
    /**
     * @brief Unknown.
     */
    int unk_0x4c; // 0x4c:0x50
    /**
     * @brief Unknown. Set to 10.
     */
    int unk_0x50; // 0x50:0x54
    /**
     * @brief Unknown. Set to 0.
     */
    int unk_0x54; // 0x54:0x58
    /**
     * @brief Unknown. Set to 4.
     */
    short unk_0x58; // 0x58:0x5a
    /**
     * @brief Unknown. Set to arg8 of CreateButton().
     */
    short unk_0x5a_arg8; // 0x5a:0x5c
    /**
     * @brief Unknown.
     */
    int unk_0x5c; // 0x5c:0x60
}; // 0x60 bytes

/**
 * @brief Create a button widget object.
 * @todo Need some ways to test this.
 * @param arg1 Unknown. Writes to offset 0x30 of the target object.
 * @param x0 @x_term x0
 * @param y0 @x_term y0
 * @param x1 @x_term x1-auto
 * @param y1 @x_term y1-auto
 * @param label UTF-16 encoded string to be used as a label.
 * @param arg7 Unknown.
 * @param flags Flags. Valid options seem to be 2, 4, 0x40.
 * @return The allocated object.
 */
void *CreateButton(
    unsigned short arg1,
    short x0,
    short y0,
    short x1,
    short y1,
    UTF16 *label,
    int arg7,
    short flags
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_WIDGETS_BUTTON_H__
