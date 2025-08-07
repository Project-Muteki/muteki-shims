/*
 * Copyright 2025 dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file heap.h
 * @brief Heap memory management function hooks.
 *
 * Since Besta RTOS does not have virtual memory support, sbrk()-based heap
 * management in newlib will not work great. Spoofing a heap space with a huge
 * malloc() on the OS heap wastes memory and can potentially cause the OS to
 * become unstable. Therefore the solution here is to simply redirect basic
 * heap management functions (i.e. malloc(), calloc(), realloc() and free()) to
 * their Besta RTOS native counterpart so we don't need a custom newlib.
 */

#ifndef __OSDEP_HEAP_H__
#define __OSDEP_HEAP_H__

#include <muteki/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate and format mchx memchunk.
 *
 * @param size Size of the memory to allocate.
 * @return Pointer to allocated memory, or NULL if allocation fails.
 */
__attribute__((assume_aligned(8))) extern void *osdep_heap_alloc(size_t size);

/**
 * @brief Get usable size of an allocated mchx memchunk.
 * 
 * @param ptr Memory allocated by osdep_heap_alloc().
 * @return Size of usable memory.
 */
size_t osdep_heap_get_alloc_size(const void *ptr);

/**
 * @brief Free a mchx memchunk.
 *
 * @param ptr Pointer to previously allocated memory.
 * @x_void_return
 */
extern void osdep_heap_free(void *ptr);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __OSDEP_HEAP_H__
