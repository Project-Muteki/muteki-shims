/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file threading.h
 * @brief Native threading API.
 */

#ifndef __MUTEKI_THREADING_H__
#define __MUTEKI_THREADING_H__

#include <muteki/common.h>

typedef void (*thread_func_t)(void);
typedef void thread_t;

/** Critical section struct. */
typedef struct {
    /** Magic */
    int32_t magic; // 0x00000202
    /** Kernel reentrant struct for this thread. */
    void *reent; // 0x58 bytes, possibly related to threads
    /** Reference counter. */
    uint16_t refcount;
    /** Unknown. */
    uint8_t unk_idx;
    /** Unknown. */
    uint8_t unk[0x9];
} critical_section_t; // 0x14

/**
 * @brief Sleep for @p millis milliseconds.
 *
 * @param millis Time to sleep in milliseconds.
 */
extern void OSSleep(short millis);

/**
 * @brief Create a new thread.
 *
 * TODO: Incomplete and not working yet.
 *
 * @param func Function to execute in the new thread.
 * @param unk2 Unknown. Set to 0.
 * @param stack_size Seems to be the size of the thread stack.
 * @param unk4 Unknown. Set to 0.
 * @return The thread object.
 */
extern thread_t *OSCreateThread(thread_func_t func, int unk2, size_t stack_size, int unk4);

/**
 * @brief Initialize a critical section (recursive mutex) context.
 *
 * @param mutex The critical section/mutex context.
 */
extern void OSInitCriticalSection(critical_section_t *mutex);

/**
 * @brief Enter/aquire a critical section (recursive mutex) context.
 *
 * This will block when multiple threads are trying to enter the same context, but it will let repeated entries initiated by the same thread to pass through. The context is released when all of the entries are reverted by a OSLeaveCriticalSection() call.
 *
 * @param mutex The critical section/mutex context.
 */
extern void OSEnterCriticalSection(critical_section_t *mutex);

/**
 * @brief Leave/release a critical section (recursive mutex) context.
 *
 * @param mutex The critical section/mutex context.
 */
extern void OSLeaveCriticalSection(critical_section_t *mutex);

/**
 * @brief Destroy a critical section (recursive mutex) context after use.
 *
 * @param mutex The critical section/mutex context.
 */
extern void OSDeleteCriticalSection(critical_section_t *mutex);

#endif // __MUTEKI_THREADING_H__
