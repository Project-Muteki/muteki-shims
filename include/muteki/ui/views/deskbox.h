/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file messagebox.h
 * @brief Functions related to the message box view.
 */

#ifndef __MUTEKI_UI_VIEWS_DESKBOX_H__
#define __MUTEKI_UI_VIEWS_DESKBOX_H__

#include <muteki/ui/common.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ui_view_deskbox_flag_e {
    VIEW_DESKBOX_NONE = 0x0000,
    VIEW_DESKBOX_CLOSE_BUTTON = 0x0001,
    VIEW_DESKBOX_DONE_BUTTON = 0x0002,
    VIEW_DESKBOX_SAVE_BUTTON = 0x0004,
    VIEW_DESKBOX_FUNCTION_MENU_BUTTON = 0x0010,
    VIEW_DESKBOX_STYLE_NORMAL = 0x0000,
    VIEW_DESKBOX_STYLE_NONE = 0x0100,
    VIEW_DESKBOX_STYLE_SIMPLE = 0x0200,
    VIEW_DESKBOX_STYLE_FLOAT = 0x0400,
    VIEW_DESKBOX_STYLE_POPUP = 0x0800,
    VIEW_DESKBOX_NO_CLOSE_BUTTON = 0x80000,
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_VIEWS_DESKBOX_H__
