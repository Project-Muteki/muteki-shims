/**
 * @file file.h
 * @brief Low-level and built-in high-level file operation API.
 */

#ifndef __MUTEKI_FILE_H__
#define __MUTEKI_FILE_H__

#include <muteki/common.h>

/**
 * @brief Open a file located at @p pathname
 *
 * Analogous to the POSIX and Windows fopen() function.
 *
 * @param pathname DOS 8.3 path to the file to be opened.
 * @param mode Mode. Tested modes are @p "rb" and @p "wb+".
 * @return Pointer reference to the opened file.
 */
extern void *_afopen(const char *pathname, const char *mode);

/**
 * @brief Read @p nmemb data units of size @p size from a file.
 *
 * Analogous to the POSIX fread() function.
 *
 * @param ptr Pointer reference to a buffer that will hold the data read from the file.
 * @param size Size of individual data unit.
 * @param nmemb Number of data units to read.
 * @param stream Pointer reference returned by @p _afopen or @p _wfopen.
 * @return Whether or not the operation succeeded.
 */
extern bool _fread(void *ptr, size_t size, size_t nmemb, void *stream);

/**
 * @brief Write @p nmemb data units of size @p size to a file.
 *
 * Analogous to the POSIX fwrite() function.
 *
 * @param ptr Pointer reference to data that will be written to the file.
 * @param size Size of individual data unit.
 * @param nmemb Number of data units to write.
 * @param stream Pointer reference returned by @p _afopen or @p _wfopen.
 * @return Whether or not the operation succeeded.
 */
extern bool _fwrite(const void *ptr, size_t size, size_t nmemb, void *stream);

/**
 * @brief Close a file.
 *
 * Analogous to the POSIX fclose() function.
 *
 * @param stream Pointer reference returned by @p _afopen or @p _wfopen.
 * @return Unclear. Could be similar to POSIX fclose().
 */
extern int _fclose(void *stream);
#endif // __MUTEKI_FILE_H__
