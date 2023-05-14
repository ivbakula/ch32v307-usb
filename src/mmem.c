#include <stddef.h>
#include "mmem.h"
#include <string.h>

extern uint32_t _heap_start;
extern uint32_t _heap_end;

header *blk_list = 0;
uint8_t *heap_end = 0;
uint32_t heap_size = 0;

#ifndef UNIT_TEST
void mmem_init(void)
{
  memset(&_heap_start, '\0', &_heap_end - &_heap_start);
  blk_list = (header *)&_heap_start;
  heap_size = blk_list->size = &_heap_end - &_heap_start - sizeof(header);
  blk_list->next = 0;
  heap_end = (uint8_t *)&_heap_end;
}
#elif
extern uint8_t heap[256];
void mmem_init(void)
{
  memset(heap, '\0', sizeof(heap));
  blk_list = (header *)&heap[0];
  heap_size = blk_list->size = &heap[255] - &heap[0] - sizeof(header);
  blk_list->next = 0;

  heap_end = (uint8_t *)&heap[255];
}
#endif

static header **find_free(size_t size)
{
  header **h = &blk_list;

  while (GET_PTR(*h))
  {
    if (IS_FREE(*h) && (*h)->size >= size)
      return h;
    h = &GET_PTR(*h)->next;
  }
  return 0;
}

static header **find_prev_hdr(void *ptr)
{
  // Find pointer to the header of memory block.
  header *hdr = (header *)((uintptr_t)ptr - sizeof(header));

  // Find hdr pointer in linked list of memory blocks
  header **h = &blk_list;
  while (GET_PTR(*h))
  {
    if (GET_PTR(GET_PTR(*h)->next) == hdr)
      return h;
    h = &(GET_PTR(*h)->next);
  }
  return 0;
}    

void *allocm(uint32_t size)
{
  size += size % 4; /* just make sure that allocd blocks are 4 bytes aligned */
  header **free_blk = find_free(sizeof(header) + size);

  if (*free_blk == 0)
    return 0;

  /* Divide free block; We won't be using whole block of memory if requested
   * memory block is smaller than free block. Divide free block into two parts. First part
   * will be a block that is allocated and second, memory block that was left after this allocation
   */
  header *nnxt = (header *)((uintptr_t)(*free_blk) + sizeof(header) + size); /* start of free memory block */
  nnxt->size = (*free_blk)->size - size - sizeof(header);

  /* if size of next block is zero, there is no point in creating it. */
  if (nnxt->size > 0)
  {
    nnxt->next = (*free_blk)->next;
    (*free_blk)->next = nnxt;
    (*free_blk)->size = size;
  }    

  void *ret = (void *)((uintptr_t)(*free_blk) + sizeof(header));
  /* Mark our newly allocated block BLK_ALLOCD */

  *free_blk = (header *)((uintptr_t)(*free_blk) | BLK_ALLOCD);
  return ret;
}

void freem(void *ptr)
{
  header **prev = find_prev_hdr(ptr);
  if (!prev)
    return;
  
  header **current = &(GET_PTR(*prev)->next);
  header *next = GET_PTR(*current)->next;

  // Mark current block as free
  *current = (header *)((uintptr_t)(*current) & (~1));

  if (GET_STATUS(next) == BLK_FREED)
  {
    (*current)->size += sizeof(header) + next->size;
    (*current)->next = next->next;
  }

  if (GET_STATUS(*prev) == BLK_FREED)
  {
    (*prev)->size += sizeof(header) + (*current)->size;
    (*prev)->next = (*current)->next;
  }
}    
