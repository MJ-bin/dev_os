#include "Types.h"
#include "Keyboard.h"


void kPrintString( int iX, int iY, const char* pcString );

// 아래 함수는 C 언어 커널의 시작 부분임
void Main( void )
{
    char vcTemp[2] = {0,};
    BYTE bFlags;
    BYTE bTemp;
    int i = 0;

    kPrintString( 0, 12, "Switch To IA-32e Mode Success" );
    kPrintString( 0, 13, "IA-32e C Language Kernel Start...........[Pass]" );
    kPrintString( 0, 14, "Keyboard Activate And LED Control Test...[    ]" );

    if ( kActivateKeyboard() == TRUE )
    {
        kPrintString( 42, 14, "Pass" );
        kChangeKeyboardLED( FALSE, FALSE, FALSE );
    }
    else
    {
        kPrintString( 42, 14, "Fail" );
        while( 1 ) ;
    }

    while( 1 )
    {   
        // 출력버퍼(port 0x60)에 값이 있을때,
        if( kIsOutputBufferFull() == TRUE )
        {
            // 출력버퍼(port 0x60)에서 스캔 코드를 읽어서 저장
            bTemp = kGetKeyboardScanCode();
            
            // 스캔 코드를 ASCII 코드로 변환하고, 눌림/떼짐 여부도 반환
            if (kConvertScanCodeToASCIICode( bTemp, &bTemp, &bFlags ) == TRUE )
            {
                // 스캔 코드가 ASCII 코드로 변환되었으면 문자로 출력
                if ( bFlags & KEY_FLAGS_DOWN )
                {
                    vcTemp[ 0 ] = bTemp;
                    kPrintString( i++ % 80, 15, vcTemp );
                }
            }
        }
        else
        {
            continue;
        }
    }
}

// 문자열을 X, Y 위치에 출력
void kPrintString( int iX, int iY, const char* pcString )
{
    CHARACTER* pstScreen = ( CHARACTER* ) 0xB8000;
    int i;

    // X, Y 좌표를 이용해서 문자열을 출력할 어드레스를 계산
    pstScreen += ( iY * 80 ) + iX;

    // NULL이 나올 때까지 문자열 출력
    for( i = 0; pcString[ i ] != 0; i++ )
    {
        pstScreen[ i ].bCharacter = pcString[ i ];
        pstScreen[ i ].bAttribute = 0x05; // 색상도 추가해봄
    }
}
