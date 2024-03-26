/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file messagebox.h
 * @brief Functions related to the message box view.
 */

#ifndef __MUTEKI_UI_VIEWS_MESSAGEBOX_H__
#define __MUTEKI_UI_VIEWS_MESSAGEBOX_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Message box types.
 * @details
 * A valid message box type is either `MB_DEFAULT` or a `MB_ICON_` enum joined by one or more `MB_BUTTON_` enums
 * with the bitwise-or operator (`|`). For example,
 * @code{.c}
 * MessageBox(_BUL("hello world"), MB_ICON_WARNING | MB_BUTTON_YES | MB_BUTTON_NO);
 * @endcode
 * will cause a message box with the message `"hello world"`, the exclamation-point icon, and both the Yes and No
 * buttons to be shown.
 */
enum message_box_type_e {
    /**
     * @brief The default type.
     * @details This is usually equivalent to `(MB_ICON_WARNING | MB_BUTTON_OK)`.
     */
    MB_DEFAULT = 0,
    /**
     * @brief Display an error (stop-sign) icon on the message box.
     */
    MB_ICON_ERROR,
    /**
     * @brief Display an info icon (lower case `i`) on the message box.
     */
    MB_ICON_INFO,
    /**
     * @brief Display a warning (exclamation-point) icon on the message box.
     */
    MB_ICON_WARNING,
    /**
     * @brief Display a question-mark icon on the message box.
     */
    MB_ICON_QUESTION,
    /**
     * @brief Provide `Yes` button on the message box.
     */
    MB_BUTTON_YES = 1 << 8,
    /**
     * @brief Provide `No` button on the message box.
     */
    MB_BUTTON_NO = 2 << 8,
    /**
     * @brief Provide `OK` button on the message box.
     */
    MB_BUTTON_OK = 4 << 8,
    /**
     * @brief Provide `Cancel` button on the message box.
     */
    MB_BUTTON_CANCEL = 8 << 8,
    /**
     * @brief Do not show any button.
     * @details This will hide all buttons on the message box. Any key press will dismiss the message box and the
     * return value will always be set to message_box_result_t::MB_RESULT_OK.
     */
    MB_BUTTON_NONE = 16 << 8,
};

/**
 * @brief Results returned by the MessageBox() function after the user dismisses the message box.
 */
enum message_box_result_t {
    /**
     * @brief The OK button was pressed by the user.
     * @details Pressing `Y` key on the keyboard could also trigger this (specifically when OK button is enabled via
     * message_box_type_e::MB_BUTTON_OK).
     */
    MB_RESULT_OK = 3,
    /**
     * @brief The Cancel button was pressed by the user.
     * @details Pressing `C` key or `ESC` key on the keyboard could also trigger this.
     */
    MB_RESULT_CANCEL = 4,
    /**
     * @brief The Yes button was pressed by the user.
     * @details Pressing `Y` key on the keyboard could also trigger this (specifically when Yes button is enabled via
     * message_box_type_e::MB_BUTTON_YES).
     */
    MB_RESULT_YES = 5,
    /**
     * @brief The No button was pressed by the user.
     * @details Pressing `N` key or `ESC` key on the keyboard could also trigger this.
     */
    MB_RESULT_NO = 6,
};

/**
 * @brief High level message box function.
 * @details Create and show a message box view with the specified `text`, an icon and one or more buttons, and return
 * which button on the message box is pressed when the user dismisses it. The `type` argument controls the icon and
 * buttons shown on the message box view.
 * @param text UTF-16 encoded text to be displayed on the message box.
 * @param type The message box type.
 * @return Which button is pressed to dismiss the message box.
 * @see message_box_type_e Valid message box types.
 * @see message_box_result_t Valid return values of this function.
 */
int MessageBox(UTF16 *text, unsigned short type);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_VIEWS_MESSAGEBOX_H__
