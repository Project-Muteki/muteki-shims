/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file loader.h
 * @brief Binary loader API.
 */

#ifndef __MUTEKI_LOADER_H__
#define __MUTEKI_LOADER_H__

#include <muteki/common.h>
#include <muteki/file.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defines the default subroutine identifiers to be used with ExecuteProgram().
 * @see ExecuteProgram
 */
enum applet_subroutine_e {
    /** Run the main subroutine. */
    APPLET_SUBROUTINE_MAIN = 0,
    /** Run a subroutine that clears the persistent states of the applet. */
    APPLET_SUBROUTINE_RESET_STATES = 5,
};

/**
 * @brief Applet argument passing V4 struct.
 * @details Mirrors the parameter set by the parent ExecuteProgram() call.
 * @see ExecuteProgram
 */
typedef struct {
    /** DOS 8.3 path to executable. */
    char *dospath;
    /** Subroutine to invoke. */
    int *subroutine;
    /** User argument 1. */
    void **applet_arg1;
    /** User argument 2. */
    void **applet_arg2;
} applet_args_v4_t;

typedef struct {
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x0;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x4;
    /**
     * @brief Low 32 bits of the capacity of the cart.
     */
    size_t size_low_32;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0xc;
    /**
     * @brief Unknown.
     */
    void *unk_0x10;
    /**
     * @brief Unknown.
     */
    unsigned char unk_0x14;
    /**
     * @brief High 8 bits of the capacity of the cart.
     */
    unsigned char size_high_8;
    /**
     * @brief Unknown.
     */
    unsigned char unk_0x16;
    /**
     * @brief Unknown.
     */
    unsigned char unk_0x17;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x18;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x1c;
} loader_cart_descriptor_t; // 0x20

typedef struct {
    /**
     * @brief ROM cart descriptor.
     */
    loader_cart_descriptor_t *cart;
    /**
     * @brief Parent file descriptor.
     * @todo Retype this to the proper file descriptor type once we actually figure out how it works.
     */
    void *parent_fd; /* Created by retype action */
    /**
     * @brief Base offset of the sub-file.
     */
    size_t subfile_base;
    /**
     * @brief Base offset of this sub-file, within #parent_fd.
     */
    size_t size;
    /**
     * @brief Current `ftell` offset of this sub-file.
     */
    size_t subfile_offset;
    /**
     * @brief Unknown. Seems to be unused.
     */
    short unk_0x14; /* _OpenSubFile sets this to 1 */
    /**
     * @brief Unknown.
     */
    short unk_0x16;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x18;
    /**
     * @brief Unknown.
     */
    unsigned int unk_0x1c;
} loader_file_descriptor_t;

/**
 * @brief Load an applet executable.
 * @details The `pathname` specified must be a DOS 8.3 name.
 * @x_syscall_num `0x10116`
 * @param pathname Path to executable.
 * @return A pointer to a structure describing the loaded executable, or NULL if the loading process failed.
 */
extern void *LoadProgramA(const char *pathname);

/**
 * @brief Load an applet executable. (UTF-16 variant)
 * @x_syscall_num `0x10281`
 * @param pathname UTF-16 LFN path to executable.
 * @return A pointer to a structure describing the loaded executable, or NULL if the loading process failed.
 * @see LoadProgramA
 */
extern void *LoadProgramW(const UTF16 *pathname);

/**
 * @brief Invoke a specific subroutine of the loaded applet with arguments.
 * @details
 * Once called, the control is fully transferred to the loaded applet and this function will block until the invoked subroutine fully exits.
 *
 * Actual format of arguments seem to be applet-specific.
 *
 * @x_syscall_num `0x10118`
 *
 * @param prog The loaded executable description returned by LoadProgramA or LoadProgramW.
 * @param subroutine Subroutine to invoke.
 * @param applet_arg1 Argument 1. Can either be a pointer to some data or an integer.
 * @param applet_arg2 Argument 2. Can either be a pointer to some data or an integer.
 * @return The return value of the subroutine
 * @see LoadProgramA
 * @see LoadProgramW
 * 
 */
extern int ExecuteProgram(void *prog, int subroutine, const void *applet_arg1, const void *applet_arg2);

/**
 * @brief Unload a loaded applet.
 * @x_syscall_num `0x10117`
 * @param prog The loaded executable description returned by LoadProgramA() or LoadProgramW().
 * @see LoadProgramA
 * @see LoadProgramW
 */
extern int FreeProgram(void *prog);

/**
 * @brief Get the path to the current running executable (argv[0]).
 * @x_syscall_num `0x10119`
 * @x_void_param
 * @return The DOS 8.3 path to the current running executable.
 */
extern const char *GetCurrentPathA();

/**
 * @brief Get the path to the current running executable (argv[0]). (UTF-16 variant)
 * @x_syscall_num `0x10282`
 * @x_void_param
 * @return The UTF-16 LFN path to the current running executable.
 */
