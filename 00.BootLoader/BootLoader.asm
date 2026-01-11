; Directive란, nasm 어셈블러에게 지시하는 코드를 의미한다.
; instruction란, nasm 어셈블러가 cpu 기계어로 만들어야하는 코드를 의미한다.

[ORG 0x00]          ; {Directive} 코드의 시작 어드레스를 0x00으로 설정
[BITS 16]           ; {Directive} 이하의 코드는 16비트 코드로 설정

SECTION .text     ; {Directive} text 섹션(세그먼트)을 정의

    jmp 0x07C0:START ; BIOS가 부트로더를 로드할때, 물리주소 0x07C0:0x0000에 로드한다.
    ; jmp X:Y 는 CS 레지스터에 X를, IP 레지스터에 Y를 설정한다. 이때 Y는 파일의 시작점 [ORG 0x00]로부터 떨어진 상대주소값.

START:
    mov ax, 0x07C0    ; AX 레지스터에 부트로더 시작주소 0x07C0 복사
    mov ds, ax        ; DS 세그먼트 레지스터에 AX 레지스터의 값(0x07C0)을 복사

    ; 리얼모드에서 즉시 세그먼트 레지스터(ds)에 값을 mov 할수 없다.
    mov ax, 0xB800    ; AX 레지스터에 0xB800 복사
    mov es, ax        ; ES 세그먼트 레지스터에 AX 레지스터의 값(0xB800)을 복사

    ; [ds로부터 offset] 에 값을 대입한다. 이때 []는 유효주소를 품는다.
    mov byte [es: 0x00], 'M'    ; ES:0x0000(= 0xB800:0x0000)에 'M' 기록
    mov byte [es: 0x01], 0x4A   ; ES:0x0001에 속성(0x4A) 기록

    jmp $         ; {Instruction} 현재 위치에서 무한 루프 수행(== while(1))

    times 510 - ($ - $$) db 0x00  ; {Directive}
                  ; $             : 현재 위치
                  ; $$            : 현재 섹션(.text) 시작 위치
                  ; ($ - $$)      : 섹션 시작부터 현재까지의 오프셋(섹션 내부의 오프셋)
                  ; 510 - ($ - $$): 현재위치부터 부트로더 마지막 시그니처를 제외한 모든 공간의 바이트크기
                  ; db            : 현재 어드레스에 1바이트 크기의 데이터를 대입
                  ; db 0x00       : 현재 어드레스에 1바이트 크기의 0x00을 대입
                  ; times N db 0x00: db 0x00 을 N 번 반복해서 출력하라. 이때 어셈블러는 출력할때마다 내부적으로 현재위치를 1바이트씩 증가시킨다.

    db 0x55       ;{Directive} 부트 섹터 시그니처 (마지막 2바이트)
    db 0xAA       ;{Directive} 부트 섹터 시그니처 (마지막 2바이트)