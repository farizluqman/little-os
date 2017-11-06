#ifndef MULTIBOOT_H
#define MULTIBOOT_H

//! The symbol table for a.out format.
typedef struct {
    unsigned long tab_size;
    unsigned long str_size;
    unsigned long address;
    unsigned long reserved;
} aout_symbol_table_t;

//! The section header table for ELF format.
typedef struct {
    unsigned long num;
    unsigned long size;
    unsigned long address;
    unsigned long shndx;
} elf_section_header_table_t;

struct multiboot_info
{
    unsigned long flags;
    unsigned long mem_lower;
    unsigned long mem_upper;
    unsigned long boot_device;
    unsigned long cmdline;
    unsigned long mods_addr;
    union
    {
        aout_symbol_table_t aout_sym_t;
        elf_section_header_table_t elf_sec_t;
    } u;
    unsigned long mmap_length;
    unsigned long mmap_addr;
};

typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;

#endif
