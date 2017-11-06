/** @author Levente Kurusa <levex@linux.com> **/
#include <stdint.h>
#include "stdlib/stdio.h"
#include "modules/memory/malloc.h"
#include "modules/tasking/tasking.h"
#include "modules/time/pit.h"

#define MAX_PAGE_ALIGNED_ALLOCS 32

uint32_t last_alloc = 0;
uint32_t heap_end = 0;
uint32_t heap_begin = 0;
uint32_t pheap_begin = 0;
uint32_t pheap_end = 0;
uint8_t *pheap_desc = 0;
uint32_t memory_used = 0;

void mm_init(uint32_t kernel_end)
{
	last_alloc = kernel_end + 0x1000;
	heap_begin = last_alloc;
	pheap_end = 0x400000;
	pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
	heap_end = pheap_begin;
	memset((char *)heap_begin, 0, heap_end - heap_begin);
	pheap_desc = (uint8_t *)malloc(MAX_PAGE_ALIGNED_ALLOCS);
	printf("Kernel heap starts at %x\n", last_alloc);
}

void mm_print_out()
{
	uint32_t free_mem = heap_end - heap_begin - memory_used;
	int free_mem_kb = free_mem / 1024;
	printf("Memory used: %d bytes (%d KB)\n", memory_used, memory_used / 1024);
	printf("Memory free: %d bytes (%d KB)\n", free_mem, free_mem_kb);
	printf("Heap size: %d bytes\n", heap_end - heap_begin);
	printf("Heap start: %x\n", heap_begin);
	printf("Heap end: %x\n", heap_end);
	printf("PHeap start: %x\nPHeap end: %x\n", pheap_begin, pheap_end);
}

void free(void *mem)
{
	alloc_t *alloc = (mem - sizeof(alloc_t));
	memory_used -= alloc->size + sizeof(alloc_t);
	alloc->status = 0;
}

void pfree(void *mem)
{
	if(mem < pheap_begin || mem > pheap_end) return;
	/* Determine which page is it */
	uint32_t ad = (uint32_t)mem;
	ad -= pheap_begin;
	ad /= 4096;
	/* Now, ad has the id of the page */
	pheap_desc[ad] = 0;
	return;
}

char* pmalloc(size_t size)
{
	/* Loop through the avail_list */
	for(int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++)
	{
		if(pheap_desc[i]) continue;
		pheap_desc[i] = 1;
		printf("PAllocated from %x to %x\n", pheap_begin + i*4096, pheap_begin + (i+1)*4096);
		return (char *)(pheap_begin + i*4096);
	}
	printf("pmalloc: FATAL: failure!\n");
	return 0;
}

char* malloc(size_t size)
{
	if(!size) return 0;

	/* Loop through blocks and find a block sized the same or bigger */
	uint8_t *mem = (uint8_t *)heap_begin;
	while((uint32_t)mem < last_alloc)
	{
		alloc_t *a = (alloc_t *)mem;
		/* If the alloc has no size, we have reaced the end of allocation */
		//printf("mem=0x%x a={.status=%d, .size=%d}\n", mem, a->status, a->size);
		if(!a->size)
			goto nalloc;
		/* If the alloc has a status of 1 (allocated), then add its size
		 * and the sizeof alloc_t to the memory and continue looking.
		 */
		if(a->status) {
			mem += a->size;
			mem += sizeof(alloc_t);
			mem += 4;
			continue;
		}
		/* If the is not allocated, and its size is bigger or equal to the
		 * requested size, then adjust its size, set status and return the location.
		 */
		if(a->size >= size)
		{
			/* Set to allocated */
			a->status = 1;

			printf("RE:Allocated %d bytes from %x to %x\n", size, mem + sizeof(alloc_t), mem + sizeof(alloc_t) + size);
			memset(mem + sizeof(alloc_t), 0, size);
			memory_used += size + sizeof(alloc_t);
			return (char *)(mem + sizeof(alloc_t));
		}
		/* If it isn't allocated, but the size is not good, then
		 * add its size and the sizeof alloc_t to the pointer and
		 * continue;
		 */
		mem += a->size;
		mem += sizeof(alloc_t);
		mem += 4;
	}

	nalloc:;
	if(last_alloc+size+sizeof(alloc_t) >= heap_end)
	{
		set_task(0);
		char* panic_message;
		sprintf(panic_message, "Cannot allocate %d bytes! Out of memory.\n", size);
		PANIC(panic_message);
	}
	alloc_t *alloc = (alloc_t *)last_alloc;
	alloc->status = 1;
	alloc->size = size;

	last_alloc += size;
	last_alloc += sizeof(alloc_t);
	last_alloc += 4;
	printf("Allocated %d bytes from %x to %x\n", size, (uint32_t)alloc + sizeof(alloc_t), last_alloc);
	memory_used += size + 4 + sizeof(alloc_t);
	memset((char *)((uint32_t)alloc + sizeof(alloc_t)), 0, size);
	return (char *)((uint32_t)alloc + sizeof(alloc_t));
/*
	char* ret = (char*)last_alloc;
	last_alloc += size;
	if(last_alloc >= heap_end)
	{
		set_task(0);
		PANIC("Cannot allocate %d bytes! Out of memory.\n", size);
	}
	printf("Allocated %d bytes from 0x%x to 0x%x\n", size, ret, last_alloc);
	return ret;*/
}

void *memcpy(void *dest, const void *src, size_t n) {

    char *cdest = dest;
    const char *csrc = src;

    size_t i;
    for(i = 0; i < n; i++) {
        cdest[i] = csrc[i];
    }

    return dest;
}

void *memset(void *p, int c, size_t count)
{
    for(unsigned int i = 0; i < count; i++)
    {
        ((char *)p)[i] = c;
    }
    return p;
}
