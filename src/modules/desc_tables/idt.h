#ifndef IDT_H
#define IDT_H

void init_idt();

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();


// IRQ Handlers
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

//    0 - Division by zero exception
//    1 - Debug exception
//    2 - Non maskable interrupt
//    3 - Breakpoint exception
//    4 - 'Into detected overflow'
//    5 - Out of bounds exception
//    6 - Invalid opcode exception
//    7 - No coprocessor exception
//    8 - Double fault (pushes an error code)
//    9 - Coprocessor segment overrun
//    10 - Bad TSS (pushes an error code)
//    11 - Segment not present (pushes an error code)
//    12 - Stack fault (pushes an error code)
//    13 - General protection fault (pushes an error code)
//    14 - Page fault (pushes an error code)
//    15 - Unknown interrupt exception
//    16 - Coprocessor fault
//    17 - Alignment check exception
//    18 - Machine check exception
//    19-31 - Reserved

#define DIVISION_BY_ZERO            0
#define DEBUG_EXCEPTION             1
#define NON_MASKABLE_INTERRUPT      2
#define BREAKPOINT_EXCEPTION        3
#define INTO_DETECTED_OVERFLOW      4
#define OUT_OF_BOUNDS_EXCEPTION     5
#define INVALID_OPCODE_EXCEPTION    6
#define NO_COPROCESSOR_EXCEPTION    7
#define DOUBLE_FAULT                8
#define COPROCESSOR_SEGMENT_OVERRUN 9
#define BAD_TSS                     10
#define SEGMENT_NOT_PRESENT         11
#define STACK_FAULT                 12
#define GENERAL_PROTECTION_FAULT    13
#define PAGE_FAULT                  14
#define UNKNOWN_INTERRUPT_EXCEPTION 15
#define COPROCESSOR_FAULT           16
#define ALIGNMENT_CHECK_EXCEPTION   17
#define MACHINE_CHECK_EXCEPTION     18

#endif
