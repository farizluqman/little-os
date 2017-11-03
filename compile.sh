# /bin/bash
GCC="i686-elf-gcc"
AS="i686-elf-as"
LD="i686-elf-ld"
NASM="nasm"

echo "$(tput setaf 5)Assembling the Entry Point (kernel.asm) $(tput sgr0)"
# Assemble the entry point (kernel.asm)
${AS} src/entry_point.s -o build/entry_point_asm.o

echo "$(tput setaf 5)Assembling the GDT $(tput sgr0)"
${AS} src/gdt.s -o build/gdt.o

echo "$(tput setaf 5)Assembling the IDT $(tput sgr0)"
${AS} src/idt.s -o build/idt.o

echo "$(tput setaf 5)Assembling the IDT $(tput sgr0)"
${AS} src/port.s -o build/port.o

echo "$(tput setaf 5)Assembling the Real Mode $(tput sgr0)"
${NASM} -f elf32 src/realmode.s -o build/realmode.o

echo "$(tput setaf 5)Compiling the kernel $(tput sgr0)"
# Compile the kernel.c
${GCC} -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -c src/kernel.c -o build/kernel_c.o

echo "$(tput setaf 5)Linking entry point with the kernel $(tput sgr0)"
# Link the entry point with the kernel
${LD} -m elf_i386 -T src/link.ld -o build/kernel build/entry_point_asm.o build/kernel_c.o build/port.o build/gdt.o build/idt.o build/realmode.o
