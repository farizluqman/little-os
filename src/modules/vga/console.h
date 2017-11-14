#ifndef _SCREEN_H
#define _SCREEN_H

#define ESC (0x1B)
#define BS  (0x08)
#define EOT (0x04)

#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5
#define VGA_CURSOR_HIGH  14
#define VGA_CURSOR_LOW   15

#include <stddef.h>
#include <stdint.h>

// hardware text mode color constants
typedef enum vga_color {
    COLOR_BLACK         = 0,
    COLOR_BLUE          = 1,
    COLOR_GREEN         = 2,
    COLOR_CYAN          = 3,
    COLOR_RED           = 4,
    COLOR_MAGENTA       = 5,
    COLOR_BROWN         = 6,
    COLOR_LIGHT_GREY    = 7,
    COLOR_DARK_GREY     = 8,
    COLOR_LIGHT_BLUE    = 9,
    COLOR_LIGHT_GREEN   = 10,
    COLOR_LIGHT_CYAN    = 11,
    COLOR_LIGHT_RED     = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN   = 14,
    COLOR_WHITE         = 15
} vga_color;

// function declarations
void console_initialize(void);
void clear_screen();
void console_writestring(const char *data);
void console_putnum(int n);
void console_putchar(char c);
void console_scrolldown(void);
void console_putentryat(char c, uint8_t color, size_t i, size_t j);
void update_cursor(int x, int y);
void console_setcolor(uint8_t color);
uint16_t make_vgaentry(char c, uint8_t color);
uint8_t make_color(vga_color fg, vga_color bg);

#endif
