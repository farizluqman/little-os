#ifndef PIT_H
#define PIT_H

#include <stdint.h>

static long SYSTEM_TICK;

void init_timer(uint32_t frequency); //frequency in Hz

#endif
