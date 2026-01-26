all: BootLoader Kernel32 ImageMaker Disk.img

# 변수 정의
BootLoader := 00.BootLoader/BootLoader.bin
Kernel32   := 01.Kernel32/Kernel32.bin
ImageMaker := 04.Utility/00.ImageMaker/ImageMaker.exe 


BootLoader:
	@echo
	@echo ============== Build Boot Loader ==============
	@echo

	make -C 00.BootLoader 
	# -C 옵션은 현재 디렉토리를 00.BootLoader로 변경한뒤 make를 실행한다.

	@echo
	@echo ============== Build Complete ==============

Kernel32:
	@echo
	@echo ============== Build Kernel32 ==============
	@echo

	make -C 01.Kernel32
	# -C 옵션은 현재 디렉토리를 00.BootLoader로 변경한뒤 make를 실행한다.
	
	@echo
	@echo ============== Build Complete ==============
	@echo

ImageMaker:
	@echo
	@echo ============== Build ImageMaker ==============
	@echo

	make -C 04.Utility/00.ImageMaker
	@echo
	@echo ============== Build Complete ==============
	@echo

Disk.img: 00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin ImageMaker
	@echo
	@echo =========== Disk Image Build Start ===========
	@echo

	# $^ 키워드는 Dependency의 모든 파일들을 의미한다.
	# cat $^ > Disk.img # 이 방식은 ImageMaker(04.Utility/00.ImageMaker/ImageMaker.c)를 도입하여 대체됨
	$(ImageMaker) $(BootLoader) $(Kernel32)

	@echo
	@echo ============= All Build Complete =============
	@echo

# make clean 시 00.BootLoader 디렉토리와 01.Kernel32 디렉토리의 clean을 실행하고 Disk.img를 삭제한다.
clean:
	make -C 00.BootLoader clean
	make -C 01.Kernel32 clean
	make -C 04.Utility/00.ImageMaker clean
	rm -f Disk.img