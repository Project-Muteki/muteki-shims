/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file utils.h
 * @brief Miscellaneous utility syscalls.
 *
 * Contains syscalls that cannot be easily grouped with other syscalls.
 */

#ifndef __MUTEKI_UTILS_H__
#define __MUTEKI_UTILS_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The framebuffer descriptor.
 *
 * Contains format description for the framebuffer and the pointer to the actual buffer.
 */
typedef struct {
    /**
     * @brief Header magic.
     *
     * Always @p "PX" without NUL.
     */
    char magic[2]; // 0:2
    /**
     * @brief Width of the framebuffer.
     */
    unsigned short width; // 2:4
    /**
     * @brief Height of the framebuffer.
     */
    unsigned short height; // 4:6
    /**
     * @brief Depth of the color in bits.
     *
     * 32 for RGBA, 4 for 16 color grayscale. Could be other values depending on the platform and the display it uses.
     */
    unsigned short depth; // 6:8
    /**
     * @brief Purpose unknown.
     *
     * Maybe some flags? (0x20780 on BA742, 0x20078 on BA110L)
     */
    unsigned int unk; // 8:12
    /**
     * @brief Palette used to map RGBA color to framebuffer color values.
     *
     * Probably only make sense when using pixfmt other than RGBA (i.e. depth of 32). It should be NULL in that case.
     */
    int *palette; // 12:16
    /**
     * @brief The actual framebuffer/VRAM region.
     *
     * The actual format varies and depends on the parameters provided above.
     */
    void *buffer; // 16:20
} vram_descriptor_t; // 20 bytes

/**
 * @brief Get the descriptor for the currently active framebuffer.
 *
 * @return The framebuffer descriptor.
 */ 
extern vram_descriptor_t *GetActiveVRamAddress();

/**
 * @brief Play a tone using the built in piezo speaker.
 * This will block the execution.
 * @param frequency Frequency of the tone in Hz.
 * @param duration Duration of the tone in milliseconds.
 * @return @p true on success.
 */
extern bool Buzzer(int frequency, unsigned short duration);

/**
 * @brief Play a beep using the built in piezo speaker.
 * This usually plays a 4000Hz tone for 10ms.
 * @return @p true on success.
 */
extern bool WarningBeep();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __MUTEKI_UTILS_H__