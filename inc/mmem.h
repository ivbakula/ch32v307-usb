#ifndef _MMEM_H
#define _MMEM_H

#include <stdint.h>

typedef enum { BLK_FREED, BLK_ALLOCD } BlkStatus;

#define GET_STATUS(ptr)         ((uintptr_t)ptr & 3)
#define SET_STATUS(ptr, status) ((uintptr_t)ptr | status)
#define CLR_STATUS(ptr)         ((header *)((uintptr_t)ptr & ~3))
#define GET_PTR(ptr)            ((header *)CLR_STATUS(ptr))
#define IS_FREE(ptr)            (GET_STATUS(ptr) == BLK_FREED)

__attribute__((aligned(4))) typedef struct header {
  uint32_t size;
  struct header *next;
} header;

#define HEADER_GET_SIZE(ptr) (((header *)ptr)->size)

/**
 * @fn allocm
 *
 * @brief Allocate contiguous block of memory.
 * @param size: desired size of memory block.
 */
void *allocm(uint32_t size);

/**
 * @fn freem
 *
 * @brief Free block of memory that was previously allocated
 *        with call to allocm.
 *
 * @param ptr: pointer to memory block
 */
void freem(void *ptr);

/**
 * @fn mmem_init
 *
 * @brief initialize memory management subsystem.
 */
void mmem_init(void);

#endif