extern const UTF16 *GetCurrentPathW();

/**
 * @brief Get the title name of the ROM file.
 * @details This will return UTF16-encoded localized title name that matches the current locale.
 * @x_syscall_num `0x10115`
 * @param[in] pathname DOS 8.3 path to the ROM file.
 * @param[out] out_name The title name in current locale.
 * @param max_size Max size of the title name.
 * @return Length of the title name in number of UTF16 code units.
 */
extern size_t GetApplicationNameA(const char *pathname, UTF16 *out_name, size_t max_size);

/**
 * @brief Get the title name of the ROM file (LFN path).
 * @details This will return UTF16-encoded localized title name that matches the current locale.
 * @warning Not all systems implement this. When not implemented, the length read will always be 0.
 * @x_syscall_num `0x10280`
 * @param[in] pathname LFN path to the ROM file.
 * @param[out] out_name The title name in current locale.
 * @param max_size Max size of the title name.
 * @return Length of the title name in number of UTF16 code units.
 */
extern size_t GetApplicationNameW(const UTF16 *pathname, UTF16 *out_name, size_t max_size);

/**
 * @brief Open a loader file descriptor from a file.
 * @details
 *
 * `mode_or_subfile` can either be a mode or one of the following UNC paths:
 * 
 * <table>
 * <tr><th>Path</th> <th>Effect</th></tr>
 *
 * <tr><td><tt>\\\\.\\ROMFILE</tt></td> <td>Reopens the current ROM/applet file.</td></tr>
 * <tr><td><tt>\\\\.\\DATAFILE</tt></td> <td>Opens the data partition of the cartridge as a file. More details TBA.</td></tr>
 * </table>
 *
 * @x_syscall_num `0x100ec`
 *
 * @param[in] pathname DOS 8.3 path to the file.
 * @param[in] mode_or_subfile Either a mode string accepted by _afopen(), or a special UNC path to open assets of the
 * current applet/cartridge.
 * @return Loader file descriptor
 */
extern loader_file_descriptor_t *_OpenFile(const char *pathname, const char *mode_or_subfile);

/**
 * @brief Open a loader file descriptor from a file (LFN path).
 * @x_syscall_num `0x100ee`
 * @param[in] pathname LFN path to the file.
 * @param[in] mode_or_subfile Either a mode string accepted by __wfopen() (in UTF-16), or a special UNC path (**in ASCII**) to open
 * assets of the current applet/cartridge.
 * @return Loader file descriptor
 */
extern loader_file_descriptor_t *_OpenFileW(const UTF16 *pathname, const void *mode_or_subfile);

/**
 * @brief Close a loader file descriptor.
 * @x_syscall_num `0x100ef`
 * @param ldrfd The loader file descriptor.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _CloseFile(loader_file_descriptor_t *ldrfd);

/**
 * @brief Read data from a loader file descriptor.
 * @x_syscall_num `0x100f0`
 * @param ldrfd The loader file descriptor.
 * @param[out] buf Buffer that holds the data.
 * @param size Number of bytes to be read.
 * @return Actual number of bytes read.
 */
extern size_t _ReadFile(loader_file_descriptor_t *ldrfd, void *buf, size_t size);

/**
 * @brief Seek to a specific position in an opened sub-file.
 * @x_syscall_num `0x100f1`
 * @param ldrfd The loader file descriptor.
 * @param offset Seek offset.
 * @param whence Treat offset as relative to start of file/current offset/end of file.
 * @retval 0 @x_term ok
 * @retval -1 @x_term ng
 */
extern int _FseekFile(loader_file_descriptor_t *ldrfd, size_t offset, int whence);

/**
 * @brief Get the size of sub-file.
 * @x_syscall_num `0x100f2`
 * @param ldrfd The loader file descriptor.
 * @return The size of the sub-file, or `-1` when something is wrong.
 */
extern ssize_t _FileSize(loader_file_descriptor_t *ldrfd);

/**
 * @brief Open a sub-file from a parent loader file descriptor.
 * @details Sub-files are areas in the parent file. The parameter `base` and `max_size` controls where and how large the area
 * will be. The sub-file descriptors operate independently of the parent file descriptor, a la `dup()` in POSIX. This is also
 * used internally to parse ROM files and asset packs.
 * @x_syscall_num `0x100f3`
 * @param parent Parent file descriptor.
 * @param base Base offset of the file.
 * @param max_size Max size of the area included in this sub-file.
 * @return The loader sub-file descriptor.
 */
extern loader_file_descriptor_t *_OpenSubFile(loader_file_descriptor_t *parent, size_t base, size_t max_size);

/**
 * @brief Get the current offset of sub-file.
 * @x_syscall_num `0x100f4`
 * @param ldrfd The loader file descriptor.
 * @return The offset of the sub-file, or `-1` when something is wrong.
 */
extern ssize_t _TellFile(loader_file_descriptor_t *ldrfd);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_LOADER_H__
