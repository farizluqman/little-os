#include "stdlib/stdio.h"
#include "modules/vga/console.h"
#include "stdlib/common.h"
#include <stdarg.h>

int printf(char *fmt, ...) {
    //return;
    va_list argp;
    va_start(argp, fmt);

    char *p;
    for(p = fmt; *p != 0; p++) {
        if(*p != '%') {
            console_putchar(*p);
            continue;
        }
        p++; // Skip the %
        int i;
        char *s;
        switch(*p) {
        case 'c':
            i = va_arg(argp, int);
            console_putchar(i);
            break;
        case 's':
            s = va_arg(argp, char *);
            console_writestring(s);
            break;
        case 'd':
            i = va_arg(argp, int);
            console_write_dec(i);
            break;
        case '0': // integer with leading zeros
            i = va_arg(argp, int);

            int num = atoi(i); // convert string to integer

            p++; // we have got the 0 and want to proceed to know how many leading zeros

            /**
             * Looking crazy?
             * why not you https://github.com/farizluqman/little-os/pulls
             */
            switch(*p){
              case '1':
                break;
              case '2':
                if(num < 10){
                  console_write_dec(0);
                }
                break;
              case '3':
                if(num < 10){
                  console_write_dec(0);
                  console_write_dec(0);
                } else if (num < 100){
                  console_write_dec(0);
                }
                break;
              case '4':
                if(num < 10){
                  console_write_dec(0);
                  console_write_dec(0);
                  console_write_dec(0);
                } else if (num < 100){
                  console_write_dec(0);
                  console_write_dec(0);
                } else if (num < 1000) {
                  console_write_dec(0);
                }
                break;
              case '5':
                if(num < 10){
                  console_write_dec(0);
                  console_write_dec(0);
                  console_write_dec(0);
                  console_write_dec(0);
                } else if (num < 100){
                console_write_dec(0);
                console_write_dec(0);
                console_write_dec(0);
                } else if (num < 1000) {
                  console_write_dec(0);
                  console_write_dec(0);
                } else if (num < 10000) {
                  console_write_dec(0);
                }
                break;
            }

            console_write_dec(i);

            p++;

            break;
        case 'x':
            i = va_arg(argp, int);
            console_write_hex(i);
            break;
        case '%':
            console_putchar('%');
            break;
        default:
            console_putchar('%');
            console_putchar(*p);
            break;
        }
    }
    return 0;
}

int sprintf(char *str, char *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);

    char *p;
    for(p = fmt; *p != 0; p++) {
        if(*p != '%') {
            *str++ = *p;
            continue;
        }
        p++; // Skip the %
        int i;
        char *s;
        switch(*p) {
        case 'c':
            i = va_arg(argp, int);
            *str++ = i;
            break;
        case 's':
            s = va_arg(argp, char *);
            while(*s) {
                *str++ = *s++;
            }
            break;
        case 'd':
            i = va_arg(argp, int);
            char decbuff[13]; // At most 12 decimal places for 32 bit int.
            char *dec = itos(i, decbuff, 13);
            while(*dec) {
                *str++ = *dec++;
            }
            break;
        case 'x':
            i = va_arg(argp, int);
            for(int j = 28; j >= 0; j-=4)
            {
                *str++ = hex_char(i>>j);
            }
            break;

        case '0': // integer with leading zeros
            i = va_arg(argp, int);

            char *num_str = itos(i, decbuff, 13); // get integer from the function argument
            int num = atoi(num_str); // convert string to integer

            p++; // we have got the 0 and want to proceed to know how many leading zeros

            /**
             * Looking crazy?
             * why not you https://github.com/farizluqman/little-os/pulls
             */
            switch(*p){
              case '1':
                break;
              case '2':
                if(num < 10){
                  *str++ = '0';
                }
                break;
              case '3':
                if(num < 10){
                  *str++ = '0';
                  *str++ = '0';
                } else if (num < 100){
                  *str++ = '0';
                }
                break;
              case '4':
                if(num < 10){
                  *str++ = '0';
                  *str++ = '0';
                  *str++ = '0';
                } else if (num < 100){
                  *str++ = '0';
                  *str++ = '0';
                } else if (num < 1000) {
                  *str++ = '0';
                }
                break;
              case '5':
                if(num < 10){
                  *str++ = '0';
                  *str++ = '0';
                  *str++ = '0';
                  *str++ = '0';
                } else if (num < 100){
                  *str++ = '0';
                  *str++ = '0';
                  *str++ = '0';
                } else if (num < 1000) {
                  *str++ = '0';
                  *str++ = '0';
                } else if (num < 10000) {
                  *str++ = '0';
                }
                break;
            }

            while(*num_str) {
                *str++ = *num_str++;
            }

            p++;

            break;
        case '%':
            *str++ = '%';
            break;
        default:
            *str++ = '%';
            *str++ = *p;
            break;
        }
    }
    *str++ = 0;
    return 0;
}

void set_status(char *str) {
    // if(console_set_status) {
    //     console_set_status(str);
    // }
}
