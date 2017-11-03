run :
	chmod +x compile.sh
	chmod +x build.sh

	./compile.sh
	./build.sh

clean :
	# Deleting all build files
	rm -rf build/kernel build/entry_point_asm.o build/kernel_c.o build/disk.img
