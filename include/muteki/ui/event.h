/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file event.h
 * @brief Low-level UI event handing API.
 */

#ifndef __MUTEKI_UI_EVENT_H__
#define __MUTEKI_UI_EVENT_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief List of available keycodes.
 */
enum keycode_e {
    /* 0x00 */
    KEY_ESC = 0x01,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_PGUP,
    KEY_PGDN,
    KEY_CAPS = 0x0a,
    KEY_DEL = 0xc,
    KEY_ENTER = 0x0d,
    /* 0x10 */
    KEY_MENU = 0x11,
    KEY_FONT = 0x12,
    /* 0x20 */
    KEY_SPACE = ' ',
    KEY_EXCL = '!',
    KEY_TAB = 0x22,
    KEY_HASH = '#',
    KEY_DOLLAR = '$',
    KEY_PERCENT = '%',
    KEY_LPAREN = '(',
    KEY_RPAREN = ')',
    KEY_STAR = '*',
    KEY_COMMA = ',',
    KEY_DASH = '-',
    KEY_DOT = '.',
    /* 0x30 */
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    KEY_QUESTION = '?',
    /* 0x40 */
    KEY_AT = '@',
    KEY_A = 'A',
    KEY_B = 'B',
    KEY_C = 'C',
    KEY_D = 'D',
    KEY_E = 'E',
    KEY_F = 'F',
    KEY_G = 'G',
    KEY_H = 'H',
    KEY_I = 'I',
    KEY_J = 'J',
    KEY_K = 'K',
    KEY_L = 'L',
    KEY_M = 'M',
    KEY_N = 'N',
    KEY_O = 'O',
    /* 0x50 */
    KEY_P = 'P',
    KEY_Q = 'Q',
    KEY_R = 'R',
    KEY_S = 'S',
    KEY_T = 'T',
    KEY_U = 'U',
    KEY_V = 'V',
    KEY_W = 'W',
    KEY_X = 'X',
    KEY_Y = 'Y',
    KEY_Z = 'Z',
    /* 0x80 */
    KEY_LANG_CHN = 0x80,
    KEY_LANG_ENG = 0x82,
    KEY_POWER = 0x83,
    KEY_F1 = 0x84,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_SHIFT = 0x8b,
    KEY_IME = 0x8e,
    /* 0x90 */
    KEY_SYMBOL = 0x91,
    KEY_HOME = 0x93,
    KEY_HELP = 0x95,
    KEY_SAVE = 0x96,
    KEY_SYLLABLE = 0x9e,
    /* 0xf0 */
    KEY_SEARCH = 0xf6,
    KEY_BACKLIGHT = 0xf8,
    KEY_VOL = 0xfa,
    KEY_USB_INSERTION = 0xfe,
    KEY_REPEAT = 0xff,
    /* 0x100+ */
    KEY_INS = 0x101,
    KEY_SHIFT_PGUP = 0xe047,
    KEY_SHIFT_PGDN = 0xe04f,
};

/**
 * @brief UI event types.
 */
enum ui_event_type_e {
    /**
     * @brief Invalid/cleared.
     */
    UI_EVENT_TYPE_INVALID = 0,
    /**
     * @brief Beginning of touch/pen down event.
     */
    UI_EVENT_TYPE_TOUCH_BEGIN = 1,
    /**
     * @brief Touch/pen move event.
     */
    UI_EVENT_TYPE_TOUCH_MOVE = 2,
    /**
     * @brief End of touch/pen up event.
     */
    UI_EVENT_TYPE_TOUCH_END = 8,
    /**
     * @brief Key(s) pressed.
     */
    UI_EVENT_TYPE_KEY = 16,
};

/**
 * @brief Structure for low level UI events
 */
