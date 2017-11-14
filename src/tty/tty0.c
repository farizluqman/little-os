#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty/manager/tty_manager.h"

// function declarations
void tty0(void);

// Our very own first Teletype Terminal
// This is where we challenge ourselves to add multitasking ability
// and it's a bit like the Linux's tty device
void tty0(void) {
    set_current_tty(0);

    console_writestring("\nPress ENTER to login to tty0...\n");

    // do we loop here? I don't know, time will tell
    while (1) {
        char c = get_ascii_char(); // this may provide some weird bugs, need to
                                   // implement our own scanf function
        fetch_tty_input(&c);
    }
}
