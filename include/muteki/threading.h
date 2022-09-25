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
    /** Waiting on an event. */
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

/** Result of waitables. */
typedef enum wait_result_e {
    /** Timeout before the event is set. */
    WAIT_RESULT_TIMEOUT = 0x82,
    /** The event is set. */
    WAIT_RESULT_RESOLVED,
    /** An error occurred. */
    WAIT_RESULT_ERROR,
} wait_result_t;

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
 * @brief Message type for message queues.
 *
 * This needs to be 4 byte aligned since the inline memcpy in the internal FIFO queue routines use hardcoded ldm/stm.
 */
typedef char message_queue_message_t[16] __attribute__((aligned(4)));

/**
 * @brief Nonatomic backend storage for message queues.
 *
 * Simple ring-buffer-based FIFO queue data structure used internally by message queues.
 */
typedef struct {
    /** Message body. */
    message_queue_message_t *messages;
    /** Number of chunks. */
    unsigned short size;
    /** Pop index. */
    short pop_idx;
    /** Push index. */
    short push_idx;
    short _padding_0xa;
} message_queue_nonatomic_t;

/**
 * @brief Thread descriptor type.
 */
typedef struct thread_s thread_t;
/**
 * @brief Semaphore descriptor type.
 */
typedef struct semaphore_s semaphore_t;
/**
 * @brief Event descriptor type.
 */
typedef struct event_s event_t;
/**
 * @brief Critical section descriptor type.
 */
typedef struct critical_section_s critical_section_t;
/**
 * @brief Message queue descriptor type.
 */
typedef struct message_queue_s message_queue_t;

/**
 * @brief Thread descriptor structure.
 */
