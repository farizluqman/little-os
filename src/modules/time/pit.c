#include "modules/time/pit.h"
#include "modules/interrupt/isr.h"
#include "modules/portio/port.h"
#include "stdlib/stdio.h"

#define PIT_NATURAL_FREQ 1193180

#define PIT_DATA0 0x40
#define PIT_DATA1 0x41
#define PIT_DATA2 0x42
#define PIT_COMMAND 0x43

#define DRAW_MODULO 10
uint8_t draw_modcount = 0;

uint8_t normal_color;

static long SYSTEM_TICK = 0;

extern uint32_t allocated_frames;
extern uint32_t total_frames;
extern uint32_t heap_free;
extern uint32_t allocations;

#define BUFFSIZE 77

static uint8_t task = 0;
static uint8_t task_was_on = 0;

void set_task(uint8_t i)
{
	if(!task_was_on) return;
	task = i;
}

static void timer_callback(registers_t regs)
{
  long freq = 100;
  SYSTEM_TICK++;
  if(mod(SYSTEM_TICK, freq) == 0){
    int sec = SYSTEM_TICK / freq;
    int sec_mod = mod(sec, 60);
    int min = sec / 60;

    char* outstring;
    sprintf(outstring, "Uptime: %02dm, %02ds", min, sec_mod);
    console_putstringat(outstring, 3, 0, 63);
  }
}

void init_timer(uint32_t frequency)
{
    printf("Initializing PIT timer\n");
    register_interrupt_handler(IRQ0, &timer_callback);

    //normal_color = make_color(COLOR_WHITE, COLOR_BLACK);

    uint32_t divisor;
    if(frequency)
        divisor = PIT_NATURAL_FREQ / frequency;
    else
        divisor = 0;
    /*
      http://wiki.osdev.org/Programmable_Interval_Timer#I.2FO_Ports

      Channel  Access Mode        Operating Mode    BCD/Binary
      0x36 == 0011 0110 == 00       11 (lobyte/hibyte) 011 (square Wave) 0 (16-bit bin
      )
    */
    outb(PIT_COMMAND, 0x36);

    //Chop freq up into bytes and send to data0 port
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(PIT_DATA0, low);
    outb(PIT_DATA0, high);
}
