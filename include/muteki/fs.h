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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Max code units (ASCII or UTF-16) a path can contain.
 * @details This includes the NUL terminator. Note that this can differ from libc definitions due to possible encoding
 * differences (e.g. newlib-muteki uses UTF-8 as the "ANSI" encoding by default).
 */
#define SYS_PATH_MAX_CU 256

/**
 * @brief Structure for find context.
 */
typedef struct {
    /**
     * @brief Current file descriptor.
     */
    void *unk0; // 0x0
    /**
     * @brief Next file descriptor.
     */
    void *unk4; // 0x4
    /** UTF-16-encoded long filename of the entry. */
    UTF16 *filename_lfn; // 0x8
    /** DOS 8.3 filename of the entry. */
    char *filename; // 0xc
    /**
     * @brief Seems to be a mirror of #filename
     * @see filename
     */
    char *filename2_alt; // 0x10
    /** Size of file. */
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
     * @details The format of this field seems to be similar to mtime, but seems to be corrupted somehow.
     * @see mtime
     */
    unsigned int btime; // 0x1c
    /**
     * @brief Access timestamp.
     * @see mtime
     */
    unsigned int atime; // 0x20
    /** FAT filesystem file attribute mask. */
    unsigned char attrib_mask; // 0x24
    /** FAT filesystem file attributes. */
    unsigned char attrib; // 0x25
} find_context_t;

/**
 * @brief Filesystem usage statistics.
 */
typedef struct {
    /** Filesystem size in KiB. */
    size_t size_kb;
    /** Seems to be the same as fs_stat_t::size_kb. */
    size_t size2_kb;
    /** Seems to be the same as fs_stat_t::size_kb. */
    size_t size3_kb;
    /** Used space in KiB. */
    size_t used_kb;
    /** Available space in KiB. */
    size_t free_kb;
    /** Filesystem size in bytes. */
    unsigned long long SYS_DWORD size;
    /** Seems to be the same as fs_stat_t::size. */
    unsigned long long SYS_DWORD size2;
    /** Seems to be the same as fs_stat_t::size. */
    unsigned long long SYS_DWORD size3;
    /** Used space in bytes. */
    unsigned long long SYS_DWORD used;
    /** Available space in bytes. */
    unsigned long long SYS_DWORD free;
} fs_stat_t;

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
 * @details Just the attributes in FAT32.
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
 * @param pathname DOS 8.3 path to the file being removed.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool _aremove(const char *pathname);

/**
 * @brief Delete a file.
 * @param pathname UTF-16 LFN path to the file being removed.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool __wremove(const UTF16 *pathname);

/**
 * @brief Begin a file/directory search.
 * @details Matches the first file/directory based on fnmatch pattern `fnmatch` and attribute mask `attrib_mask`. Saves
 * context at `ctx`, which can be used later with _afindnext() and _findclose().
 * @param fnmatch The pattern.
 * @param ctx The context object.
 * @param attrib_mask Attribute mask (use 0 to match everything regardless of attributes).
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _afindfirst(const char *fnmatch, find_context_t *ctx, int attrib_mask);

/**
 * @brief Continue a previously started file/directory search.
 * @details Matches the next file/directory.
 * @param ctx The context object.
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _afindnext(find_context_t *ctx);

/**
 * @brief Begin a file/directory search (LFN support).
 * @details Matches the first file/directory based on fnmatch pattern `fnmatch` and attribute mask `attrib_mask`. Saves
 * context at `ctx`, which can be used later with _afindnext() and _findclose().
 * @param fnmatch The pattern (encoded in UTF-16).
 * @param ctx The context object.
 * @param attrib_mask Attribute mask (use 0 to match everything regardless of attributes).
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _wfindfirst(const UTF16 *fnmatch, find_context_t *ctx, int attrib_mask);

/**
 * @brief Continue a previously started file/directory search.
 * @details
 * Matches the next file/directory.
 *
 * Not sure if interchangeable with _afindnext() since it should have no dependency on different find context. Needs to
 * be verified later.
 *
 * @param ctx The context object.
 * @retval 0 There is a match.
 * @retval -1 No more match was found.
 */
extern short _wfindnext(find_context_t *ctx);

/**
 * @brief Terminate a file/directory search.
 * @param ctx The context object.
 * @retval 0 @x_term ok
 */
extern int _findclose(find_context_t *ctx);

/**
 * @brief Get attributes of specific path.
 * @details Similar to GetFileAttributeW() in Windows.
 * @param path UTF-16 LFN path.
 * @return The attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _wfgetattr(UTF16 *path);

/**
 * @brief Set attributes of specific path.
 * @details Similar to SetFileAttributeW() in Windows.
 * @param path UTF-16 LFN path.
 * @param attrs The new attribute.
 * @return The new attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _wfsetattr(UTF16 *path, short attrs);

/**
 * @brief Get attributes of specific path.
 * @details Similar to GetFileAttributeA() in Windows.
 * @param path DOS 8.3 path.
 * @return The attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _afgetattr(char *path);

/**
 * @brief Set attributes of specific path.
 * @details Similar to SetFileAttributeA() in Windows.
 * @param path DOS 8.3 path.
 * @param attrs The new attribute.
 * @return The new attribute, or -1 on error.
 * @see fs_attribute_e
 */
extern short _afsetattr(char *path, short attrs);

/**
 * @brief Create a new directory.
 * @param path UTF-16 LFN path to the new directory.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _wmkdir(UTF16 *path);

/**
 * @brief Create a new directory.
 * @param path DOS 8.3 path to the new directory.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _amkdir(char *path);

/**
 * @brief Remove a directory.
 * @details Sets kerrno namespace to ::ERRNO_NS_KERNEL and error code to `0xffff` when directory is not empty.
 * @param path UTF-16 LFN path to the directory being removed.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _wrmdir(UTF16 *path);

/**
 * @brief Remove a directory.
 * @details Sets kerrno namespace to ::ERRNO_NS_KERNEL and error code to `0xffff` when directory is not empty.
 * @param path DOS 8.3 path to the directory being removed.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _armdir(char *path);

/**
 * @brief Get system CWD value.
 * @param unk Some context object, format unknown. Use NULL to get the CWD of current applet.
 * @param buf Buffer large enough to contain a DOS8.3 path. Must be at least ::SYS_PATH_MAX_CU bytes long.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _agetcurdir(void *unk, char *buf);

/**
 * @brief Get system CWD value.
 * @param unk Some context object, format unknown. Use NULL to get the CWD of current applet.
 * @param buf Buffer large enough to contain a UTF-16 LFN path. Must be at least ::SYS_PATH_MAX_CU units long.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _wgetcurdir(void *unk, UTF16 *buf);

/**
 * @brief Change system working directory to a new path.
 * @param path DOS 8.3 path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _achdir(const char *path);

/**
 * @brief Change system working directory to a new path.
 * @param path UTF-16 LFN path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _wchdir(const UTF16 *path);

/**
 * @brief Rename/move a file.
 * @param old_path Old DOS 8.3 path.
 * @param new_path New DOS 8.3 path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _arename(const char *old_path, const char *new_path);

/**
 * @brief Rename/move a file.
 * @param old_path Old UTF-16 LFN path.
 * @param new_path New UTF-16 LFN path.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern short _wrename(const UTF16 *old_path, const UTF16 *new_path);

/**
 * @brief Get filesystem usage stats.
 * @param fsid Filesystem ID.
 * @param fs_stat Target to store filesystem usage stats.
 * @retval 0 @x_term ok
 */
extern int FSGetDiskRoomState(int fsid, fs_stat_t *fs_stat);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_FS_H__
