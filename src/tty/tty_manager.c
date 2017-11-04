#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty/tty_manager.h"
#include "stdlib/stdio.h"

#include "tty/tty0.c"

const char *AVAILABLE_COMMANDS[] = {
  "clear",
  "host",
  "\0"
};

// function declarations
void display_current_tty(void);
void refresh_tty(char c);

int current_tty;

char *buffer_str = "";
int write_flag = 0; // the flag used whether to write output on console or not
size_t left = 0;

void init_tty(){
  // on which tty we should go?
  tty0();
}

// Our very own Teletype Terminal manager
// will replace with the tty id soon... very soon!
void display_current_tty() {
  printf("tty%d > ", current_tty);
}

void set_current_tty(int i)
{
  current_tty = i;
}

void check_tty()
{
  int found = 0;
  int clear = 0; // if requested to clear screen

  if (strcmp(buffer_str, "clear") == 0) {
    found = 1;
    clear = 1;
    clear_screen();
  }

  if (strcmp(buffer_str, "ls") == 0) {
    found = 1;
    console_writestring("Not implemented");
  }

  if (strcmp(buffer_str, "malloc") == 0) {
    found = 1;
    console_writestring("Allocating 1024 bytes... \n");
    malloc(1024);
  }

  if (strcmp(buffer_str, "test") == 0) {
    found = 1;
    console_writestring("Allocating 1024 bytes... \n");
    char* mem = malloc(1024);
    console_writestring("\nCheck memory stat... \n");
    mm_print_out();
    console_writestring("Freeing the memory... \n");
    free(mem);
    console_writestring("\nCheck memory stat... \n");
    mm_print_out();
  }

  if (strcmp(buffer_str, "memory") == 0 || strcmp(buffer_str, "mem") == 0) {
    found = 1;
    mm_print_out();
    //pit_wait(5);
  }

  if (strcmp(buffer_str, "crash") == 0) {
    found = 1;
    console_writestring("Allocating more bytes than what you have... DON'T DO IT DAVE!!!!... \n");
    malloc(33554432);
  }

  if (strcmp(buffer_str, "__dev") == 0) {
    found = 1;
    printf("This is a dev %d ", 10);
  }

  if (strcmp(buffer_str, "reboot") == 0) {
    reboot();
  }

  if (found == 0 && left > 0){
    // user typed something but command not found
    printf("Command not found\n");
  } else if (found == 0 && left == 0) {
    // user did nothing but only enter
  } else if (clear == 0){
    // command is found and application is executed
    console_writestring("\n");
  }
}

void refresh_tty(char c)
{
  switch (c)
  {
    case '\b': // backspace
      if(left == 0){ // protect from "over deleting" characters
        buffer_str[0] = '\0'; // clear buffer
      } else {
        console_putchar(c); // only "delete" if there are characters on the left
        left--;
        buffer_str[left] = '\0'; // clear character from buffer
      }
      break;
    case '\n': // newline
    case '\r': // carriage return
      console_writestring("\n");
      check_tty();
      buffer_str[0] = '\0';
      left = 0;
      display_current_tty();
      break;
    default:
      console_putchar(c);
      left++;
      append(buffer_str, c);
  }
}
