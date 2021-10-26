/**
 * @file event.h
 * @brief Low-level UI event handing API.
 */

#ifndef __MUTEKI_UI_EVENT_H__
#define __MUTEKI_UI_EVENT_H__

#include <muteki/common.h>

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
    KEY_F1 = 0x84,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_SHIFT = 0x8b,
    KEY_IME = 0x8e,
    /* 0x91 */
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
 * @brief Structure for low level UI events
 */
typedef struct {
    /**
     * Purpose unknown.
     */
    void *unk0; // 0-4 sometimes a pointer especially on unstable USB connection? junk data?
    /**
     * Seems to be the type of event (0x10 being key event)
     */
    int event_type; // 4-8 16: key (?).
    /**
     * Keycode for the first pressed key.
     */
    short key_code0; // 8-10
    /**
     * Keycode for the second pressed key.
     *
     * Note that depending on the exact keys pressed simultaneously, this is not always accurate.
     */
    short key_code1; // 10-12 sometimes set when 2 keys are pressed simultaneously. Does not always work.
    /**
     * Set along with a @p KEY_USB_INSERTION event. Seems to point to some data. Exact purpose unknown.
     */
    void *usb_data; // 12-16 pointer that only shows up on USB insertion event.
    /**
     * Purpose unknown.
     */
    void *unk16; // 16-20 sometimes a pointer especially on unstable USB connection? junk data?
    /**
     * Purpose unknown.
     */
    void *unk20; // 20-24 seems to be always 0. Unused?
} ui_event_t;

/**
 * @brief Discard all unprocessed UI events.
 */
extern short ClearAllEvents();

/**
 * @brief Process pending events.
 *
 * Exact purpose of this function is currently unclear.
 *
 * @param event pointer to a @p ui_event_t struct.
 * @return @p true if events were processed, @p false otherwise.
 * @see TestKeyEvent Related function that specifically processes key events.
 */
extern bool TestPendEvent(ui_event_t *event);

/**
 * @brief Process pending key events.
 *
 * Exact purpose of this function is currently unclear. It is at least responsible for beeping on key presses, etc.
 *
 * @param event pointer to a @p ui_event_t struct.
 * @return @p true if events were processed, @p false otherwise.
 * @see TestPendEvent
 */
extern bool TestKeyEvent(ui_event_t *event);

/**
 * @brief Get event.
 *
 * Exact purpose of this function is currently unclear. It may be responsible for setting some fields in the @p ui_event_t struct.
 *
 * @param event pointer to a @p ui_event_t struct.
 * @return @p 1 if successful.
 */
extern int32_t GetEvent(ui_event_t *event);

#endif // __MUTEKI_UI_EVENT_H__
