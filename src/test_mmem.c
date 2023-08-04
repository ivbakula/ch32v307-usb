#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "mmem.h"

void mmem_init(void);
void *allocm(uint32_t size);
void freem(void *ptr);

uint8_t heap[256];
extern header *blk_list;

uint32_t _heap_start;
uint32_t _heap_end;

#define TO_HEADER ((header *)&heap[0])

typedef struct
{
  uint32_t a;
  uint32_t b;
  uint32_t c;
  uint32_t d;
} AlignedStruct;

void dump_heap(void)
{
  header *h = blk_list;
  while (GET_PTR(h)) {
    printf("HEADER: 0x%08tx\n is_occupied: %d\n size: %d\n next: 0x%08tx\n ", (uintptr_t)GET_PTR(h), (int)GET_STATUS(h),
      GET_PTR(h)->size, (uintptr_t)GET_PTR(h)->next);
    h = GET_PTR(h)->next;
  }
}

void hexdump_heap(void)
{
  header *h = blk_list;
  while (GET_PTR(h)) {
    puts("=================\n");
    printf("BLOCK: 0x%08tx\n", (uintptr_t)GET_PTR(h));
    uint8_t *c = (char *)GET_PTR(h);
    for (int i = 0; i < GET_PTR(h)->size + sizeof(header); i++) {
      printf("%02x", c[i]);
      if (i % 4 == 0)
        putchar('\n');
    }
    puts("\n=================\n");
    h = GET_PTR(h)->next;
  }
}

void test_allocm(void)
{
  mmem_init();
  const uint32_t blksz = sizeof(char) * 32;
  char *ptr = (char *)allocm(blksz);

  /* Check if first block is marked as alloced */
  assert(GET_STATUS(blk_list) == BLK_ALLOCD);

  /* Check allocd block size and pointer */
  header *blk = GET_PTR(blk_list);
  assert(blk->size == blksz);
  //  assert((char *)&blk->block[0] == ptr);

  /* Check if offset to the next block is right */
  assert(blk->next == (header *)(ptr + blksz)); /* +4 because header starts after the block */

  /* check next block */
  blk = blk->next;
  dump_heap();
  assert(blk->size == sizeof(heap) - 2 * sizeof(header) - blksz);
  assert(blk->next == 0);
}

void alloc_blocks(void)
{
}

void test_allocm_stress(void)
{
  mmem_init();
  AlignedStruct *astr = (AlignedStruct *)allocm(sizeof(*astr));
  astr->a = 0xdeadbeef;
  astr->b = 0xacabc0fe;
  astr->c = 0xff00ff00;
  astr->d = 0xaefbacff;

  assert(astr->a == 0xdeadbeef);
  assert(astr->b == 0xacabc0fe);
  assert(astr->c == 0xff00ff00);
  assert(astr->d == 0xaefbacff);

  AlignedStruct *bstr = (AlignedStruct *)allocm(sizeof(*bstr));
  bstr->a = 0x12345678;
  bstr->b = 0xaaaaaaaa;
  bstr->c = 0xbbbbbbbb;
  bstr->d = 0xcccccccc;

  assert(bstr->a == 0x12345678);
  assert(bstr->b == 0xaaaaaaaa);
  assert(bstr->c == 0xbbbbbbbb);
  assert(bstr->d == 0xcccccccc);

  AlignedStruct *cstr = (AlignedStruct *)allocm(sizeof(*cstr));
  cstr->a = 0xdddddddd;
  cstr->b = 0xeeeeeeee;
  cstr->c = 0xffffffff;
  cstr->d = 0xfaceb007;

  assert(cstr->a == 0xdddddddd);
  assert(cstr->b == 0xeeeeeeee);
  assert(cstr->c == 0xffffffff);
  assert(cstr->d == 0xfaceb007);

  AlignedStruct *dstr = (AlignedStruct *)allocm(sizeof(*dstr));
  dstr->a = 0xaefbacff;
  dstr->b = 0xff0ff00f;
  dstr->c = 0xacabc0fe;
  dstr->d = 0xdeadbeef;

  assert(dstr->a == 0xaefbacff);
  assert(dstr->b == 0xff0ff00f);
  assert(dstr->c == 0xacabc0fe);
  assert(dstr->d == 0xdeadbeef);

  assert(astr->a == 0xdeadbeef);
  assert(astr->b == 0xacabc0fe);
  assert(astr->c == 0xff00ff00);
  assert(astr->d == 0xaefbacff);

  assert(bstr->a == 0x12345678);
  assert(bstr->b == 0xaaaaaaaa);
  assert(bstr->c == 0xbbbbbbbb);
  assert(bstr->d == 0xcccccccc);

  assert(cstr->a == 0xdddddddd);
  assert(cstr->b == 0xeeeeeeee);
  assert(cstr->c == 0xffffffff);
  assert(cstr->d == 0xfaceb007);

  /* freem(cstr); */
  /* freem(bstr); */
  /* freem(dstr); */
  //  hexdump_heap();
  dump_heap();
}

void test_free(void)
{
  mmem_init();
  AlignedStruct *astr = (void *)allocm(sizeof(*astr));
  AlignedStruct *bstr = (void *)allocm(sizeof(*bstr));
  AlignedStruct *cstr = (void *)allocm(sizeof(*cstr));
  AlignedStruct *dstr = (void *)allocm(sizeof(*dstr));

  freem(cstr);
  freem(bstr);
  freem(dstr);

  dump_heap();
}

void test_hole_1(void)
{
  mmem_init();
  AlignedStruct *astr = (void *)allocm(sizeof(*astr));
  AlignedStruct *bstr = (void *)allocm(sizeof(*bstr));
  AlignedStruct *cstr = (void *)allocm(sizeof(*cstr));
  AlignedStruct *dstr = (void *)allocm(sizeof(*dstr));

  freem(cstr);

  char *str = (char *)allocm(sizeof(char) * 64);
  dump_heap();
}

void test_hole_2(void)
{
  mmem_init();
  AlignedStruct *astr = (void *)allocm(sizeof(*astr));
  AlignedStruct *bstr = (void *)allocm(sizeof(*bstr));
  AlignedStruct *cstr = (void *)allocm(sizeof(*cstr));
  AlignedStruct *dstr = (void *)allocm(sizeof(*dstr));

  freem(bstr);
  freem(cstr);

  char *str = (char *)allocm(sizeof(char) * 32);
  dump_heap();
}

int main()
{
  test_hole_2();
  //  test_free();
  //  test_allocm();
  //  test_allocm_stress();
}
