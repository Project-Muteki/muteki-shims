/**
 * @file fs.h
 * @brief Filesystem access API.
 */

#ifndef __MUTEKI_FS_H__
#define __MUTEKI_FS_H__

#include <muteki/common.h>

/**
 * @brief Delete a file.
 *
 * @param pathname DOS 8.3 path to the file being removed.
 * @return Whether or not the operation was successful.
 */
extern bool _aremove(const char *pathname);

#endif // __MUTEKI_FS_H__
