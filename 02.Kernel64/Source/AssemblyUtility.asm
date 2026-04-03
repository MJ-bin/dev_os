[BITS 64]           ; 이하의 코드는 64비트 코드로 설정

SECTION .text       ; text 섹션(세그먼트)을 정의

; C 언어에서 호출할 수 있도록 이름을 노출
global kInPortByte, kOutPortByte

; 포트로부터 1바이트를 읽음
; rdi(PARAM): 포트 번호
kInPortByte:
    push rdx        
    mov rdx, rdi    ; RDX 레지스터에 파라미터 1(포트 번호)를 저장
    mov rax, 0      
    in al, dx       ; DX 레지스터에 저장된 포트 어드레스에서 한 바이트를 읽어 AL 레지스터에 저장, AL 레지스터는 함수의 반환 값으로 사용
    pop rdx         
    ret             
    
; 포트에 1바이트를 씀
; rdi(PARAM1): 포트 번호, rsi(PARAM2): 데이터
kOutPortByte:
    push rdx        
    push rax        
    mov rdx, rdi    ; RDX 레지스터에 파라미터 1(포트 번호)를 저장
    mov rax, rsi    ; RAX 레지스터에 파라미터 2(데이터)를 저장
    out dx, al      ; DX 레지스터에 저장된 포트 어드레스에 AL 레지스터에 저장된 한 바이트를 씀
    pop rax         
    pop rdx
    ret             
    
; 포트로부터 2바이트를 읽음
; rdi(PARAM): 포트 번호
kInPortWord:
    push rdx
    mov rdx, rdi    ; RDX 레지스터에 파라미터 1(포트 번호)를 저장
    mov rax, 0      
    in ax, dx       ; DX 레지스터에 저장된 포트 어드레스에서 두 바이트를 읽어 AX 레지스터에 저장, AX 레지스터는 함수의 반환 값으로 사용
    pop rdx         
    ret             
    
; 포트에 2바이트를 씀
;  rdi(PARAM1): 포트 번호, rsi(PARAM2): 데이터
kOutPortWord:
    push rdx        
    push rax
    mov rdx, rdi    ; RDX 레지스터에 파라미터 1(포트 번호)를 저장
    mov rax, rsi    ; RAX 레지스터에 파라미터 2(데이터)를 저장    
    out dx, ax      ; DX 레지스터에 저장된 포트 어드레스에 AX 레지스터에 저장된 두 바이트를 씀
    pop rax         
    pop rdx
    ret             
