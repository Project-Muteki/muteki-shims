/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

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

// This only supports GCC, LLVM and probably also MSVC at the moment.
#if defined(__CHAR16_TYPE__) && (__CHAR16_MAX__ < 0x10000)
typedef __CHAR16_TYPE__ __BESTA_UTF_TYPE;
#define __BESTA_UTF_LITERAL(x) (u ## x)
#elif defined(__WCHAR_TYPE__) && (__WCHAR_MAX__ < 0x10000)
typedef __WCHAR_TYPE__ __BESTA_UTF_TYPE;
#define __BESTA_UTF_LITERAL(x) (L ## x)
#elif defined(_MSC_VER) && _MSC_VER > 0
#include <wchar.h>
typedef wchar_t __BESTA_UTF_TYPE;
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
 *
 * This will map to either u string or L string depending on availability of <b>compiler support</b> for char16_t and wchar_t's size.
 */
#define _BUL(x) __BESTA_UTF_LITERAL(x)

/**
 * @brief Attribute for structures that contain DWORD (64-bit) fields, that are used by system calls.
 *
 * This will align the struct fields to 4 bytes so ldr/str can still work as normal while all 64-bit values are retrieved with 2 ldr/str instructions instead of 1 ldrd/strd instruction (when compiling the program for armv5 and up) or emulated counterpart (when compiling the program for armv4). This is needed because Besta RTOS uses 4-byte alignment for 64-bit fields by default and ldrd/strd will not work on those fields.
 *
 * For single variable this is not necessary since 8-byte aligned variables can be understood by code that uses 4-byte alignment.
 */
#define SYS_DWORD __attribute__((packed, aligned(4)))

#endif // __MUTEKI_COMMON_H__
