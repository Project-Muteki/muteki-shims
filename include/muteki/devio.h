/**
 * @file devio.h
 * @brief Device IO API.
 */

#ifndef __MUTEKI_DEVIO_H__
#define __MUTEKI_DEVIO_H__
/**
 * @brief Open or create a file/device by its @p pathname.
 *
 * Analogous to the Windows CreateFile() API. Seems to wrap the internal _af* series syscalls.
 *
 * TODO: Documentation on accepted arguments are mostly based on speculations at this moment. More details and through verifications needed.
 *
 * @param pathname DOS 8.3 path to the file to be opened.
 * @param access Access flags.
 * @param shmode File sharing mode. (?)
 * @param secattr Seems to be ignored.
 * @param on_noentry Behavior when entry does not exist. (?)
 * @param flags Flags and attributes of the file. (?)
 * @param template_file Seems to be ignored.
 * @return File handle. Depending on the type of the handle it could either be a pointer reference to a file struct or a device descriptor.
 */
extern void *CreateFile(char *pathname, int access, int shmode, void *secattr, int on_noentry, int flags, void *template_file);

/**
 * @brief Close a handle.
 *
 * Analogous to the Windows CloseHandle() API.
 *
 * @param h Handle to close.
 * @return Whether the operation was successful.
 */
extern bool CloseHandle(void *h);

/**
 * @brief Send an IOCTL request to a device handle @p h.
 *
 * Analogous to the Windows DeviceIoControl() API.
 *
 * @param h Device handle.
 * @param request IOCTL request type.
 * @param in The input buffer, or NULL if not applicable.
 * @param inlen The size of the input buffer in bytes.
 * @param out The output buffer, or NULL if not applicable.
 * @param outlen The size of the output buffer in bytes.
 * @param retlen The size of returned data (if applicable).
 * @param overlapped Probably ignored.
 * @return Whether the operation was successful.
 */
extern bool DeviceIoControl(void *h, int request, const void *in, int inlen, void *out, int outlen, int *retlen, void *overlapped);

#endif // __MUTEKI_DEVIO_H__