typedef struct {
    /**
     * @brief Unknown.
     * @details Maybe used on event types other than touch and key press.
     */
    void *unk0; // 0-4 sometimes a pointer especially on unstable USB connection? junk data?
    /**
     * @brief The type of event (0x10 being key event)
     * @see ui_event_type_e List of event types.
     */
    int event_type; // 4-8 16: key (?).
    union {
        struct {
            /** 
             * @brief Keycode for the first pressed key.
             * @details Only available when ::event_type is ui_event_type_e::UI_EVENT_TYPE_KEY.
             */
            short key_code0; // 8-10
            /**
             * @brief Keycode for the second pressed key.
             * @details Only available when ::event_type is ui_event_type_e::UI_EVENT_TYPE_KEY.
             * @note Depending on the exact keys pressed simultaneously, this is not always accurate. Moreover,
             * some devices may lack support of simultaneous key presses.
             */
            short key_code1; // 10-12 sometimes set when 2 keys are pressed simultaneously. Does not always work.
        };
        struct {
            /**
             * @brief The X coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ui_event_type_e::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ui_event_type_e::UI_EVENT_TYPE_TOUCH_MOVE, or ui_event_type_e::UI_EVENT_TYPE_TOUCH_END.
             */
            short touch_x;
            /**
             * @brief The Y coordinate of where the touch event is located, in pixels.
             * @details Only available when ::event_type is ui_event_type_e::UI_EVENT_TYPE_TOUCH_BEGIN,
             * ui_event_type_e::UI_EVENT_TYPE_TOUCH_MOVE, or ui_event_type_e::UI_EVENT_TYPE_TOUCH_END.
             */
            short touch_y;
        };
    };
    /**
     * @brief Unknown.
     * @details Set along with a ::KEY_USB_INSERTION event. Seems to point to some data. Exact purpose unknown.
     */
    void *usb_data; // 12-16 pointer that only shows up on USB insertion event.
    /**
     * @brief Unknown.
     * @details Maybe used on event types other than touch and key press.
     */
    void *unk16; // 16-20 sometimes a pointer especially on unstable USB connection? junk data?
    /**
     * @brief Unknown.
     * @details Seems to be always 0, although ClearEvent() explicitly sets this to 0. Maybe used on event types other
     * than touch and key press.
     */
    void *unk20; // 20-24 seems to be always 0. Unused?
} ui_event_t;

/**
 * @brief Configuration struct for key press event generator.
 * @details
 * The generator is responsible for detecting long key presses and sending repeated "pressed" events when such long
 * presses are detected.
 *
 * The intervals below have the resolution of 25ms per step and the actual millisecond value needs to be calculated by
 * using `milliseconds = 25ms * (interval + 1)`. That is, setting the interval to 0 will give you 25ms of delay,
 * setting it to 1 will give you 50ms, and so forth.
 * 
 * The `beep_on_press` value sets the system configuration variable `0x10` (SYS_CONFIG_BEEP_ON_EVENT) that will reflect
 * in the settings app. This is, however, temporary and will reset when the user reboots the system.
 */
typedef struct {
    /**
     * @brief Delay interval before triggering long press repeat.
     */
    unsigned short long_press_delay;
    /**
     * @brief Repeat rate when long press repeat is triggered.
     */
    unsigned short long_press_repeat_rate;
    /**
     * @brief Whether or not to beep on key/touchscreen presses.
     */
    unsigned short beep_on_press;
} key_press_event_config_t;

/**
 * @brief Read current configuration of key event generator.
 * @x_syscall_num 0x10032
 * @param[out] config The configuration struct for holding the exported configuration.
 * @x_void_return
 * @see key_press_event_config_t The config struct and details on the expected format.
 */
void GetSysKeyState(key_press_event_config_t *config);

/**
 * @brief Configure key event generator.
 * @x_syscall_num 0x10033
 * @param[in] config The configuration struct to be imported.
 * @x_void_return
 * @see key_press_event_config_t The config struct and details on the expected format.
 */
void SetSysKeyState(const key_press_event_config_t *config);

/**
 * @brief Discard all unprocessed UI events.
 * @x_syscall_num `0x1004a`
 * @x_void
 */
extern void ClearAllEvents();

/**
 * @brief Process pending events.
 * @details Exact purpose of this function is currently unclear.
 * @x_syscall_num `0x10046`
 * @param event pointer to a ::ui_event_t struct.
 * @retval true Some events were processed.
 * @retval false No event was processed.
 * @see TestKeyEvent Related function that specifically processes key events.
 */
extern bool TestPendEvent(ui_event_t *event);

/**
 * @brief Process pending key events.
 * @details Exact purpose of this function is currently unclear. It is at least responsible for beeping on key presses,
 * etc.
 * @x_syscall_num `0x1004b`
 * @param event pointer to a ::ui_event_t struct.
 * @retval true Some events were processed.
 * @retval false No event was processed.
 * @see TestPendEvent
 */
extern bool TestKeyEvent(ui_event_t *event);

/**
 * @brief Get event.
 * @details Exact purpose of this function is currently unclear. It may be responsible for setting some fields in
 * the ::ui_event_t struct.
 * @note This function will block when there is currently no event to be processed.
 * @x_syscall_num `0x1003f`
 * @param[out] event Pointer to a ::ui_event_t struct.
 * @retval true Some events were returned.
 * @retval false No event was returned.
 */
extern bool GetEvent(ui_event_t *event);

/**
 * @brief Invalidate an event struct
 * @details This function zeros out ui_event_t::event_type and ui_event_t::unk20.
 * @x_syscall_num `0x10049`
 * @param[out] event Pointer to a ::ui_event_t struct.
 * @x_void_return
 */
void ClearEvent(ui_event_t *event);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UI_EVENT_H__
