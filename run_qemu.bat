@echo off
setlocal
cd /d "%~dp0"

set "QEMU=c:\Program Files\qemu\qemu-system-x86_64.exe"
set "IMG=c:/dev_os/Disk.img"

"%QEMU%" -m 64 -fda "%IMG%" -rtc base=localtime -machine pc

pause