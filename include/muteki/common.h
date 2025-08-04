/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file common.h
 * @brief Common include file for all muteki-shims headers.
 * @details Includes several headers that provide standard C features used by each header (e.g. `stdint.h` for
 * `?int*_t` and `stdbool.h` for `bool` type).
 */

#ifndef __MUTEKI_COMMON_H__
#define __MUTEKI_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER) && _MSC_VER > 0
#error "MSVC is not yet supported."
#endif

#ifndef __GHIDRA__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#if (defined(__MUTEKI_IS_BOOTSTRAPPING__) && __MUTEKI_IS_BOOTSTRAPPING__ == 1)
typedef long ssize_t; 
#else
#include <sys/types.h> // for ssize_t
#endif

// This only supports GCC, LLVM and probably also MSVC at the moment.
#if defined(__WCHAR_TYPE__) && (__WCHAR_MAX__ < 0x10000)
typedef __WCHAR_TYPE__ __BESTA_UTF_TYPE;
#define __BESTA_UTF_LITERAL(x) (L ## x)
#elif defined(__CHAR16_TYPE__) && (__CHAR16_MAX__ < 0x10000)
typedef __CHAR16_TYPE__ __BESTA_UTF_TYPE;
#define __BESTA_UTF_LITERAL(x) (u ## x)
#elif defined(_MSC_VER) && _MSC_VER > 0
#include <wchar.h>
typedef wchar_t __BESTA_UTF_TYPE;
#define __BESTA_UTF_LITERAL(x) (L ## x)
#else
#error "Compiler does not support any 2 bytes wide string literal."
#endif

/**
 * @brief "Portable" Besta UTF-16 type used by other headers.
 * @details This is guaranteed to be 16-bit wide, unlike wchar_t (normally 32-bit wide) or char16_t (new C11 addition,
 * can be >16-bit depending on the toolchain).
 */
#define UTF16 __BESTA_UTF_TYPE

/**
 * @brief Besta UTF-16 string literal.
 * @details This will map to either u string or L string depending on availability of <b>compiler support</b> for
 * char16_t and wchar_t's size.
 */
#define _BUL(x) __BESTA_UTF_LITERAL(x)

#define __SYS_DWORD __attribute__((packed, aligned(4)))
#define __SYS_ALIGN(n) __attribute__((aligned(n)))
#define __SYS_IS_PRINTF(arg_index_str, arg_index_fmt) __attribute__((format(printf, arg_index_str, arg_index_fmt)))

/**
 * @brief Attribute for structure fields that are DWORD (64-bit), that are used by system calls.
 * @details
 * This will align the struct fields to 4 bytes so ldr/str can still work as normal while all 64-bit values
 * are retrieved with 2 ldr/str instructions instead of 1 ldrd/strd instruction (when compiling the program for armv5
 * and up) or emulated counterpart (when compiling the program for armv4). This is needed because Besta RTOS uses
 * 4-byte alignment for 64-bit fields by default and ldrd/strd will not work on those fields.
 *
 * For single variable this is not necessary since 8-byte aligned variables can be understood by code that uses 4-byte
 * alignment.
 *
 * @todo MSVC support (is it possible?)
 */
#define SYS_DWORD __SYS_DWORD

/**
 * @brief Attribute for forcing alignment.
 * @todo MSVC support
 */
#define SYS_ALIGN(n) __SYS_ALIGN(n)

/**
 * @brief Attribute for opting a function in for printf static checking.
 * @todo MSVC support (or simply bypass it).
 */
#define SYS_IS_PRINTF(arg_index_str, arg_index_fmt) __SYS_IS_PRINTF(arg_index_str, arg_index_fmt)

#else // defined(__GHIDRA__)

// Type stubs for Ghidra

// TODO get rid of these and find a way to let ghidra use system includes seamlessly
typedef long int32_t;
typedef uchar uint8_t;
typedef ulong size_t;
typedef ulong uintptr_t;

#define UTF16 wchar16
#define _BUL(x) x
#define SYS_DWORD
#define SYS_ALIGN(n)
#define SYS_IS_PRINTF(arg_index_str, arg_index_fmt)

#endif // defined(__GHIDRA__)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_COMMON_H__
