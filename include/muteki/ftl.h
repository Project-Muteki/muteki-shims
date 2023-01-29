/*
 * Copyright 2022-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file ftl.h
 * @brief Flash Translation Layer (FTL) and block device API.
 */

#ifndef __MUTEKI_FTL_H__
#define __MUTEKI_FTL_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the size of block device the current active drive belongs to.
 *
 * When the size is >= 4GiB, the size is split into 2 parts and they must be joined together by using 
 * @code{.c}
 * size_t size_hi = 0;
 * size_t size_lo = FTL_GetCurDiskSize(&size_hi);
 * unsigned long long actual_size = (((unsigned long long) size_hi) << 32) | size_lo;
 * @endcode
 *
 * @param size_hi The high 32-bit of the size.
 * @return The low 32-bit of the size.
 */
extern size_t FTL_GetCurDiskSize(size_t *size_hi);

/**
 * @brief Read sectors from the block device the current active drive belongs to.
 *
 * Sector size seems to be constant 512 bytes.
 *
 * @param sector Start sector number.
 * @param ptr Target pointer. The size must be multiple of 512 bytes.
 * @param count How many sectors to read.
 * @return 0 if successful, negative if failed.
 */
extern int FTL_ReadSector(size_t sector, void *ptr, size_t count);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_FTL_H__
