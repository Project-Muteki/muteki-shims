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

// This only supports GCC and LLVM at the moment.
#if defined(__CHAR16_TYPE__) && (__CHAR16_MAX__ < 0x10000)
typedef __CHAR16_TYPE__ __BESTA_UTF_TYPE;
#define __BESTA_UTF_LITERAL(x) (u ## x)
#elif defined(__WCHAR_TYPE__) && (__WCHAR_MAX__ < 0x10000)
typedef __WCHAR_TYPE__ __BESTA_UTF_TYPE;
#define __BESTA_UTF_LITERAL(x) (L ## x)
#else
#error "Compiler does not support any 2 bytes wide string literal."
#endif

/**
 * @brief "Portable" Besta UTF-16 type used by other headers.
 *
 * This is guaranteed to be 16-bit wide, unlike wchar_t (normally 32-bit wide) or char16_t (new C11 addition, can be >16-bit depending on the toolchain).
 */
#define UTF16 __BESTA_UTF_TYPE

/**
 * @brief Besta UTF-16 string literal.
 */
#define _BUL(x) __BESTA_UTF_LITERAL(x)

#endif // __MUTEKI_COMMON_H__
