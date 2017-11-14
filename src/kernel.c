#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Teletype Terminal Manager
#include "stdlib/stdio.h"
#include "tty/manager/tty_manager.h"
#include "modules/interrupt/isr.h"
#include "modules/multiboot/multiboot.h"
#include "modules/bios/realmode.h"
#include "modules/portio/port.h"
#include "modules/desc_tables/gdt.h"
#include "modules/memory/phymem.h"
#include "modules/vga/console.h"
#include "stdlib/common.h"

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif

#if defined(__cplusplus)
extern "C"  /* Use C linkage for kernel_main. */
#endif

extern void pause();
// extern char _binary_f32_disk_start;
//
extern uint32_t kernel_end;
extern uint32_t kernel_base;

// function declarations
void kernel_main(struct multiboot_info *mbt, unsigned int magic);
//size_t strlen(const char *str);

// kernel entry point
void kernel_main(struct multiboot_info *mbt, unsigned int magic) {

    uint32_t low_pages = 256;                           // 1024 * 1024 bytes / 4096
    uint32_t high_pages = (mbt->mem_upper * 1024) / 4096 + 30000;

    uint32_t total_frames = high_pages + low_pages;

    // Initialize terminal interface
    console_initialize();

    if (magic == MULTIBOOT_MAGIC_NUMBER) {
        printf("Magic number match. %x\n", magic);
    } else {
        printf("Magic number fail. expected: %x got: %x\n", MULTIBOOT_MAGIC_NUMBER, magic);
        printf("Memory management is unavailable. Did you boot from grub?\n");
        return;
    }

    printf("Kernel base is %x, end is %x\n", &kernel_base, &kernel_end);

    multiboot_memory_map_t* mmap = mbt->mmap_addr;
    while(mmap < mbt->mmap_addr + mbt->mmap_length) {
        mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
    }

    printf("RAM size: %d MB \n",  mbt->mem_upper / 1024 + 2); // +2 kernel memory
    printf("PAGING size: %d \n",  high_pages); // +2 kernel memory

    // Initialize the GDT
    console_writestring("[GDT] Initializing GDT... ");
    init_gdt();

    // Remap the PIC so when we start the interrupter our machine will not
    // go KABOOM!
    console_writestring("[PIC] Remapping PIC... ");
    remap_pic();

    console_writestring("[IDT] Initializing IDT... ");
    init_idt();

    console_writestring("[PIT] Initializing PIT... ");

    initialize_paging(total_frames, 0, 0);
    malloc_stats();

    init_timer(100); // 100 MHz

    //mm_init(&kernel_end);

    //console_movecursor(0, 0);
    console_writestring("Initializing I/O peripherals... ");
    initialize_keyboard(); // will soon be more than keyboard

    while (1) {
        initialize_tty();
    }

}
