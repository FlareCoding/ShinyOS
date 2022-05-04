CC = /usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
LD = /usr/local/x86_64elfgcc/bin/x86_64-elf-ld

C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}

CFLAGS = -ffreestanding -mno-red-zone -m64 -g -I ./

KERNEL_OFFSET = 0x8000
KERNEL_ENTRY = _kmain

all: debug

debug: os-image kernel.elf
	dd if=os-image.bin of=os-image.img bs=512 conv=notrunc

os-image: bootloader.bin kernel.bin
	cat bootloader.bin kernel.bin > $@.bin

bootloader.bin: bootloader/bootsector/bootloader.asm
	nasm -f bin $^ -o $@

kernel.bin: kernel_loader.o ${OBJ}
	$(LD) -T link.ld

# Used for debugging purposes
kernel.elf: kernel_loader.o ${OBJ}
	$(LD) -T link.ld -o kernel.elf

kernel_loader.o: bootloader/sector2/kernel_loader.asm
	nasm $< -f elf64 -o $@

kernel/Interrupts.o: kernel/Interrupts.c
	${CC} ${CFLAGS} -mgeneral-regs-only -c $< -o $@

%.o: kernel/%.c drivers/%.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf *.bin *.img *.elf *.o kernel/*.o drivers/*.o
