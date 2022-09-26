/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file file.h
 * @brief Low-level and built-in high-level file operation API.
 */

#ifndef __MUTEKI_FILE_H__
#define __MUTEKI_FILE_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief @p whence values for __fseek().
 */
enum sys_seek_whence_e {
    /** Seek from the beginning of file. */
    _SYS_SEEK_SET = 0,
    /** Seek from current offset. */
    _SYS_SEEK_CUR,
    /** Seek from the end of file. */
    _SYS_SEEK_END,
};

/**
 * @brief Open a file located at @p pathname.
 *
 * Analogous to the fopen() function in POSIX and Windows.
 *
 * @param pathname DOS 8.3 path to the file to be opened.
 * @param mode Mode. Tested modes are @p "rb" and @p "wb+".
 * @return Pointer reference to the opened file.
 */
extern void *_afopen(const char *pathname, const char *mode);

/**
 * @brief Open a file located at UTF-16-encoded @p pathname.
 *
 * Analogous to the _wfopen() function in Windows.
 *
 * @param pathname Path to the file to be opened. LFN (long filename) is supported.
 * @param mode Mode. Tested modes are @p _BUL("rb") and @p _BUL("wb+").
 * @return Pointer reference to the opened file.
 */
extern void *__wfopen(const UTF16 *pathname, const UTF16 *mode);

/**
 * @brief Read @p nmemb data units of size @p size from a file.
 *
 * Analogous to the POSIX fread() function.
 *
 * @param ptr Pointer reference to a buffer that will hold the data read from the file.
 * @param size Size of individual data unit.
 * @param nmemb Number of data units to read.
 * @param stream Pointer reference returned by _afopen() or _wfopen().
 * @return Number of data units read.
 */
extern size_t _fread(void *ptr, size_t size, size_t nmemb, void *stream);

/**
 * @brief Write @p nmemb data units of size @p size to a file.
 *
 * Analogous to the fwrite() function in POSIX.
 *
 * @param ptr Pointer reference to data that will be written to the file.
 * @param size Size of individual data unit.
 * @param nmemb Number of data units to write.
 * @param stream Pointer reference returned by _afopen() or _wfopen().
 * @return Number of data units written.
 */
extern size_t _fwrite(const void *ptr, size_t size, size_t nmemb, void *stream);

/**
 * @brief Seek to a specific position in an opened file.
 *
 * Analogous to the fseek() function in POSIX.
 *
 * @param stream Pointer reference returned by _afopen() or _wfopen().
 * @param offset Seek offset.
 * @param whence Treat offset as relative to start of file/current offset/end of file.
 * @return 0 when successful, -1 when there's an error.
 * @see sys_seek_whence_e
 */
extern int __fseek(void *stream, long offset, int whence);

/**
 * @brief Return the current position of the file.
 *
 * Analogous to the ftell() function in POSIX.
 *
 * @param stream Pointer reference returned by _afopen() or _wfopen().
 * @return Current position when successful, -1 when there's an error.
 */
extern long _ftell(void *stream);

/**
 * @brief Flush the cached writes to the file.
 *
 * Analogous to the fflush() function in POSIX.
 *
 * @param stream Pointer reference returned by _afopen() or _wfopen().
 * @return 0 when successful, -1 when there's an error.
 */
extern int __fflush(void *stream);

/**
 * @brief Close a file.
 *
 * Analogous to the fclose() function in POSIX.
 *
 * @param stream Pointer reference returned by _afopen() or _wfopen().
 * @return Unclear. Could be similar to POSIX fclose().
 */
extern int _fclose(void *stream);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_FILE_H__
