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

#ifdef UCHAR_H_PROVIDED
#include <uchar.h>
#else
typedef uint16_t char16_t;
#endif

#endif // __MUTEKI_COMMON_H__
