all: BootLoader Disk.img

BootLoader:
	@echo
	@echo ============== Build Boot Loader ==============
	@echo

	make -C 00.BootLoader 
	# -C 옵션은 현재 디렉토리를 00.BootLoader로 변경한뒤 make를 실행한다.

	@echo
	@echo ============== Build Complete ==============

Disk.img: 00.BootLoader/BootLoader.bin
	@echo
	@echo =========== Disk Image Build Start ===========
	@echo

	cp 00.BootLoader/BootLoader.bin Disk.img

	@echo
	@echo ============= All Build Complete =============
	@echo

# make clean 시 00.BootLoader 디렉토리의 clean을 실행하고 Disk.img를 삭제한다.
clean:
	make -C 00.BootLoader clean
	rm -f Disk.img