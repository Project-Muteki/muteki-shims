/**
 * @file fs.h
 * @brief Filesystem access API.
 */

#ifndef __MUTEKI_FS_H__
#define __MUTEKI_FS_H__

#include <muteki/common.h>

typedef struct {
    uint8_t *unk0; // 0x0
    uint8_t *unk4; // 0x4
    /**
     * @brief UTF-16-encoded long filename of the entry.
     */
    UTF16 *filename_lfn; // 0x8
    /**
     * @brief DOS 8.3 filename of the entry.
     */
    char *filename; // 0xc
    /**
     * @brief Seems to be a mirror of @p filename
     * @see filename
     */
    char *filename2_alt; // 0x10
    /**
     * @brief Size of file.
     */
    size_t size; // 0x14
    /**
     * @brief Modify datestamp in FAT datestamp format.
     *
     * See https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#DIR_OFS_10h for the exact format.
     */
    unsigned short fat_mdate; // 0x18
    /**
     * @brief Might be a sorting key based on mtime.
     *
     * Seems to be based on the actual timestamp with precision down to 4 seconds. However there are bit flips that don't make sense. Therefore recovering the exact timestamp might not be possible.
     */
    unsigned short mtime_key; // 0x1a
    /**
     * @brief Create datestamp in FAT datestamp format.
     * @see fat_mdate
     */
    unsigned short fat_cdate; // 0x1c
    /**
     * @brief Might be a sorting key based on ctime.
     * @see mtime_key
     */
    unsigned short ctime_key; // 0x1e
    /**
     * @brief Access datestamp in FAT datestamp format.
     * @see fat_mdate
     */
    unsigned short fat_adate; // 0x20
    /**
     * @brief Might be a sorting key based on atime.
     * @see mtime_key
     */
    unsigned short atime_key; // 0x22
    uint8_t unk36; // 0x24
    /**
     * @brief FAT filesystem file attributes.
     */
    uint8_t attrib; // 0x25
} find_context_t;

/**
 * @brief Delete a file.
 *
 * @param pathname DOS 8.3 path to the file being removed.
 * @return Whether or not the operation was successful.
 */
extern bool _aremove(const char *pathname);

/**
 * @brief Begin a file/directory search.
 *
 * Matches the first file/directory based on fnmatch pattern @p fnmatch. Saves context at @p ctx, which can be used later with @p _afindnext and @p _findclose.
 *
 * @param fnmatch The pattern.
 * @param ctx The context object.
 * @return 0 if there is a match. -1 when no match was found.
 */
extern short _afindfirst(const char *fnmatch, find_context_t *ctx);

/**
 * @brief Continue a previously started file/directory search.
 *
 * Matches the next file/directory.
 *
 * @param ctx The context object.
 * @return 0 if there is a match. -1 when no more match was found.
 */
extern short _afindnext(find_context_t *ctx);

/**
 * @brief Begin a file/directory search (LFN support).
 *
 * Matches the first file/directory based on fnmatch pattern @p fnmatch. Saves context at @p ctx, which can be used later with @p _afindnext and @p _findclose.
 *
 * @param fnmatch The pattern (encoded in UTF-16).
 * @param ctx The context object.
 * @return 0 if there is a match. -1 when no match was found.
 */
extern short _wfindfirst(const UTF16 *fnmatch, find_context_t *ctx);

/**
 * @brief Continue a previously started file/directory search.
 *
 * Matches the next file/directory.
 *
 * Not sure if interchangeable with _afindnext() since it should have no dependency on different find context. Needs to be verified later.
 *
 * @param ctx The context object.
 * @return 0 if there is a match. -1 when no more match was found.
 */
extern short _wfindnext(find_context_t *ctx);

/**
 * @brief Terminate a file/directory search.
 *
 * @param ctx The context object.
 * @return 0 on success.
 */
extern int _findclose(find_context_t *ctx);

#endif // __MUTEKI_FS_H__
