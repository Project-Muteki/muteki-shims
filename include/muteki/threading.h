/**
 * @file threading.h
 * @brief Native threading API.
 */

#ifndef __MUTEKI_THREADING_H__
#define __MUTEKI_THREADING_H__

#include <muteki/common.h>

typedef void (*thread_func_t)(void);
typedef void thread_t;

/**
 * @brief Sleep for \p millis milliseconds.
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

#endif // __MUTEKI_THREADING_H__
