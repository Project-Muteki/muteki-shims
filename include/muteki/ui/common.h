#ifndef __MUTEKI_UI_COMMON_H__
#define __MUTEKI_UI_COMMON_H__

#include <muteki/common.h>

// TODO verify this on a color screen device
#define RGB_FROM_U8(r, g, b) ((r & 0xff) << 16 | ((g & 0xff) << 8) | (b & 0xff))

extern void rgbSetBkColor(int rgb);
extern void rgbSetColor(int rgb);
extern void ClearScreen(bool fill_with_fg);
// TODO this does not work properly. Figure out why
extern void WriteAlignString(short x, short y, const char *msg, uint16_t max_width);
//extern int GetFontWidth(int32_t type);
extern short GetCharWidth(char16_t c, uint8_t font_type);
extern int GetFontHeight(uint8_t font_type);
extern void PrintfXY(short x, short y, const char *format, ...);
extern void SetFontType(int8_t type);
extern void WriteChar(short x, short y, char16_t c, bool invert);
extern void WriteString(short x, short y, const char16_t *s, bool invert);

extern uint16_t GetMaxScrX();
extern uint16_t GetMaxScrY();

void ScrollUp(short x0, short y0, short x1, short y1, short amount);
void ScrollDown(short x0, short y0, short x1, short y1, short amount);
void ScrollLeft(short x0, short y0, short x1, short y1, short amount);
void ScrollRight(short x0, short y0, short x1, short y1, short amount);

#endif // __MUTEKI_UI_COMMON_H__
