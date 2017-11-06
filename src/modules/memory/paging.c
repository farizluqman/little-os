#include <stdint.h>
#include <stddef.h>
#include "modules/interrupt/isr.h"
#include "modules/memory/paging.h"
#include "modules/memory/frame.h"
#include "stdlib/stdio.h"
#include "modules/memory/kmalloc_early.h"
#include "stdlib/common.h"
#include "modules/memory/kheap.h"

struct page_directory *kernel_directory;
struct page_directory *current_directory;
extern uint32_t placement_address;

uint8_t initialized = 0;

void identity_map(uint32_t address, uint32_t length) {
    uint32_t curr = address & 0xFFFFF000;
    while(curr < address + length) {
        struct page *p = get_page(curr, 1, kernel_directory);
        p->present = 1;
        p->rw = 1;
        p->user = 0;
        p->frame = curr >> 12;
        curr += 0x1000;
    }
}

void initialize_paging(uint32_t total_frames, uint32_t ident_addr, uint32_t ident_len) {
    init_frame_allocator(total_frames);

    // Make a page directory for the kernel.
    kernel_directory = (struct page_directory *)e_kmalloc_a(sizeof (struct page_directory));
    memset(kernel_directory, 0, sizeof (struct page_directory));
    current_directory = kernel_directory;

    // Go ahead and allocate all the page tables for the kernel.
    // This is wasteful, but a lot easier than figuring out how to build
    // a kernel page allocator.
    printf("[PAGING] Allocating kernel page tables... ");
    uint32_t i = 0;
    for(i = 0; i < 0xFFFFFFFF;) {
        get_page(i, 1, kernel_directory);
        i += 0x1000 * 1024;
        if(i == 0) {
            break;
        }
    }

    console_writedone();

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.

    // This is hacky. Probably want to do this some other way.
    // Reaching into kmalloc_early and grabbing placement_address
    // is not ideal.
    i = 0;
    while(i < placement_address)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    // Before we do anything else, disable the original kmalloc so we don't
    // start leaking into the address space.
    uint32_t heap_start = disable_early_kmalloc();

    printf("  KHEAP_START: %x\n", heap_start);

    // bootstrap the kheap with INITIAL_HEAP_PAGE_COUNT pages.
    for(i = 0; i < INITIAL_HEAP_PAGE_COUNT; i++) {
        alloc_frame(get_page(heap_start + (i * 0x1000), 1, kernel_directory), 0, 0);
    }

    // Before we enable paging, we must register our page fault handler.
    register_interrupt_handler(14, page_fault);

    if(ident_addr) {
        identity_map(ident_addr, ident_len);
    }

    // Now, enable paging!
    switch_page_directory(kernel_directory);
    initialized = 1;

    printf("[PAGING] Initializing kernel heap... ");
    // Set up the kernel heap!
    initialize_kheap(heap_start);

    console_writedone();

}

void switch_page_directory(struct page_directory *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}


struct page *get_page(uint32_t address, int make, struct page_directory *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32_t tmp;
        if(!initialized)
        {
            dir->tables[table_idx] = (struct page_table *)e_kmalloc_ap(sizeof(struct page_table), &tmp);
        }
        else
        {
            dir->tables[table_idx] = (struct page_table *)kmalloc_ap(sizeof(struct page_table), 1, &tmp);
        }
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return NULL;
    }
}

struct page *get_kernel_page(uint32_t address, int make)
{
    return get_page(address, make, kernel_directory);
}

struct page *map_kernel_page(uint32_t address, int make)
{
    struct page *p = get_page(address, make, kernel_directory);
    if(!p) return NULL;
    alloc_frame(p, 0, 0);
    return p;
}

void unmap_kernel_page(uint32_t address)
{
    struct page *p = get_page(address, 0, kernel_directory);
    if(p) {
        free_frame(p);
    }
}

void page_fault(registers_t regs)
{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;         // Write operation?
    int us = regs.err_code & 0x4;         // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
    //int id = regs.err_code & 0x10;        // Caused by an instruction fetch?

    // Output an error message.
    printf("Page fault! ( ");
    if (present) {printf("present ");}
    if (rw) {printf("read-only ");}
    if (us) {printf("user-mode ");}
    if (reserved) {printf("reserved ");}
    printf(") at %x\n", faulting_address);
    PANIC("Page fault");
}
