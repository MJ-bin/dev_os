#include "Types.h"
#include "Page.h"

void kPrintString( int iX, int iY, const char* pcString );
BOOL kIsMemoryEnough(void); // 1~64MB 까지의 메모리가 존재하는지 확인하는 함수(1MB 단위로 Write-Read 테스트)
BOOL kInitializeKernel64Area(void); // 1~6MB 사이의 메모리를 0으로 초기화(for IA-32e 커널)
// bool 타입은 C99 freestanding에서 지원하지 않음, Types.h 에서 정의한 BOOL 타입을 사용


// Main함수: 엔트리포인트 함수(0x10200)
void Main( void )
{
    kPrintString( 0, 4, "C Language Kernel Started!" );

    // 1MB ~ 64MB 사이의 메모리가 존재하는지 확인하는 함수
    kPrintString( 0, 5, "Minimum Memory size(64MB) checking.....[    ]" );
    if (kIsMemoryEnough() == FALSE)
    {
        kPrintString( 40, 5, "Fail" );
        kPrintString( 0, 6, "[ERROR] NOT ENOUGH MEMORY: OS Requires Over 64MB Memory" );
        while( 1 ) ;
    }
    else 
    {
        kPrintString( 40, 5, "Pass" );
    }

    // 1MB ~ 6MB 사이의 메모리를 초기화(for IA-32e 커널)
    kPrintString( 0, 6, "Initializing Kernel64Area(1MB~6MB).....[    ]" );
    if (kInitializeKernel64Area() == FALSE)
    {
        kPrintString( 40, 6, "Fail" );
        kPrintString( 0, 7, "[ERROR] FAILED TO INITIALIZE KERNEL64AREA: cannot initialize memory" );
        while( 1 ) ;
    }
    else 
    {
        kPrintString( 40, 6, "Pass" );
    }

    // IA-32e 모드 커널을 위한 페이지 테이블 생성
    kPrintString( 0, 7, "Initializing Page Tables...............[    ]" );
    kInitializePageTables();
    kPrintString( 40, 7, "Pass" );

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


// 1MB ~ 64MB 사이의 메모리가 충분한지 확인하는 함수
BOOL kIsMemoryEnough(void)
{
    DWORD* pdwCurrentAddress;
    pdwCurrentAddress = ( DWORD* ) 0x100000;

    while ((DWORD)pdwCurrentAddress < 0x4000000) 
    {
        // 1MB ~ 64MB 사이의 메모리의 처음 값(4바이트)을 백업
        DWORD* qwBackup;
        *qwBackup = *pdwCurrentAddress;

        // 테스트 값 write/read
        *pdwCurrentAddress = 0x6d6a6f73; // "mjos" to Hex: 6d 6a 6f 73
        if (*pdwCurrentAddress != 0x6d6a6f73) 
        {
            return FALSE;
        }

        // 백업한 값을 복원
        *pdwCurrentAddress = *qwBackup;
        pdwCurrentAddress += (0x100000/4); // 1MB씩 증가
    }
    return TRUE;
}

// 1MB ~ 6MB 사이의 메모리를 초기화(for IA-32e 커널) 함수
BOOL kInitializeKernel64Area(void)
{
    // 초기화 시작 주소
    DWORD* pdwCurrentAddress;
    pdwCurrentAddress = ( DWORD* ) 0x100000;

    while ((DWORD)pdwCurrentAddress < 0x600000) 
    {
        *pdwCurrentAddress = 0x00;

        if (*pdwCurrentAddress != 0x00) 
        {
            return FALSE;
        }
        pdwCurrentAddress++;
    }
    return TRUE;
}