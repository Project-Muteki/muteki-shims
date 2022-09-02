/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

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
     * @brief Modify timestamp.
     * @see FIND_TS_YEAR
     * @see FIND_TS_MONTH
     * @see FIND_TS_DAY
     * @see FIND_TS_HOUR
     * @see FIND_TS_MINUTE
     * @see FIND_TS_SECOND
     */
    unsigned int mtime; // 0x18
    /**
     * @brief Create/birth timestamp.
     * The format of this field seems to be similar to mtime, but seems to be corrupted somehow.
     * @see mtime
     */
    unsigned int btime; // 0x1c
    /**
     * @brief Access timestamp.
     * @see mtime
     */
    unsigned int atime; // 0x20
    uint8_t unk36; // 0x24
    /**
     * @brief FAT filesystem file attributes.
     */
    uint8_t attrib; // 0x25
} find_context_t;

/**
 * @brief Read year from find timestamp.
 */
#define FIND_TS_YEAR(ts) ((ts >> 25) + 1980)
/**
 * @brief Read month from find timestamp.
 */
#define FIND_TS_MONTH(ts) ((ts >> 21) & 0xf)
/**
 * @brief Read day from find timestamp.
 */
#define FIND_TS_DAY(ts) ((ts >> 16) & 0x1f)

/**
 * @brief Read hour from find timestamp.
 */
#define FIND_TS_HOUR(ts) ((ts >> 11) & 0x1f)
/**
 * @brief Read minute from find timestamp.
 */
#define FIND_TS_MINUTE(ts) ((ts >> 5) & 0x3f)
/**
 * @brief Read second from find timestamp.
 */
#define FIND_TS_SECOND(ts) ((ts & 0xf) * 4)

/**
 * @brief File/directory attributes.
 *
 * Just the attributes in FAT32.
 * @see https://docs.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants
 */
enum fs_attribute_e {
    /** Entry is read only. */
    ATTR_READONLY = 0x1,
    /** Entry is hidden. */
    ATTR_HIDDEN = 0x2,
    /** Entry is a system file/directory. */
    ATTR_SYSTEM = 0x4,
    /** Entry is a directory. */
    ATTR_DIR = 0x10,
    /** Entry is archived. */
    ATTR_ARCHIVE = 0x20,
    ATTR_DEVICE = 0x40,
    /** Entry does not have any other attribute. */
    ATTR_NONE = 0x80,
};

/**
 * @brief Delete a file.
 *
 * @param pathname DOS 8.3 path to the file being removed.
 * @return Whether or not the operation was successful.
 */
extern bool _aremove(const char *pathname);

/**
 * @brief Delete a file.
 *
 * @param pathname UTF-16 LFN path to the file being removed.
 * @return Whether or not the operation was successful.
 */
extern bool __wremove(const UTF16 *pathname);

/**
 * @brief Begin a file/directory search.
 *
 * Matches the first file/directory based on fnmatch pattern @p fnmatch and attribute mask @p attrib_mask. Saves context at @p ctx, which can be used later with @p _afindnext and @p _findclose.
 *
 * @param fnmatch The pattern.
 * @param ctx The context object.
 * @param attrib_mask Attribution mask.
 * @return 0 if there is a match. -1 when no match was found.
 */
extern short _afindfirst(const char *fnmatch, find_context_t *ctx, int attrib_mask);

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
 * Matches the first file/directory based on fnmatch pattern @p fnmatch and attribute mask @p attrib_mask. Saves context at @p ctx, which can be used later with @p _afindnext and @p _findclose.
 *
 * @param fnmatch The pattern (encoded in UTF-16).
 * @param ctx The context object.
 * @param attrib_mask Attribution mask.
 * @return 0 if there is a match. -1 when no match was found.
 */
extern short _wfindfirst(const UTF16 *fnmatch, find_context_t *ctx, int attrib_mask);

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

/**
 * @brief Get attributes of specific path.
 *
 * Similar to GetFileAttributeW() in Windows.
 *
 * @param path UTF-16 LFN path.
 * @return The attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _wfgetattr(UTF16 *path);

/**
 * @brief Set attributes of specific path.
 *
 * Similar to SetFileAttributeW() in Windows.
 *
 * @param path UTF-16 LFN path.
 * @param attrs The new attribute.
 * @return The new attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _wfsetattr(UTF16 *path, short attrs);

/**
 * @brief Get attributes of specific path.
 *
 * Similar to GetFileAttributeA() in Windows.
 *
 * @param path DOS 8.3 path.
 * @return The attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _afgetattr(char *path);

/**
 * @brief Set attributes of specific path.
 *
 * Similar to SetFileAttributeA() in Windows.
 *
 * @param path DOS 8.3 path.
 * @param attrs The new attribute.
 * @return The new attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _afsetattr(char *path, short attrs);

#endif // __MUTEKI_FS_H__
