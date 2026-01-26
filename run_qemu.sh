#!/bin/bash
# 스크립트가 있는 디렉토리로 이동 (Windows의 cd /d "%~dp0" 와 동일)
cd "$(dirname "$0")"

# -fda: 플로피 디스크 이미지 지정 (Windows의 -fda 와 동일)
# -curses: 텍스트 모드로 실행 (SSH 환경이라 그래픽 창을 띄울 수 없어 추가된 옵션)
#          종료하려면 Alt+2 누른 후 quit 입력, 또는 단순히 새로운 터미널에서 pkill qemu
qemu-system-x86_64 -m 64 -fda ./Disk.img -rtc base=localtime -M pc -curses "$@"