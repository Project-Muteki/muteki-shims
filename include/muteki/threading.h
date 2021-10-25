/**
 * @file threading.h
 * @brief Native threading API.
 */

#ifndef __MUTEKI_THREADING_H__
#define __MUTEKI_THREADING_H__

#include <muteki/common.h>

/**
 * @brief Sleep for \p millis milliseconds.
 *
 * @param millis Time to sleep in milliseconds.
 */
extern void OSSleep(short millis);

#endif // __MUTEKI_THREADING_H__
