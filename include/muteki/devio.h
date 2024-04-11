/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file devio.h
 * @brief Device IO API.
 */

#ifndef __MUTEKI_DEVIO_H__
#define __MUTEKI_DEVIO_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Magic for device IO descriptor.
 * @details The value is ASCII `"BXCH"`.
 */
#define DEVIO_DESC_MAGIC (0x48435842u)

/**
 * @brief The device IO descriptor.
 */
typedef struct {
    /**
     * @brief Path to the device or DOS 8.3 path to the file.
     */
    char path[80];
    /**
     * @brief File descriptor if this descriptor is file-backed or has a file-like descriptor.
     * @todo Change this to the file descriptor type when that is finished.
     */
    void *fd;
    /**
     * @brief Unknown.
     */
    int unk_0x54;
    /**
     * @brief Device ID if this descriptor is device-backed.
     */
    unsigned int device_id;
    /**
     * @brief Reference counter.
     */
    int refcount;
    /**
     * @brief Access mode.
     */
    int access;
    /**
     * @brief Sharing mode.
     */
    int shmode;
    /**
     * @brief Behavior on no entry.
     */
    int on_noentry;
    /**
     * @brief Upper 16-bit of the flags.
     */
    unsigned int flags_upper;
    /**
     * @brief Lower 16-bit of the flags.
     */
    unsigned int flags_lower;
    /**
     * @brief Descriptor magic. Always `BXCH`.
     */
    unsigned int magic;
    /**
     * @brief Unknown.
     */
    int unk_0x78;
} devio_descriptor_t; // 0x7c bytes

/**
 * @brief Invalid descriptor.
 * @details This is usually returned by CreateFile() when an error occurred.
 */
#define DEVIO_DESC_INVALID ((devio_descriptor_t *) 0xffffffff)

/**
 * @brief Open or create a file/device by its `pathname`.
 * @details Analogous to the Windows CreateFile() API. Seems to wrap the internal _af* series syscalls.
 * @todo Documentation on accepted arguments are mostly based on speculations at this moment. More details and through
 * verifications needed.
 * @param pathname DOS 8.3 path to the file to be opened.
 * @param access Access flags.
 * @param shmode File sharing mode. (?)
 * @param secattr Seems to be ignored.
 * @param on_noentry Behavior when entry does not exist. (?)
 * @param flags Flags and attributes of the file. (?)
 * @param template_file Seems to be ignored.
 * @return File handle. Depending on the type of the handle it could either be a pointer reference to a file struct or
 * a device descriptor.
 */
extern devio_descriptor_t *CreateFile(
    char *pathname,
    int access,
    int shmode,
    void *secattr,
    int on_noentry,
    int flags,
    void *template_file
);

/**
 * @brief Close a handle.
 * @details Analogous to the Windows CloseHandle() API.
 * @param h Handle to close.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool CloseHandle(devio_descriptor_t *h);

/**
 * @brief Send an IOCTL request to a device handle `h`.
 * @details Analogous to the Windows DeviceIoControl() API.
 * @param h Device handle.
 * @param request IOCTL request type.
 * @param in The input buffer, or NULL if not applicable.
 * @param inlen The size of the input buffer in bytes.
 * @param out The output buffer, or NULL if not applicable.
 * @param outlen The size of the output buffer in bytes.
 * @param retlen The size of returned data (if applicable).
 * @param overlapped Probably ignored.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool DeviceIoControl(
    devio_descriptor_t *h,
    int request,
    const void *in,
    int inlen,
    void *out,
    int outlen,
    int *retlen,
    void *overlapped
);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_DEVIO_H__urn Whether the operation was successful.
