run :
	chmod +x compile.sh
	chmod +x build.sh
	chmod +x src/modules/mod_asm.lnk
	./compile.sh
	./build.sh

clean :
	# Deleting all build files
	rm -rf build/kernel build/*.o build/*.img

emulate :
	# Run with QEMU
	qemu-system-i386 build/disk.img
