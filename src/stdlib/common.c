#include "stdlib/common.h"

extern void halt();

int k_toupper(int c) {
    if(c >= 97 && c <= 122) {
        return c - 32;
    }
    return c;
}
int k_tolower(int c) {
    if(c >=65 && c <= 90) {
        return c + 32;
    }
    return c;
}

int coerce_int(char *s, uint32_t *val) {
    uint32_t result = 0;

    while(*s && *s != '\n') {
        if(*s >= 48 && *s <= 57) {
            result *= 10;
            result = result + (*s - 48);
        }
        else {
            return 0;
        }
        s++;
    }
    *val = result;
    return 1;
}

uint8_t hex_char(uint8_t byte)
{
    byte = byte & 0x0F;
    if(byte < 0xA)
    {
        char buff[2];
        itos(byte, buff, 2);
        return buff[0];
    }
    else
    {
        switch(byte)
        {
        case 0x0A:
            return 'A';
            break;
        case 0x0B:
            return 'B';
            break;
        case 0x0C:
            return 'C';
            break;
        case 0x0D:
            return 'D';
            break;
        case 0x0E:
            return 'E';
            break;
        case 0x0F:
            return 'F';
            break;
        }
    }
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {

    char *cdest = dest;
    const char *csrc = src;

    size_t i;
    for(i = 0; i < n; i++) {
        cdest[i] = csrc[i];
    }

    return dest;
}

void *memset(void *p, int c, size_t count)
{
    for(unsigned int i = 0; i < count; i++)
    {
        ((char *)p)[i] = c;
    }
    return p;
}

void PANIC(char *err, ...) {
    printf(err);
    while (1);
}
