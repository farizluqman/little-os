#include "../headers/console.h"

// constants
static const size_t VGA_HEIGHT = 25;
static const size_t VGA_WIDTH = 80;

// global variables
size_t console_row;
size_t console_column;
uint8_t console_color;
uint16_t *console_buffer;

// initializes the terminal global variables, leaves the screen blank
void console_initialize(void) {
    console_row = 0;
    console_column = 0;
    console_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    console_buffer = (uint16_t *) 0xB8000; // VGA text buffer address
    // fill screen with spaces
    for (size_t i = 0; i < VGA_HEIGHT; ++i) {
        for (size_t j = 0; j < VGA_WIDTH; ++j) {
            console_putentryat(' ', console_color, i, j);
        }
    }
}

// prints a string to the terminal
void console_writestring(const char *data) {
    while (*data) {
        console_putchar(*data++);
    }
    // busy waiting, to view individual lines as they appear
    // for (size_t i = 0; i < 1e7; ++i)
    //     ;
}

void console_putnum(int n) {
    int div = n / 10;
    if (div != 0) {
        console_putnum(div);
    }
    console_putchar('0' + n % 10);
}

// prints a character to the terminal
void console_putchar(char c) {
    switch (c) {
        case '\b': // backspace
          --console_column;
          console_putchar(' ');
          --console_column;
          break;
        case '\n': // newline
            ++console_row;
            console_column = 0;
            break;
        case '\r': // carriage return
            console_column = 0;
            break;
        default:
            console_putentryat(c, console_color, console_row, console_column);
            ++console_column;
            break;
    }
    // past the far right of the screen, move cursor to the next line
    if (console_column == VGA_WIDTH) {
        console_column = 0;
        ++console_row;
    }
    // past the bottom of the screen, scroll down, leave cursor in the last line
    if (console_row == VGA_HEIGHT) {
        console_scrolldown();
        --console_row;
    }

    update_cursor(console_column, console_row);
}

// gives the linear index into the VGA buffer corresponding to a certain (i, j)
// position
static inline size_t index(size_t i, size_t j) {
    return VGA_WIDTH * i + j;
}

// scrolls the terminal down a single line of text
void console_scrolldown(void) {
    // copy every line to the one above
    for (size_t i = 0; i < VGA_HEIGHT-1; ++i) {
        for (size_t j = 0; j < VGA_WIDTH; ++j) {
            console_buffer[index(i, j)] = console_buffer[index(i+1, j)];
        }
    }
    // fill last line with spaces
    for (size_t j = 0; j < VGA_WIDTH; ++j) {
        console_putentryat(' ', console_color, VGA_HEIGHT-1, j);
    }
}

// puts a char with the given color at the specified terminal location
void console_putentryat(char c, uint8_t color, size_t i, size_t j) {
    console_buffer[index(i, j)] = make_vgaentry(c, color);
}

// sets the color of the text
void console_setcolor(uint8_t color) {
    console_color = color;
}

// creates a vga byte pair from the given character and VGA color byte
uint16_t make_vgaentry(char c, uint8_t color) {
    uint16_t c16 = c;
    uint16_t color16 = color;
    return color16 << 8 | c16;
}

// creates the VGA text attribute byte from the specified foreground and
// background colors
uint8_t make_color(vga_color fg, vga_color bg) {
    return bg << 4 | fg;
}

// variable names are based on documentation from the FreeVGA Project

// VGA I/O ports
static const uint16_t CRTC_ADDR_PORT = 0x03D4;
static const uint16_t CRTC_DATA_PORT = 0x03D5;

// VGA CRTC registers
static const uint8_t CURSOR_LOCATION_HI_REG = 0x0E;
static const uint8_t CURSOR_LOCATION_LO_REG = 0x0F;

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
