#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// function declarations
void tty0(void);

// Our very own first Teletype Terminal
// This is where we challenge ourselves to add multitasking ability
// and it's a bit like the Linux's tty device
void tty0(void) {

  while (1) {
    char c = get_ascii_char();
    console_putchar((char*)c);
    // looping YAY!!
  }

}
