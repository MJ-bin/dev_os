all: BootLoader.bin

BootLoader.bin: BootLoader.asm
	# nasm 어셈블리어 컴파일러로 빌드한다.
	nasm -o BootLoader.bin BootLoader.asm

clean:
	rm -f BootLoader.bin