#ifndef __MUTEKI_UI_COMMON_H__
#define __MUTEKI_UI_COMMON_H__

#include <muteki/common.h>

extern void rgbSetBkColor(int32_t rgb);
extern void rgbSetColor(int32_t rgb);
extern void ClearScreen(int32_t unk);
// TODO this does not work properly. Figure out why
extern void WriteAlignString(uint16_t x, uint16_t y, char *msg, uint16_t max_width);
extern int GetFontHeight(int32_t type);
extern void PrintfXY(uint16_t x, uint16_t y, char *format, ...);
extern void SetFontType(int32_t type);

#endif
