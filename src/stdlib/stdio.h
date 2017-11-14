#ifndef STDIO_H
#define STDIO_H

/**
 * Supports %c %s %d and %x
 */
int sprintf(char *str, char *fmt, ...);
void set_status(char *str);
int printf(char *fmt, ...);

#endif
