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

    mov si, 0x00      ; SI 레지스터(문자열 원본 인덱스 레지스터)에 0x00 복사


.SCREENCLEARLOOP:
    mov byte [es: si], 0x00 
    mov byte [es: si + 1], 0x0A
    add si, 2
    cmp si, 80 * 25 * 2
    jl .SCREENCLEARLOOP

    mov si, 0x00
    mov di, 0x00


.MESSAGELOOP:
    mov cl, byte [ si + MESSAGE1 ] ; MESSAGE1의 어드레스에서 SI 레지스터 값만큼
                                   ; 더한 위치의 문자를 CL 레지스터에 복사
                                   ; CL 레지스터는 CX 레지스터의 하위 1바이트를 의미
                                   ; 문자열은 1바이트면 충분하므로 CX 레지스터의 하위 1바이트만 사용

    cmp cl, 0             ; 복사된 문자와 0을 비교
    je .MESSAGEEND        ; 복사한 문자의 값이 0이면 문자열이 종료되었음을
                         ; 의미하므로 .MESSAGEEND로 이동하여 문자 출력 종료

    mov byte [ es: di ], cl ; 0이 아니라면 비디오 메모리 어드레스 0xB800:di에 문자를 출력

    add si, 1             ; SI 레지스터에 1을 더하여 다음 문자열로 이동
    add di, 2             ; DI 레지스터에 2를 더하여 비디오 메모리(0xB800:di)의 다음 문자 위치로 이동
                          ; 비디오 메모리는 (문자, 속성)의 쌍으로 구성되므로 문자만 출력하려면, 2를 더해야 함

    jmp .MESSAGELOOP      ; 메시지 출력 루프로 이동하여 다음 문자를 출력


.MESSAGEEND:
    jmp $         ; {Instruction} 현재 위치에서 무한 루프 수행(== while(1))


CODEDESCRIPTOR:
    dw 0xFFFF        ; Limit [15:0]
    dw 0x0000        ; Base [15:0]
    db 0x00          ; Base [23:16]
    db 0x9A          ; P=1, DPL=0, Code Segment, Execute/Read
    db 0xCF          ; G=1, D=1, L=0, Limit[19:16]
    db 0x00          ; Base [31:24]


DATADESCRIPTOR:
    dw 0xFFFF        ; Limit [15:0]
    dw 0x0000        ; Base [15:0]
    db 0x00          ; Base [23:16]
    db 0x92          ; P=1, DPL=0, Data Segment, Read/Write
    db 0xCF          ; G=1, D=1, L=0, Limit[19:16]
    db 0x00          ; Base [31:24]


MESSAGE1:
    db 'mjbin OS Boot Loader Start~!!', 0 ; 출력할 메시지 정의

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