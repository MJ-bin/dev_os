# ImageMaker.exe는 리눅스에서도 동작한다.
all: ImageMaker.exe
	@echo "ImageMaker.exe is created."

ImageMaker.exe: ImageMaker.c
	@echo "Compiling ImageMaker.c..."
	gcc -o $@ $<

clean:
	rm -f ImageMaker.exe