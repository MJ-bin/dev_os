#include "Types.h"

void kPrintString( int iX, int iY, const char* pcString );

// Main함수: 엔트리포인트 함수(0x10200)
void Main( void )
{
    kPrintString( 0, 3, "C Language Kernel Started!" );

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
        pstScreen[ i ].bAttribute = 0x0F; // 색상도 추가해봄
    }
}
