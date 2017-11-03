#!/bin/bash
GCC="i686-elf-gcc"

# Set GCC flag to -I src to have the "Namespacing" style in C++ or Java
GCC_FLAGS="-I src"
AS="i686-elf-as"
LD="i686-elf-ld"
NASM="nasm"
NASM_FLAGS="-f elf32"
MODULES_DIR="modules"

echo "$(tput setaf 5)Assembling boot loader (the entry_point)... $(tput sgr0)"
${AS} src/boot/entry_point.s -o build/entry_point.o

echo "$(tput setaf 5)Load module assembly link configurations... $(tput sgr0)"
. src/${MODULES_DIR}/mod_asm.lnk

OBJECTS_TO_LINK=()

## Ugly?
## I really could use your help
## https://github.com/farizluqman/little-os/pulls

## now loop through the above array
for i in "${MOD_ASM_LINKS[@]}"
do
  echo "$(tput setaf 3)      Assembling ${i} using ${AS} ... $(tput sgr0)"
  # echo "${AS} src/${i} -o build/${i}o"
  base=$(basename $i)
  file=$(echo ${base}|cut -f1 -d".")
  OBJECTS_TO_LINK+=("${file}.o")
  ${AS} src/${i} -o build/${file}.o
done

## now loop through the above array
for i in "${MOD_NASM_LINKS[@]}"
do
  echo "$(tput setaf 3)      Assembling ${i} using ${NASM} ... $(tput sgr0)"
  # echo "${AS} src/${i} -o build/${i}o"
  base=$(basename $i)
  file=$(echo ${base}|cut -f1 -d".")
  OBJECTS_TO_LINK+=("${file}.o")
  ${NASM} ${NASM_FLAGS} src/${i} -o build/${file}.o
done

OBJS_STRING=""

## now loop through the above array
for i in "${OBJECTS_TO_LINK[@]}"
do
  OBJS_STRING="${OBJS_STRING} build/${i}"
done

echo "$(tput setaf 5)Compiling the kernel $(tput sgr0)"
# Compile the kernel.c
${GCC} ${GCC_FLAGS} -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -c src/kernel/kernel.c -o build/kernel_c.o

echo "$(tput setaf 5)Linking entry point with the kernel and all object files $(tput sgr0)"

# Link the entry point with the kernel
${LD} -m elf_i386 -T src/link.ld -o build/kernel build/entry_point.o build/kernel_c.o ${OBJS_STRING}
