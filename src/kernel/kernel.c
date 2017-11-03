#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "modules/iobus/port.h"
#include "modules/cpu/gdt.h"
#include "modules/cpu/realmode.h"
#include "modules/vga/console.h"

#include "modules/cpu/pic.c"
#include "modules/cpu/gdt.c"
#include "modules/cpu/idt.c"
#include "modules/cpu/isr.c"

#include "modules/keyboard/keyboard.c"
#include "modules/vga/console.c"

#include "stdlib/string.c"

#include "tty/tty0.c"


// function declarations
void kernel_main(void);
size_t strlen(const char *str);

// kernel entry point
void kernel_main(void) {
    console_writestring("Initializing console...\n");

    // Initialize terminal interface
    console_initialize();

    // Initialize the GDT
    console_writestring("Initializing GDT...\n");
    init_gdt();

    // Remap the PIC so when we start the interrupter our machine will not
    // go KABOOM!
    console_writestring("Remapping PIC...\n");
    remap_pic();

    console_writestring("Initializing IDT...\n");
    init_idt();

    //console_movecursor(0, 0);
    console_writestring("Initializing I/O peripherals...\n");
    initialize_keyboard(); // will soon be more than keyboard

    tty0();
}