struct thread_s {
    /** Magic. Always @p 0x100. */
    int magic; // always 0x100
    /** Stack pointer. When the thread is suspended this will point to the CPU context saved on thread stack. */
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
 * @brief Semaphore descriptor structure.
 */
struct semaphore_s {
    /** Magic. Always @p 0x200. */
    int magic;
    int _padding_0x4;
    /** Counter. */
    short ctr;
    /**
     * @brief Wait state of the current semaphore.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
};

/**
 * @brief Event descriptor structure.
 */
struct event_s {
    /** Magic. Always @p 0x201 */
    int magic;
    /** Flag value. 1 is set and 0 is clear. */
    int flag;
    /** Set to non-0 will inhibit the event from getting cleared after a OSWaitForEvent() is resolved. */
    short latch_on;
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
    /** Magic. Always @p 0x202. Note that for some reason this is the same as ::message_queue_t. */
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
 * @brief Message queue descriptor structure.
 */
struct message_queue_s {
    /** Magic. Always @p 0x202. Note that for some reason this is the same as ::critical_section_t. */
    int magic;
    /** Storage structure i.e. the actual queue part of the queue. */
    message_queue_nonatomic_t *storage;
    short _padding_0x8;
    /**
     * @brief Wait state of the current event.
     * @see threading_waitable_t
     */
    threading_waitable_t wait_state;
    char _padding_0x13;
};

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
 * @param arg2 TODO Unknown. thread_t::unk_0xc will be set to this.
 * @return Always 0.
 */
extern int OSTerminateThread(thread_t *thr, int arg2);

/**
 * @brief Terminate current thread.
 *
 * This calls OSTerminateThread() with the descriptor of current thread as @p thr.
 *
 * @param arg1 TODO Unknown. thread_t::unk_0xc will be set to this.
 * @return Always 0.
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
 * @brief Suspend a thread from outside of that thread.
 *
 * @param thr The thread descriptor.
 * @return true if successful.
 */
extern bool OSSuspendThread(thread_t *thr);

/**
 * @brief Start/restart a previously suspended thread.
 *
 * @param thr The thread descriptor.
 * @return true if successful.
 */
extern bool OSResumeThread(thread_t *thr);

/**
 * @brief Force wake up a sleeping thread
 *
 * This expire the sleep counter of a thread immediately and reschedule if the thread is not suspended.
 *
 * @param thr The thread descriptor.
 * @return true if successful.
 */
extern bool OSWakeUpThread(thread_t *thr);

/**
 * @brief Create an semaphore descriptor.
 *
 * @param init_ctr Initial counter value.
 * @return The semaphore descriptor.
 */
extern semaphore_t *OSCreateSemaphore(short init_ctr);

/**
 * @brief Wait and acquire a semaphore.
 *
 * @param semaphore The semaphore context.
 * @param timeout Timeout in OSSleep() units.
 * @return The result.
 * @see wait_result_t
 */
extern wait_result_t OSWaitForSemaphore(semaphore_t *semaphore, short timeout);

/**
 * @brief Release a semaphore.
 *
 * @param semaphore The semaphore context.
 * @return true if successful.
 */
extern bool OSReleaseSemaphore(semaphore_t *semaphore);

/**
 * @brief Destroy a semaphore.
 *
 * @param semaphore The semaphore context.
 * @return true if successful.
 */
extern bool OSCloseSemaphore(semaphore_t *semaphore);

/**
 * @brief Create an event descriptor.
 *
 * @param latch_on Set to non-0 will inhibit the event from getting cleared after a OSWaitForEvent() is resolved.
 * @param flag The initial flag value. Can be either 0 or 1.
 * @return The event descriptor.
 */
extern event_t *OSCreateEvent(short latch_on, int flag);

/**
 * @brief Wait for an event.
 *
 * @param event The event context.
 * @param timeout Timeout in OSSleep() units.
 * @return The result.
 * @see wait_result_t
 */
extern wait_result_t *OSWaitForEvent(event_t *event, short timeout);

/**
 * @brief Set the event flag.
 *
 * This sets the event_t::flag to 1.
 * @param event The event context.
 * @return true if successful.
 */
extern bool OSSetEvent(event_t *event);

/**
 * @brief Reset the event flag.
 *
 * This sets the event_t::flag to 0.
 * @param event The event context.
 * @return true if successful.
 */
extern bool OSResetEvent(event_t *event);

/**
 * @brief Destroy the event descriptor.
 *
 * @param event The event context.
 * @return true if successful.
 */
extern bool OSCloseEvent(event_t *event);

/**
 * @brief Initialize a critical section descriptor.
 *
 * @param cs The critical section descriptor.
 */
extern void OSInitCriticalSection(critical_section_t *cs);

/**
 * @brief Enter/aquire a critical section.
 *
 * Besta critical sections behave like recursive mutexes. Therefore this will block when multiple threads are trying to enter the same context, but it will let repeated entry attempts initiated by the same thread to pass through. The context is released when all of the entries are reverted by a OSLeaveCriticalSection() call.
 *
 * @param cs The critical section descriptor.
 */
extern void OSEnterCriticalSection(critical_section_t *cs);

/**
 * @brief Leave/release a critical section.
 *
 * @param cs The critical section descriptor.
 */
extern void OSLeaveCriticalSection(critical_section_t *cs);

/**
 * @brief Destroy a critical section descriptor.
 *
 * @param cs The critical section descriptor.
 */
extern void OSDeleteCriticalSection(critical_section_t *cs);

/**
 * @brief Create a message queue descriptor.
 *
 * @param size Size of the queue in number of messages (will use sizeof(::message_queue_message_t) * size bytes of memory).
 * @return The message queue descriptor.
 */
extern message_queue_t *OSCreateMsgQue(unsigned short size);

/**
 * @brief Push a message into the queue.
 *
 * @param queue The message queue descriptor.
 * @param message The message being pushed.
 * @return true if successful.
 */
extern bool OSPostMsgQue(message_queue_t *queue, const message_queue_message_t *message);

/**
 * @brief Push a message into the queue and reschedule immediately.
 *
 * @param queue The message queue descriptor.
 * @param message The message being pushed.
 * @return true if successful.
 */
extern bool OSSendMsgQue(message_queue_t *queue, const message_queue_message_t *message);

/**
 * @brief Peek the bottom of the queue without popping the message.
 *
 * @param queue The message queue descriptor.
 * @param message The result message.
 * @return true if successful.
 */
extern bool OSPeekMsgQue(message_queue_t *queue, message_queue_message_t *message);

/**
 * @brief Pop a message from the queue.
 *
 * @param queue The message queue descriptor.
 * @param message The result message.
 * @return true if successful.
 */
extern bool OSGetMsgQue(message_queue_t *queue, message_queue_message_t *message);

/**
 * @brief Destroy a message queue descriptor.
 *
 * @param queue The message queue descriptor.
 * @return true if successful.
 */
extern bool OSCloseMsgQue(message_queue_t *queue);

#endif // __MUTEKI_THREADING_H__
