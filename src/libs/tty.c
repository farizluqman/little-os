#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// function declarations
void display_current_tty(void);
void refresh_tty(char c);

// Our very own Teletype Terminal manager
// will replace with the tty id soon... very soon!
void display_current_tty(void) {
  console_writestring("tty0 > ");
}

void refresh_tty(char c)
{
  switch (c)
  {
    case '\n': // newline
        display_current_tty();
        break;
    case '\r': // carriage return
        display_current_tty();
        break;
  }
}
