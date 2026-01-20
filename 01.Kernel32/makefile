all: Kernel32.bin

Kernel32.bin: Source/EntryPoint.s
	# $< 키워드는 Dependency의 첫번째 파일로 대체된다. 즉, Source/EntryPoint.s
	nasm -o Kernel32.bin $<

clean:
	rm -f Kernel32.bin
