CC  = /usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
CXX = /usr/local/x86_64elfgcc/bin/x86_64-elf-g++
LD  = /usr/local/x86_64elfgcc/bin/x86_64-elf-ld

C_SOURCES   = $(wildcard kernel/*.c drivers/*.c)
CPP_SOURCES = $(wildcard kernel/*.cpp drivers/*.cpp kernel/paging/*.cpp)
HEADERS     = $(wildcard kernel/*.h drivers/*.h)

OBJ_C = ${C_SOURCES:.c=.o}
OBJ_CPP = ${CPP_SOURCES:.cpp=.o}
OBJ = ${OBJ_C} ${OBJ_CPP}

CFLAGS = -ffreestanding -mno-red-zone -m64 -g -I ./
CXXFLAGS = -ffreestanding -mno-red-zone -m64 -g -I ./ -std=c++17 -Wall
LXXFLAGS = -std=c++17

KERNEL_OFFSET = 0x8000
KERNEL_ENTRY = _kmain

all: debug

debug: os-image kernel.elf
	dd if=os-image.bin of=os-image.img bs=512 conv=notrunc

os-image: bootloader.bin kernel.bin
	cat bootloader.bin kernel.bin > $@.bin

bootloader.bin: bootloader/bootsector/bootloader.asm
	nasm -f bin $^ -o $@

kernel.bin: kernel_loader.o kernel/Interrupts.o ${OBJ}
	$(LD) -T link.ld

# Used for debugging purposes
kernel.elf: kernel_loader.o kernel/Interrupts.o ${OBJ}
	$(LD) -T debug-elf-link.ld

kernel_loader.o: bootloader/sector2/kernel_loader.asm
	nasm $< -f elf64 -o $@

kernel/Idt.o: kernel/Idt.cpp
	${CXX} ${CXXFLAGS} ${LXXFLAGS} -mgeneral-regs-only -c $< -o $@

%.o: kernel/%.c drivers/%.c ${HEADERS}
	${C} ${CFLAGS} -c $< -o $@

%.o: kernel/%.cpp drivers/%.cpp kernel/paging/%.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} ${LXXFLAGS} -c $< -o $@

kernel/Interrupts.o: kernel/Interrupts.asm
	nasm $< -f elf64 -o $@

clean:
	rm -rf *.bin *.img *.elf *.o kernel/*.o kernel/*/*.o drivers/*.o drivers/*/*.o
