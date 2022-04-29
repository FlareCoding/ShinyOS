albert_os:
	nasm -f bin bootsector/bootsector.asm -o bootsector.bin

clean:
	rm -rf bootsector.bin