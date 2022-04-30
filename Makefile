CC = /usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
LD = /usr/local/x86_64elfgcc/bin/x86_64-elf-ld

albert_os: bootsector.bin kernel.bin
	cat $^ > $@.bin

bootsector.bin: bootsector/bootsector.asm
	nasm -f bin bootsector/bootsector.asm -o bootsector.bin

kernel.bin: kernel_entry.o kernel.o
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

kernel_entry.o: kernel/kernel_entry.asm
	nasm $< -f elf64 -o $@

kernel.o: kernel/kernel.c
	$(CC) -ffreestanding -mno-red-zone -m64 -c $< -o $@

clean:
	rm -rf *.bin *.o