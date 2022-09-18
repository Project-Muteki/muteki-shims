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
 * @brief Thread wait reason enum.
 */
enum thread_wait_reason_e {
    /** Nothing */
    WAIT_ON_NONE = 0x0,
    /** Waiting on a semaphore. */
    WAIT_ON_SEMAPHORE = 0x1,
    /** Waiting on an event/mailbox message. */
    WAIT_ON_EVENT = 0x2,
    /** Waiting for a message queue push. */
    WAIT_ON_QUEUE = 0x4,
    /** Waiting to be unsuspended by OSResumeThread(). */
    WAIT_ON_SUSPEND = 0x8,
    /** Waiting for a critical section to be released. */
    WAIT_ON_CRITICAL_SECTION = 0x10,
    /** Waiting for sleep counter to expire. */
    WAIT_ON_SLEEP = 0x20,
};

/**
 * @brief Thread function type
 */
typedef void (*thread_func_t)(void *user_data);

/**
 * @brief Common data structure for waitables.
 *
 * Once requested, waitables are able to let a currently running thread to pause execution and wait for a specific event to happen. This is pretty much the same as event group and event table in uC/OS-II.
 */
typedef struct {
    /** Bitfield that indicates which waiting_by bytes are currently active. */
    unsigned char active_bytes;
    /** Bitfield that tracks threads that are waiting for this waitable. Indexed by <tt>byte offset * 8 + bit offset</tt> */
    unsigned char waiting_by[8];
} threading_waitable_t;

/**
 * @brief Thread descriptor type.
 */
typedef struct thread_s thread_t;
/**
 * @brief Event descriptor type.
 */
typedef struct event_s event_t;
/**
 * @brief Critical section descriptor type.
 */
typedef struct critical_section_s critical_section_t;

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
    /** Unknown. Related to thread termination. Initializes to 0. */
    int unk_0xc; // init to 0
    /** Error code. */
    kerrno_t kerrno; // init to 0
    /** Unknown. Initializes to 0x80000000. */
    uintptr_t unk_0x14; // init to 0x80000000
    /** Thread function entrypoint. */
    thread_func_t thread_func;
    /** Unknown. */
    short unk_0x1c;
    /** Milliseconds left to sleep. */
    unsigned short sleep_counter;
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
    /** Event descriptor that belongs to the event the thread is currently waiting for. */
    event_t *event;
    /** Previous thread descriptor. */
    thread_t *prev;
    /** Next thread descriptor. */
    thread_t *next;
    /** Unknown and seems to be uninitialized. */
    char unk_0x34[0x20];
};

/**
 * @brief Event descriptor structure.
 */
struct event_s {
    /** Magic. Always @p 0x201 */
    int magic;
    /** User data attached to the event. */
    void *user_data;
    /** TODO OSCreateEvent arg1 */
    short unk_0x8;
    /**
     * @brief Wait state of the current event.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
};

/**
 * @brief Critical section descriptor structure.
 */
struct critical_section_s {
    /** Magic. Always @p 0x202. */
    int magic; // 0x00000202
    /** Thread descriptor for this thread. */
    thread_t *thr;
    /** Reference counter. */
    unsigned short refcount;
    /**
     * @brief Wait state of the current critical section.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
}; // 0x14

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
 * @brief Terminate a thread.
 *
 * @param thr Thread to terminate.
 * @param arg2 Unknown. thread_t::unk_0xc will be set to this.
 * @return 0 if successful.
 */
extern int OSTerminateThread(thread_t *thr, int arg2);

/**
 * @brief Terminate current thread.
 *
 * This calls OSTerminateThread() with the descriptor of current thread as @p thr.
 *
 * @param arg1 Unknown. thread_t::unk_0xc will be set to this.
 * @return 0 if successful.
 */
extern int OSExitThread(int arg1);

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
