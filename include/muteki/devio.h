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
extern void *CreateFile(char *pathname, int access, int shmode, void *secattr, int on_noentry, int flags, void *template_file);

/**
 * @brief Close a handle.
 * @details Analogous to the Windows CloseHandle() API.
 * @param h Handle to close.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool CloseHandle(void *h);

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
extern bool DeviceIoControl(void *h, int request, const void *in, int inlen, void *out, int outlen, int *retlen, void *overlapped);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_DEVIO_H__urn Whether the operation was successful.
