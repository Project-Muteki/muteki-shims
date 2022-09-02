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

/** Kernel error code type */
typedef int32_t kerrno_t;

/**
 * @brief Flags for FormatMessage()
 * @see https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessage
 */
enum message_flags_e {
    /** Allocate buffer for the message. */
    FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x100,
    /** Use system error message string table to generate the message. */
    FORMAT_MESSAGE_FROM_SYSTEM = 0x1000,
};

/**
 * @brief Kernel errno namespace.
 */
enum errno_ns_flag_e {
    /** User (errors set by OSSetLastError()). */
    ERRNO_NS_USER = 0x2,
    /** Kernel. */
    ERRNO_NS_KERNEL = 0x4,
    /** Executable loader. */
    ERRNO_NS_EXEC = 0x8,
    /** Applet specific (errors set by _SetLastError()). */
    ERRNO_NS_APPLET_SPECIFIC = 0x2000,
};

/**
 * @brief Error code for ::ERRNO_NS_EXEC.
 */
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

/**
 * @brief Error code for ::ERRNO_NS_KERNEL.
 */
enum errno_kernel_e {
    /** Peripheral communication failed. */
    PERIPHERAL_COMM_FAILED = 0x0003,
    /** General hardware error. */
    GENERAL_HW_ERROR,

    /** FTL: Data corruption detected. */
    FTL_DATA_CORRUPTED = 0x0010,
    /** FTL: Data corruption detected and error correction attempt failed. */
    FTL_ECC_FAILED,
    /** FTL: Data corruption detected and error correction attempt succeeded. */
    FTL_ECC_TRIGGERED = 0x0018,
    /** FTL: Invalid Logical Block Address. */
    FTL_INVALID_LBA = 0x0021,
    /** Medium is write protected. */
    MEDIUM_WP_ENABLED = 0x0027,
    /** Medium changed */
    MEDIUM_CHANGED,
    /** Medium is of an incompatible type. */
    MEDIUM_INCOMPATIBLE = 0x0030,
    /** Medium I/O error. */
    MEDIUM_ERROR,
    /** Medium is not loaded. */
    MEDIUM_UNLOADED = 0x003a,

    /** Storage device not found. */
    SYS_STORAGE_DEVICE_NOT_FOUND = 0x0060,
    /** Erase of storage device failed. */
    SYS_ERASE_FAILED,
    /** Out of memory. */
    SYS_OUT_OF_MEMORY,
    /** Low battery. */
    SYS_LOW_BATTERY,
    /** Lock switch is on. System is locked. */
    SYS_LOCK_SWITCH_ENABLED,

    /** Block device format prompt. */
    MKFS_PROMPT = 0x0102,
    /** Block device needs formatting. */
    MKFS_UNSUPPORTED,
    /** mkfs: No space left for device. */
    MKFS_NO_SPACE_LEFT,
    /** mkfs: Mode error. */
    MKFS_MODE_ERROR,
    /** mkfs: I/O error. */
    MKFS_IO_ERROR,

    /** Invalid drive letter */
    FS_INVALID_DRIVE_LETTER = 0x0113,

    /** Filename contains invalid characters. */
    FS_INVALID_FILENAME = 0x0140,
    /** General filesystem I/O error. */
    FS_OPERATION_ERROR,
    /** File or directory exists. */
    FS_ENTRY_EXISTS,
    /** Too many files in this directory. */
    FS_DIR_FULL,
    /** No such file or directory. */
    FS_NO_SUCH_ENTRY,
    /** File/directory is not available. */
    FS_FILE_UNAVAILABLE,
    /** Accessing file out of bound. */
    FS_FILE_OOB_ACCESS,
    /** Conflicting file/directory attributes */
    FS_CONFLICTING_ATTR,
    /** Too many open files. */
    FS_TOO_MANY_OPEN_FILES,
    /** File/directory is locked for exclusive access. */
    FS_FILE_LOCKED,
    /** Attribute error. */
    FS_FILE_ATTR_ERROR,
    /** No space left for device. */
    FS_NO_SPACE_LEFT,

    /** No such file or directory (alternative). */
    FS_NO_SUCH_ENTRY_ALT = 0x0154,
    /** File is read-only. */
    FS_READ_ONLY_FILE = 0x0158,
    /** Path too long. */
    FS_PATH_TOO_LONG = 0x0162,
    /** Too many files in this directory (alternative). */
    FS_DIR_FULL_ALT1 = 0x0165,
    /** Too many files in this directory (alternative). */
    FS_DIR_FULL_ALT2,

    /** Database is corrupted. */
    DB_CORRUPTED = 0x0200,
    /** Failed to open database. */
    DB_OPEN_FAILED,
    /** Database index is full. Suggesting entry cleanup. */
    DB_INDEX_FULL_DELETE = 0x0203,
    /** Database is full. */
    DB_FULL,
    /** Database index is full. Suggesting sync with PC. */
    DB_INDEX_FULL_SYNC,
    /** Database: too many open files. */
    DB_TOO_MANY_OPEN_FILES,
};

/**
 * @brief Get the namespace portion of raw kernel errno value.
 * @param kerrno Kernel errno value.
 * @return The namespace.
 * @see errno_ns_flag_e
 */
static inline unsigned short KERRNO_NS(kerrno_t kerrno) {
    return kerrno >> 16;
}

/**
 * @brief Get the error code portion of raw kernel errno value.
 * @param kerrno Kernel errno value.
 * @return The error code.
 * @see errno_exec_e
 * @see errno_kernel_e
 */
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
 * Unlike OSSetLastError(), this also clears the kernel errno before calling OSSetLastError().
 *
 * If the errno namespace is unset, it will also automatically set ::ERRNO_NS_APPLET_SPECIFIC before passing it to
 * OSSetLastError().
 *
 * Requires @p -lkrnllib when dynamically linking with the shims.
 *
 * @param err New errno value.
 */
extern void _SetLastError(kerrno_t err);

/**
 * @brief Get errno.
 *
 * If kernel errno is set, the errno is returned with ::ERRNO_NS_KERNEL bit set. Otherwise it will return the result from OSSetLastError() with ::ERRNO_NS_USER bit set.
 *
 * Requires @p -lkrnllib when dynamically linking with the shims.
 *
 * @return The current errno value from either kernel or @p OSSetLastError.
 */
extern kerrno_t _GetLastError(void);

#endif
