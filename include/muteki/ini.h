/*
 * Copyright 2024-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file ini.h
 * @brief INI file operation API.
 * @todo This is mostly ripped from MSDN. Verification needed. Specifically some of the behaviors around `NULL`
 * handling.
 */

#ifndef __MUTEKI_INI_H__
#define __MUTEKI_INI_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Read a value from an INI file and parse it as an integer.
 * @x_syscall_num `0x10110`
 * @param section The section where the key is located.
 * @param key The key.
 * @param default_value The default value to be returned when the key does not exist in the INI file.
 * @param path Path to INI file.
 * @return The value that is associated with `section.key`, or `default` if the key does not exist.
 */
extern unsigned int _GetPrivateProfileInt(const char *section, const char *key, int default_value, const char *path);

/**
 * @brief Read a value from an INI file.
 * @details For values, the function will copy at most `outsize - 1` bytes and the last byte in the `out` buffer will
 * always be set to <tt>'\0'</tt>. For listing names, the function will output at most `outsize - 2` bytes, with the
 * last 2 bytes always set to <tt>'\0'</tt> with the first one being a string terminator and the second one being a list
 * terminator.
 * @x_syscall_num `0x10111`
 * @param section The section where the key is located, or `NULL` to list all available sections.
 * @param key The key, or `NULL` to list all available keys.
 * @param default_value The default value to be copied to `out` when the key does not exist in the INI file, or `""`
 * when set to `NULL`.
 * @param[out] out Pointer to the output buffer.
 * @param outsize Maximum size of the output buffer.
 * @param path Path to INI file.
 * @return The length of the string copied to the `out` buffer.
 */
extern unsigned int _GetPrivateProfileString(
    const char *section,
    const char *key,
    const char *default_value,
    char *out,
    size_t outsize,
    const char *path
);

/**
 * @brief Write a value to or remove a value from an INI file.
 * @x_syscall_num `0x10112`
 * @param section The section where the key is located.
 * @param key The key, or `NULL` to remove this section.
 * @param value The value, or `NULL` to remove this key.
 * @param path Path to INI file.
 * @retval true @x_term ok
 * @retval false @x_term ng
 */
extern bool _WritePrivateProfileString(const char *section, const char *key, const char *value, const char *path);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_INI_H__
