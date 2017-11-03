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

char *buffer_str = "";

size_t left = 0;
const char *AVAILABLE_COMMANDS[] = {
  "clear",
  "host",
  "\0"
};

void check_tty()
{
  if (strcmp(buffer_str, "clear") == 0) {
    clear_screen();
  }

  if (strcmp(buffer_str, "ls") == 0) {
    console_writestring("Not implemented\n\n");
  }
}

void refresh_tty(char c)
{
  switch (c)
  {
    case '\b': // backspace
      // !todo protect from deleting extra characters
      if(left == 0){
        buffer_str[0] = '\0';
      } else {
        console_putchar(c);
        left--;
        buffer_str[left] = '\0';
      }
      break;
    case '\n': // newline
    case '\r': // carriage return
      console_putchar(c);
      check_tty();
      buffer_str[0] = '\0';
      left = 0;
      display_current_tty();
      break;
    default:
      console_putchar(c);
      left++;
      append(buffer_str, c);
      break;
  }
}
