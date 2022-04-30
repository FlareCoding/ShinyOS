CC = /usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
LD = /usr/local/x86_64elfgcc/bin/x86_64-elf-ld

KERNEL_OFFSET = 0x8000

albert_os: bootsector.bin kernel.bin
	cat $^ > $@.bin

bootsector.bin: bootsector/bootsector.asm
	nasm -f bin bootsector/bootsector.asm -o bootsector.bin

kernel.bin: bootsector_kernel_loader.o kernel.o
	$(LD) -o $@ -Ttext $(KERNEL_OFFSET) $^ --entry _kmain --oformat binary

kernel.o: kernel/kernel.c
	$(CC) -ffreestanding -mno-red-zone -m64 -c $< -o $@

bootsector_kernel_loader.o: bootsector/bootsector_kernel_loader.asm
	nasm $< -f elf64 -o $@

clean:
	rm -rf *.bin *.o