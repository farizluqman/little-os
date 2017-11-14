#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tty/manager/tty_manager.h"
#include "modules/vga/console.h"
#include "modules/memory/kheap.h"
#include "stdlib/stdio.h"

static int current_tty;

char buffer_str[100];
int write_flag = 0; // the flag used whether to write output on console or not
size_t left = 0; // number of characters typed by the user

void initialize_tty()
{
    // on which tty we should go?
    tty0(); // go to tty0
}

void display_current_tty()
{
    printf("tty%d > ", current_tty);
}

int get_current_tty()
{
    return 0;
}

void set_current_tty(int i)
{
    current_tty = i;
}

void process_tty_command()
{
    int found = 0;
    int clear = 0; // if requested to clear screen

    printf("Typed: %s \n", &buffer_str);


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
        console_writestring("Allocating 30 MB...\n");
        kmalloc(1024 * 30000);
    }

    if (strcmp(buffer_str, "memory") == 0 || strcmp(buffer_str, "mem") == 0) {
        found = 1;
        malloc_stats();
        //pit_wait(5);
    }

    if (strcmp(buffer_str, "__dev") == 0) {
        found = 1;
        printf("This is a dev %d ", 10);
        console_writedone();
        console_writefail();
    }

    if (strcmp(buffer_str, "reboot") == 0) {
        reboot();
    }

    if (found == 0 && left > 0) {
        // user typed something but command not found
        printf("Command not found\n");
    } else if (found == 0 && left == 0) {
        // user did nothing but only enter
    } else if (clear == 0) {
        // command is found and application is executed
        console_writestring("\n");
    }
}

void fetch_tty_input(char *c)
{
    switch (*c)
    {
    case '\b': // backspace
        if(left == 0) { // protect from "over deleting" characters
            buffer_str[0] = '\0'; // clear buffer
        } else {
            console_putchar(*c); // only "delete" if there are characters on the left
            left--;
            buffer_str[left] = '\0'; // clear character from buffer
        }
        break;
    case '\n': // newline
    case '\r': // carriage return
        console_writestring("\n");
        process_tty_command();
        buffer_str[0] = '\0';
        left = 0;
        display_current_tty();
        break;
    default:
        console_putchar(*c);
        buffer_str[left] = *c;
        left++;
        buffer_str[left] = '\0';
    }
}
