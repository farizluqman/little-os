#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "modules/bios/realmode.h"
#include "modules/portio/port.h"
#include "modules/interrupt/isr.h"
#include "modules/desc_tables/gdt.h"
#include "modules/vga/console.h"
#include "stdlib/common.h"
#include "modules/multiboot/multiboot.h"
//#include "modules/memory/malloc.h"

#include "modules/pic/pic.c"
#include "modules/desc_tables/gdt.c"
#include "modules/desc_tables/idt.c"
#include "modules/interrupt/isr.c"
#include "modules/keyboard/keyboard.c"
#include "modules/vga/console.c"
#include "modules/syscall/syscall.c"
#include "modules/portio/port.c"
#include "modules/time/pit.c"

#include "modules/memory/kheap.c"
#include "modules/memory/paging.c"
#include "modules/memory/kmalloc_early.c"
//#include "modules/memory/malloc.c"
#include "modules/memory/frame.c"

// Include standard libraries
#include "stdlib/common.c"
#include "stdlib/string.c"
#include "stdlib/stdio.c"
#include "stdlib/math.c"

#define MULTIBOOT_MAGIC_NUMBER 0x2BADB002

// Teletype Terminal Manager
#include "tty/tty_manager.c"

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler"
#endif


#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
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

		uint32_t low_pages = 256; // 1024 * 1024 bytes / 4096
		uint32_t high_pages = (mbt->mem_upper * 1024) / 4096;

		uint32_t total_frames = high_pages + low_pages;


    // Initialize terminal interface
    console_initialize();

    if (magic == MULTIBOOT_MAGIC_NUMBER)
  		printf("Magic number match. %x\n", magic);
  	else {
  		printf("Magic number fail. expected: %x got: %x\n", MULTIBOOT_MAGIC_NUMBER, magic);
  		printf("Memory management is unavailable. Did you boot from grub?\n");
  		return;
  	}

    printf("Kernel base is %x, end is %x\n", &kernel_base, &kernel_end);

    //mm_init(&kernel_end);

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

    init_timer(100); // 100 MHz

		//mm_init(&kernel_end);
		initialize_paging(total_frames, 0x0010DE98, 1280 * 720 * 4);
		malloc_stats();

    //console_movecursor(0, 0);
    console_writestring("Initializing I/O peripherals... ");
    initialize_keyboard(); // will soon be more than keyboard

    init_tty();

    while (1);
}
