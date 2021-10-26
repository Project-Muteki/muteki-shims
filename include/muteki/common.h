/**
 * @file common.h
 * @brief Common include file for all muteki-shims headers.
 *
 * Includes several headers that provide standard C features used by each header (e.g. stdint for ?int*_t and stdbool for bool type).
 */

#ifndef __MUTEKI_COMMON_H__
#define __MUTEKI_COMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief "Portable" UTF-16 type used by other headers.
 *
 * This is guaranteed to be 16-bit wide, unlike wchar_t (normally 32-bit wide) or char16_t (new C11 addition, can be >16-bit depending on the toolchain).
 */
typedef uint16_t UTF16;

#endif // __MUTEKI_COMMON_H__
