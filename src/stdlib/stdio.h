#ifndef STDIO_H
#define STDIO_H

/**
 * Supports %c %s %d and %x
 */
int __divdi3(long a, long b);
int printf(char *fmt, ...);
int sprintf(char *str, char *fmt, ...);
void set_status(char *str);

#endif
