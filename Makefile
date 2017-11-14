# run :
# 	chmod +x compile.sh
# 	chmod +x build.sh
# 	chmod +x src/modules/mod_asm.lnk
# 	./compile.sh
# 	./build.sh
#
# clean :
# 	# Deleting all build files
# 	rm -rf build/kernel build/*.o build/*.img
#
# autorun :
# 	chmod +x autorun.sh
# 	./autorun.sh
#
# emulate :
# 	# Run with QEMU
# 	qemu-system-i386 build/disk.img

KERNEL_IMG=kernel.bin

CC=i686-elf-gcc
AS=i686-elf-as
NASM=nasm
LD=i686-elf-gcc
#LD=i686-elf-ld

CFLAGS = -ggdb -m32 -O0 -Wall -Wextra -std=gnu99 -ffreestanding -I src
AFLAGS = -m32 -ggdb
LDFLAGS = $(CFLAGS) -nostdlib -lgcc -Wl,--build-id=none
NASMFLAGS = -f elf32

## END CONFIGURABLE ##

## Gather the necessary assembly files
NASM_FILES=$(shell find . -name \*.nasm -print)
NASM_OBJ=$(patsubst %.nasm,%.o,$(NASM_FILES))

ASM_FILES=$(shell find . -name \*.s -print)
ASM_OBJ=$(patsubst %.s,%.o,$(ASM_FILES))

## Gather the necessary C files
C_FILES=$(shell find . -name \*.c -print)
C_OBJ=$(patsubst %.c,%.o,$(C_FILES))
DEPFILES=$(patsubst %.c,./deps/%.d,$(C_FILES))

OBJECTS=$(ASM_OBJ) $(C_OBJ)

# clean:
# 	-@find . -name "*.o" -type f -delete
# 	-@rm -R deps

all: $(KERNEL_IMG)

## Depfiles contain inter-file dependencies
-include $(DEPFILES)

clean:
	-@find . -name "*.o" -type f -delete
	-@rm -R deps
	-@rm -rf disk.img kernel.bin
	# -@rm f32.disk

$(KERNEL_IMG) : $(OBJECTS) link.ld
	sudo chmod +x build.sh
	./build.sh

## $(basename $(NASM_FILES))
%.o : %.nasm
	@echo -e "[MM]\t\t" $@
	$(NASM) $(NASMFLAGS) -o $@ $<

link.ld : $(NASM_OBJ)
	$(LD) $(LDFLAGS) -T src/link.ld -o $(KERNEL_IMG) $(OBJECTS)

deps/%.d : %.c
	@mkdir -p deps
	@mkdir -p `dirname $@`
	@echo -e "[MM]\t\t" $@
	@$(CC) $(CFLAGS) -MM $< -MF $@
