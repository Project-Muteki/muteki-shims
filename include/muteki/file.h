/**
 * @file file.h
 * @brief Low-level and built-in high-level file operation API.
 */

#ifndef __MUTEKI_FILE_H__
#define __MUTEKI_FILE_H__

#include <muteki/common.h>

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
 * @param mode Mode. Tested modes are @p u"rb" and @p u"wb+".
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
 * @param stream Pointer reference returned by @p _afopen or @p _wfopen.
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
 * @param stream Pointer reference returned by @p _afopen or @p _wfopen.
 * @return Number of data units written.
 */
extern size_t _fwrite(const void *ptr, size_t size, size_t nmemb, void *stream);

/**
 * @brief Close a file.
 *
 * Analogous to the fclose() function in POSIX.
 *
 * @param stream Pointer reference returned by @p _afopen or @p _wfopen.
 * @return Unclear. Could be similar to POSIX fclose().
 */
extern int _fclose(void *stream);
#endif // __MUTEKI_FILE_H__
