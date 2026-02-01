#include "Types.h"

void kPrintString( int iX, int iY, const char* pcString );
BOOL kInitializeKernel64Area(void); // bool 타입은 C99 freestanding에서 지원하지 않음, Types.h 에서 정의한 BOOL 타입을 사용

// Main함수: 엔트리포인트 함수(0x10200)
void Main( void )
{
    kPrintString( 0, 4, "C Language Kernel Started!" );

    // 1MB ~ 6MB 사이의 메모리를 초기화(for IA-32e 커널)
    kInitializeKernel64Area();
    kPrintString( 0, 5, "Kernel64Area Initialized!" );

    while( 1 ) ;
}

// 문자열 출력 함수
void kPrintString( int iX, int iY, const char* pcString )
{   
    // 0xB8000 메모리 주소를 2바이트 크기의 구조체(CHARACTER)로 보겠다.
    CHARACTER* pstScreen = ( CHARACTER* ) 0xB8000;
    int i;

    pstScreen += ( iY * 80 ) + iX;
    for( i = 0; pcString[ i ] != 0; i++ )
    {
        pstScreen[ i ].bCharacter = pcString[ i ];
        pstScreen[ i ].bAttribute = 0xA0; // 색상도 추가해봄
    }
}

BOOL kInitializeKernel64Area(void)
{
    // 초기화 시작 주소
    DWORD* pdwCurrentAddress;
    pdwCurrentAddress = ( DWORD* ) 0x100000;

    while ((DWORD)pdwCurrentAddress < 0x600000) {
      *pdwCurrentAddress = 0x00;

      if (*pdwCurrentAddress != 0x00) {
        return FALSE;
      }
      pdwCurrentAddress++;
    }

    return TRUE;
}