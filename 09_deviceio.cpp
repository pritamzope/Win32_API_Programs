
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>


int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	HANDLE hFile;
	HANDLE hCompletionPort;
	OFSTRUCT ofStruct;
	LARGE_INTEGER li;
	ULARGE_INTEGER ulCompSize;
	BYTE pb[10];
	DWORD dNumBytes;
	BOOL bRead;
	DWORD dwNumBytes;
	ULONG_PTR CompletionKey;
	OVERLAPPED* pOverlapped;
	int i;

	DWORD dSectorsPerCluster;
	DWORD dBytesPerSector;
	DWORD dNumberOfFreeClusters;
	DWORD dTotalNumberOfClusters;

	GetDiskFreeSpace(TEXT("E:"), &dSectorsPerCluster, &dBytesPerSector,
		&dNumberOfFreeClusters, &dTotalNumberOfClusters);

	_tprintf(TEXT("dSectorsPerCluster %u\n"), dSectorsPerCluster);
	_tprintf(TEXT("dBytesPerSector %u\n"), dBytesPerSector);
	_tprintf(TEXT("dNumberOfFreeClusters %u\n"), dNumberOfFreeClusters);
	_tprintf(TEXT("dTotalNumberOfClusters %u\n"), dTotalNumberOfClusters);

	hFile = CreateFile(TEXT("E:\\Softwares\\OS\\ISO\\W10X64_8in1.1709.ENU.OCT2017.iso"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING | CREATE_NEW,
		FILE_FLAG_NO_BUFFERING, NULL);

	if (hFile == INVALID_HANDLE_VALUE){
		_tprintf(TEXT("Error CreateFile %u\n"), GetLastError());
	}
	else{
		hCompletionPort = CreateIoCompletionPort(hFile, NULL, 0, 1);

		ulCompSize.LowPart = GetCompressedFileSize(TEXT("E:\\Softwares\\OS\\ISO\\W10X64_8in1.1709.ENU.OCT2017.iso"),
			&ulCompSize.HighPart);

		_tprintf(TEXT("CompressedSize High %u, Low %u\n"), ulCompSize.HighPart, ulCompSize.LowPart);

		if (GetFileSizeEx(hFile, &li)){
			_tprintf(TEXT("Size High %u, Low %u\n"), li.HighPart, li.LowPart);
		}

		bRead = ReadFile(hFile, pb, 10, &dNumBytes, NULL);
		_tprintf(TEXT("bRead %d, dNumBytes %u, "), bRead, dNumBytes);
		for (i = 0; i < 10; i++){
			_tprintf(TEXT("%d "), pb[i]);
		}
		_tprintf(TEXT("\n"));

		BOOL bOk = GetQueuedCompletionStatus(hCompletionPort,
			&dwNumBytes, &CompletionKey, &pOverlapped, 1000);
		_tprintf(TEXT("dwNumBytes %x, CompletionKey %x\n"), dwNumBytes, CompletionKey);


		CloseHandle(hCompletionPort);

	}

	CloseHandle(hFile);



	getchar();

	return 0;
}



/*
Output :

dSectorsPerCluster 8
dBytesPerSector 512
dNumberOfFreeClusters 46647765
dTotalNumberOfClusters 138534399
CompressedSize High 1, Low 523534336
Size High 1, Low 523534336
bRead 0, dNumBytes 0, 204 204 204 204 204 204 204 204 204 204
dwNumBytes cccccccc, CompletionKey cccccccc


*/
