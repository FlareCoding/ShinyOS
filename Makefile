CC = /usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
LD = /usr/local/x86_64elfgcc/bin/x86_64-elf-ld

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}

CFLAGS = -g -I ./

KERNEL_OFFSET = 0x8000

albert_os: bootsector.bin kernel.bin kernel.elf
	cat bootsector.bin kernel.bin > $@.bin

bootsector.bin: bootsector/bootsector.asm
	nasm -f bin bootsector/bootsector.asm -o bootsector.bin

kernel.bin: bootsector_kernel_loader.o ${OBJ}
	$(LD) -o $@ -Ttext $(KERNEL_OFFSET) $^ --entry _kmain --oformat binary

# Used for debugging purposes
kernel.elf: bootsector_kernel_loader.o ${OBJ}
	$(LD) -o $@ -Ttext $(KERNEL_OFFSET) $^

bootsector_kernel_loader.o: bootsector/bootsector_kernel_loader.asm
	nasm $< -f elf64 -o $@

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -mno-red-zone -m64 -c $< -o $@

clean:
	rm -rf *.bin *.o kernel/*.o