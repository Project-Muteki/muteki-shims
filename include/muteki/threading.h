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
#include <muteki/errno.h>

/**
 * @brief Thread function type
 */
typedef void (*thread_func_t)(void *user_data);

/**
 * @brief Thread wait reason enum.
 */
enum thread_wait_reason_e {
    /** Nothing */
    WAIT_ON_NONE = 0x0;
    /** Waiting to be unsuspended by OSResumeThread(). */
    WAIT_ON_SUSPEND = 0x8;
};

/**
 * @brief Thread descriptor type.
 */
typedef struct thread_s thread_t;

/**
 * @brief Thread descriptor structure.
 */
struct thread_s {
    /** Magic. Always @p 0x100. */
    int magic; // always 0x100
    /** Stack pointer. */
    uintptr_t *sp;
    /** Allocated stack memory. */
    void *stack;
    /** Unknown. Initializes to 0. */
    int unk_0xc; // init to 0
    /** Error code. */
    kerrno_t kerrno; // init to 0
    /** Unknown. Initializes to 0x80000000. */
    uintptr_t unk_0x14; // init to 0x80000000
    /** Thread function entrypoint. */
    thread_func_t thread_func;
    /** Unknown. */
    short unk_0x1c;
    /** Unknown. */
    short unk_0x1e;
    /**
     * Current wait reason of the thread.
     * @see thread_wait_reason_e
     */
    short wait_reason; // 0x20
    /** Slot number. For scheduler. */
    short slot; // 0x22
    /** Lower 3 bit of the slot number. For scheduler. */
    char slot_low3b;
    /** Upper 3 bit of the slot number. For scheduler. */
    char slot_high3b;
    /** Lower 3 bit bitmask of the slot number. For scheduler. */
    char slot_low3b_bit;
    /** Upper 3 bit bitmask of the slot number. For scheduler. */
    char slot_high3b_bit;
    /** Event descriptor (?). */
    void *event;
    /** Previous thread descriptor. */
    thread_t *prev;
    /** Next thread descriptor. */
    thread_t *next;
    /** Unknown and seems to be uninitialized. */
    char unk_0x34[0x20];
};

/** Critical section struct. */
typedef struct {
    /** Magic */
    int32_t magic; // 0x00000202
    /** Thread descriptor for this thread. */
    thread_t *thr; // 0x58 bytes, possibly related to threads
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
 * @param func Function to execute in the new thread.
 * @param user_data User data for the thread.
 * @param stack_size The size of the thread stack.
 * @param defer_start Do not immediately schedule this thread and create it as suspended.
 * @return The thread descriptor.
 */
extern thread_t *OSCreateThread(thread_func_t func, void *user_data, size_t stack_size, bool defer_start);

/**
 * @brief Get the thread slot number.
 *
 * On Besta RTOS there is no explicit way to specify thread priority. Priority is implied in the natural order of the threads in the global thread table. Some slots in the table seem to be reserved (8 for the top and 18 for the bottom) and are not accessible by just allocating the thread with OSCreateThread(). User can move threads to these reserved slots by calling the OSSetThreadPriority() function.
 * @param thr The thread descriptor.
 * @return The slot number of the thread.
 */
extern short OSGetThreadPriority(thread_t *thr);

/**
 * @brief Set the thread slot number.
 *
 * @param thr The thread descriptor.
 * @param new_slot The new slot number.
 * @return true if successful.
 * @see OSGetThreadPriority
 */
extern bool OSSetThreadPriority(thread_t *thr, short new_slot);

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
