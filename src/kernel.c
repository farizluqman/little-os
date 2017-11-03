#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "headers/port.h"
#include "headers/console.h"
#include "headers/realmode.h"
#include "headers/gdt.h"
#include "libs/pic.c"
#include "libs/gdt.c"
#include "libs/idt.c"
#include "libs/isr.c"
#include "libs/console.c"
#include "libs/string.c"
#include "libs/keyboard.c"

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
