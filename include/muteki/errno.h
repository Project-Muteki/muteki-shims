/**
 * @file errno.h
 * @brief Besta error code constants and functions.
 *
 */

#ifndef __MUTEKI_ERRNO_H__
#define __MUTEKI_ERRNO_H__

#include <muteki/common.h>

typedef int32_t muteki_errno_t;

enum message_flags_e {
    FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x100,
    FORMAT_MESSAGE_FROM_SYSTEM = 0x1000,
};

enum errno_exec_e {
    EXEC_UNSUPPORTED = 0x0001,
    EXEC_INVALID,
    EXEC_OPEN_FAILED,
    EXEC_LOADER_FAILURE,
    EXEC_MALLOC_FAILED,
    EXEC_DECOMPRESSION_FAILED,
    EXEC_MP3_PLAYER_IS_RUNNING,
};

enum errno_general_e {
    PERIPHERAL_COMM_FAILED = 0x0003,
    GENERAL_HW_ERROR,
    DATA_CORRUPTED = 0x0010,
    ECC_FAILED,
    ECC_TRIGGERED = 0x0018,
    INVALID_LBA = 0x0021,
    MEDIUM_WP_ENABLED = 0x0027,
    MEDIUM_CHANGED = 0x0028,
    MEDIUM_INCOMPATIBLE = 0x0030,
    MEDIUM_ERROR = 0x0031,
    NO_MEDIUM = 0x003a,
    STORAGE_DEVICE_NOT_FOUND = 0x0060,
    ERASE_FAILED = 0x0061,
    OUT_OF_MEMORY = 0x0062,
    LOW_BATTERY = 0x0063,
    LOCK_SWITCH_ENABLED = 0x0064,
    
    MEDIUM_FORMAT_PROMPT = 0x0102,
    MEDIUM_FORMAT_UNSUPPORTED,
    NO_SPACE,
    FS_ERROR,
    FS_CORRUPTED,
    INVALID_DRIVE_LETTER = 0x0113,
    INVALID_FILENAME = 0x0140,
    FILE_OPERATION_ERROR,
    FILE_EXISTS,
    DIR_FULL,
    NO_SUCH_ENTRY,
    FILE_UNAVAILABLE,
    FILE_OOB_ACCESS,
    CONFLICTING_ATTR,
    TOO_MANY_OPEN_FILES,
    FILE_LOCKED,
    FILE_ATTR_ERROR,
    NO_SPACE_LEFT,
    NO_SUCH_ENTRY_ALT = 0x0154,
    READ_ONLY_FILE = 0x0158,
    PATH_TOO_LONG = 0x0162,
    DIR_FULL_ALT1 = 0x0165,
    DIR_FULL_ALT2 = 0x0166,
    DB_CORRUPTED = 0x0200,
    DB_OPEN_FAILED,
    DB_INDEX_FULL_DELETE = 0x0203,
    DB_FULL,
    DB_INDEX_FULL_SYNC,
    TOO_MANY_OPEN_FILES_ALT,
};

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
size_t FormatMessage(int32_t flags, int32_t _sbz0, int32_t _sbz1, int32_t _sbz2, void *outbuf, size_t outlen);

void OSSetLastError(muteki_errno_t err);
muteki_errno_t OSGetLastError(void);

void _SetLastError(muteki_errno_t err);
muteki_errno_t _GetLastError(void);

#endif
