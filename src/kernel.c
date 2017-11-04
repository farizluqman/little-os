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
#include "modules/memory/malloc.h"

#include "modules/pic/pic.c"
#include "modules/desc_tables/gdt.c"
#include "modules/desc_tables/idt.c"
#include "modules/interrupt/isr.c"
#include "modules/keyboard/keyboard.c"
#include "modules/vga/console.c"
#include "modules/syscall/syscall.c"
#include "modules/portio/port.c"
#include "modules/time/pit.c"
#include "modules/memory/malloc.c"

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
// extern uint32_t kernel_end;
// extern uint32_t kernel_base;

// function declarations
void kernel_main(struct multiboot_info *mi, unsigned int magic);
//size_t strlen(const char *str);

typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;

// kernel entry point
void kernel_main(struct multiboot_info *mbt, unsigned int magic) {
    // Initialize terminal interface
    console_initialize();

    if (magic == MULTIBOOT_MAGIC_NUMBER)
  		printf("magic number match. %x\n", magic);
  	else {
  		printf("magic number fail. expected: %x got: %x\n", MULTIBOOT_MAGIC_NUMBER, magic);
  		printf("memory management is unavailable. Did you boot from grub?\n");
  		return;
  	}

    //printf("Kernel base is %x, end is %x\n", &kernel_base, &kernel_end);

    console_writestring("\n");

    //mm_init(&kernel_end);

    // Initialize the GDT
    console_writestring("Initializing GDT...\n");
    init_gdt();

    // Remap the PIC so when we start the interrupter our machine will not
    // go KABOOM!
    console_writestring("Remapping PIC...\n");
    remap_pic();

    console_writestring("Initializing IDT...\n");
    init_idt();

    console_writestring("Initializing PIT...\n");
    init_timer(100); // 50 hz

    //console_movecursor(0, 0);
    console_writestring("Initializing I/O peripherals...\n");
    initialize_keyboard(); // will soon be more than keyboard

    init_tty();

    while (1);
}
