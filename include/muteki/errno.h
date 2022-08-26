/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file errno.h
 * @brief Besta error code constants and functions.
 *
 */

#ifndef __MUTEKI_ERRNO_H__
#define __MUTEKI_ERRNO_H__

#include <muteki/common.h>

typedef int32_t kerrno_t;

enum message_flags_e {
    FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x100,
    FORMAT_MESSAGE_FROM_SYSTEM = 0x1000,
};

enum errno_ns_flag_e {
    ERRNO_NS_USER = 0x2,
    ERRNO_NS_KERNEL = 0x4,
    ERRNO_NS_EXEC = 0x8,
    ERRNO_NS_APPLET_SPECIFIC = 0x2000,
};

enum errno_exec_e {
    EXEC_UNSUPPORTED = 0x0001,
    EXEC_INVALID,
    EXEC_OPEN_FAILED,
    EXEC_LOADER_FAILURE,
    EXEC_MALLOC_FAILED,
    EXEC_DECOMPRESSION_FAILED,
    EXEC_MP3_PLAYER_IS_RUNNING,
    EXEC_UNKNOWN_FORMAT = 0x000a,
};

enum errno_kernel_e {
    PERIPHERAL_COMM_FAILED = 0x0003,
    GENERAL_HW_ERROR,

    FTL_DATA_CORRUPTED = 0x0010,
    FTL_ECC_FAILED,
    FTL_ECC_TRIGGERED = 0x0018,
    FTL_INVALID_LBA = 0x0021,
    MEDIUM_WP_ENABLED = 0x0027,
    MEDIUM_CHANGED,
    MEDIUM_INCOMPATIBLE = 0x0030,
    MEDIUM_ERROR,
    MEDIUM_UNLOADED = 0x003a,

    SYS_STORAGE_DEVICE_NOT_FOUND = 0x0060,
    SYS_ERASE_FAILED,
    SYS_OUT_OF_MEMORY,
    SYS_LOW_BATTERY,
    SYS_LOCK_SWITCH_ENABLED,

    MKFS_PROMPT = 0x0102,
    MKFS_UNSUPPORTED,
    MKFS_NO_SPACE_LEFT,
    MKFS_MODE_ERROR,
    MKFS_IO_ERROR,

    FS_INVALID_DRIVE_LETTER = 0x0113,

    FS_INVALID_FILENAME = 0x0140,
    FS_OPERATION_ERROR,
    FS_ENTRY_EXISTS,
    FS_DIR_FULL,
    FS_NO_SUCH_ENTRY,
    FS_FILE_UNAVAILABLE,
    FS_FILE_OOB_ACCESS,
    FS_CONFLICTING_ATTR,
    FS_TOO_MANY_OPEN_FILES,
    FS_FILE_LOCKED,
    FS_FILE_ATTR_ERROR,
    FS_NO_SPACE_LEFT,

    FS_NO_SUCH_ENTRY_ALT = 0x0154,
    FS_READ_ONLY_FILE = 0x0158,
    FS_PATH_TOO_LONG = 0x0162,
    FS_DIR_FULL_ALT1 = 0x0165,
    FS_DIR_FULL_ALT2,

    DB_CORRUPTED = 0x0200,
    DB_OPEN_FAILED,
    DB_INDEX_FULL_DELETE = 0x0203,
    DB_FULL,
    DB_INDEX_FULL_SYNC,
    DB_TOO_MANY_OPEN_FILES,
};

static inline unsigned short KERRNO_NS(kerrno_t kerrno) {
    return kerrno >> 16;
}

static inline unsigned short KERRNO_ERR(kerrno_t kerrno) {
    return kerrno & 0xffff;
}

/**
 * @brief Fetch an error message.
 *
 * Fetch error descriptions (encoded in @p UTF16) for last set error in a similar manner to FormatMessage API in Windows.
 *
 * @param flags Flags. Only @p FORMAT_MESSAGE_FROM_SYSTEM and @p FORMAT_MESSAGE_ALLOCATE_BUFFER are supported. In addition, @p FORMAT_MESSAGE_FROM_SYSTEM must be set.
 * @param _sbz0 Ignored. Should be set to 0.
 * @param _sbz1 Ignored. Should be set to 0.
 * @param _sbz2 Ignored. Should be set to 0.
 * @param outbuf The output buffer. If using @p FORMAT_MESSAGE_ALLOCATE_BUFFER, the pointer to the allocated buffer will be placed at this location. Otherwise the message will be written directly to this location. It's caller's responsibility to free any buffer allocated by @p FORMAT_MESSAGE_ALLOCATE_BUFFER.
 * @param outlen When not using @p FORMAT_MESSAGE_ALLOCATE_BUFFER, this is the length of the message buffer. Otherwise this is the minimum size of the buffer allocated for the message by the function.
 * @return Length of the message written, or 0 if fails.
 */
extern size_t FormatMessage(int32_t flags, int32_t _sbz0, int32_t _sbz1, int32_t _sbz2, void *outbuf, size_t outlen);

/**
 * @brief Set the global errno.
 *
 * @param err New errno value.
 */
extern void OSSetLastError(kerrno_t err);

/**
 * @brief Get errno value from the global errno variable.
 *
 * @return The current errno value.
 */
extern kerrno_t OSGetLastError(void);

/**
 * @brief Set errno.
 *
 * Unlike @p OSSetLastError, this also clears the kernel errno before calling @p OSSetLastError.
 *
 * If the errno namespace is unset, it will also automatically set @p ERRNO_NS_KERNEL_OVERRIDE_DEFAULT before passing it to @p OSSetLastError.
 *
 * Requires @p -lkrnllib when dynamically linking with the shims.
 *
 * @param err New errno value.
 */
extern void _SetLastError(kerrno_t err);

/**
 * @brief Get errno.
 *
 * If kernel errno is set, the errno is returned with @p ERRNO_NS_KERNEL bit set. Otherwise it will return the result from @p OSSetLastError with @p ERRNO_NS_USER bit set.
 *
 * Requires @p -lkrnllib when dynamically linking with the shims.
 *
 * @return The current errno value from either kernel or @p OSSetLastError.
 */
extern kerrno_t _GetLastError(void);

#endif
