#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

// 운영체제에 따라 헤더 파일이 다르므로 조건 컴파일로 처리
#ifdef __linux__
    #include <unistd.h>
    #define O_BINARY 0
#else
    #include <io.h>
#endif

#define BYTESOFSECTOR 512

/*
이 코드는 BootLoader.asm 에 정의된 os 이미지의 크기를 저장할 변수 TOTALSECTORCOUNT 를 설정합니다. 이 코드는 freestand 에서 동작하지 않습니다.
이 코드는 커널의 일부분이 아니라 C 라이브러리 환경에서 빌드하여 커널 빌드 도구로 사용됩니다.
*/

// 함수 선언
int AdjustInSectorSize(int iFd, int iSourceSize);
void WriteKernelInformation(int iTargetFd, int iKernelSectorCount);
int CopyFile(int iSourceFd, int iTargetFd);

// Main 함수
int main(int argc, char *argv[]) {
    int iSourceFd;
    int iTargetFd;
    int iBootLoaderSize;
    int iKernel32SectorCount;
    int iSourceSize;

    // 커맨드 라인 옵션 검사
    if (argc < 3) {
    fprintf(stderr, "[ERROR] ImageMaker.exe BootLoader.bin Kernel32.bin\n");
    exit(-1);
    }

    // Disk.img 파일을 생성
    if ((iTargetFd = open("Disk.img", O_RDWR | O_CREAT | O_TRUNC | O_BINARY,
                        S_IREAD | S_IWRITE)) == -1) {
    fprintf(stderr, "[ERROR] Disk.img open fail.\n");
    exit(-1);
    }

    // 부트 로더 파일을 열어서 모든 내용을 디스크 이미지 파일로 복사
    printf("[INFO] Copy boot loader to image file\n");
    if ((iSourceFd = open(argv[1], O_RDONLY | O_BINARY)) == -1) {
        fprintf(stderr, "[ERROR] %s open fail\n", argv[1]);
        exit(-1);
    }

    iSourceSize = CopyFile(iSourceFd, iTargetFd); // 파일 복사후 전체 사이즈(바이트) 반환
    close(iSourceFd);

    // 파일 크기를 섹터 크기인 512바이트로 맞추기 위해 나머지 부분을 0x00으로 채움
    iBootLoaderSize = AdjustInSectorSize(iTargetFd, iSourceSize); // 파일의 전체 사이즈 반환(섹터수)
    printf("[INFO] %s size = [%d] and sector count = [%d]\n", argv[1], iSourceSize, iBootLoaderSize);

    // 32비트 커널 파일을 열어서 모든 내용을 디스크 이미지 파일로 복사
    printf("[INFO] Copy protected mode kernel to image file\n");
    if ((iSourceFd = open(argv[2], O_RDONLY | O_BINARY)) == -1) {
        fprintf(stderr, "[ERROR] %s open fail\n", argv[2]);
        exit(-1);
    }

    iSourceSize = CopyFile(iSourceFd, iTargetFd); // 파일 복사후 전체 사이즈(바이트) 반환
    close(iSourceFd);

    // 파일 크기를 섹터 크기인 512바이트로 맞추기 위해 나머지 부분을 0x00으로 채움
    iKernel32SectorCount = AdjustInSectorSize(iTargetFd, iSourceSize); // 32비트 커널의 전체 사이즈 반환(섹터수)
    printf("[INFO] %s size = [%d] and sector count = [%d]\n", argv[2], iSourceSize, iKernel32SectorCount);

    // 디스크 이미지에 커널 정보를 갱신
    printf("[INFO] Start to write kernel information\n");

    // 부트섹터의 5번째 바이트(부트로더가 로드해야할 섹터수 필드)부터 커널에 대한 정보를 넣음
    WriteKernelInformation(iTargetFd, iKernel32SectorCount);
    printf("[INFO] Image file create complete\n");

    close(iTargetFd);
    return 0;
}

// 현재 위치부터 512바이트 배수 위치까지 맞추어 0x00으로 채움
int AdjustInSectorSize(int iFd, int iSourceSize) {
    /*
    기능: 파일의 크기를 512바이트 배수로 맞추기 위해 파일 끝에 0x00으로 채움
    반환: 파일의 전체 사이즈 반환(섹터수)
    */
    int i;
    int iAdjustSizeToSector;
    char cCh;
    int iSectorCount;

    iAdjustSizeToSector = iSourceSize % BYTESOFSECTOR; // 나머지
    cCh = 0x00;

    if (iAdjustSizeToSector != 0) {
        iAdjustSizeToSector = 512 - iAdjustSizeToSector; // 채워야할 패딩크기
        printf("[INFO] File size [%lu] and fill [%u] byte\n", (long unsigned int)iSourceSize, iAdjustSizeToSector);
        
        for (i = 0; i < iAdjustSizeToSector; i++) {
            write(iFd, &cCh, 1);
        }
    } else {
        printf("[INFO] File size is aligned 512 byte\n");
    }

    // 섹터 수를 되돌려줌
    iSectorCount = (iSourceSize + iAdjustSizeToSector) / BYTESOFSECTOR;
    return iSectorCount;
}

// 부트 로더에 커널에 대한 정보를 삽입
void WriteKernelInformation(int iTargetFd, int iKernelSectorCount) {
    /*
    기능: 부트 로더 5번 바이트(부트로더가 로드해야할 섹터수 필드)에 커널섹터수를 삽입
    반환: void
    */

    unsigned short usData;
    long lPosition;

    // 파일의 시작에서 5바이트 떨어진 위치가 커널의 총 섹터 수 정보를 나타냄
    lPosition = lseek(iTargetFd, (off_t)5, SEEK_SET); // 파일 시작점(SEEK_SET==0)으로부터 5바이트 떨어진 위치로 커서 이동
    if (lPosition == -1) {
        fprintf(stderr, "lseek fail. Return value = %ld, errno = %d, %d\n", lPosition, errno, SEEK_SET);
        exit(-1);
    }

    // 커널 섹터 수를 2바이트 크기로 저장
    usData = (unsigned short)iKernelSectorCount;
    write(iTargetFd, &usData, sizeof(usData));

    printf("[INFO] Total sector count except boot loader [%d]\n", iKernelSectorCount);
}

// 소스 파일(Source FD)의 내용을 목표 파일(Target FD)에 복사하고 그 크기를 되돌려줌
int CopyFile(int iSourceFd, int iTargetFd) {
    /*
    기능: iSourceFd 파일내용을 읽고 iTargetFd에 쓰는 동작
    반환: 파일의 전체 사이즈 반환(바이트)
    */
    int iSourceFileSize;
    int iRead;
    int iWrite;
    char vcBuffer[BYTESOFSECTOR];

    iSourceFileSize = 0;
    while (1) {
        iRead = read(iSourceFd, vcBuffer, sizeof(vcBuffer));
        iWrite = write(iTargetFd, vcBuffer, iRead);

        if (iRead != iWrite) {
            fprintf(stderr, "[ERROR] iRead != iWrite.. \n");
            exit(-1);
        }
        iSourceFileSize += iRead;

        if (iRead != sizeof(vcBuffer)) { // 512바이트가 읽히지 않았다면, 파일의 마지막 남은부분(512바이트 미만)이 읽혔다는 의미 -> 루프탈출
            break;
        }
    }
    return iSourceFileSize; // 파일의 전체 사이즈 반환(바이트)
}