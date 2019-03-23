
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<Psapi.h>
#include<stdlib.h>
#include<string.h>

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	HANDLE hFile;
	HANDLE hFileMapping;
	DWORD dwFileSize;
	LPVOID lpFileAddress;

	hFile = CreateFile(TEXT("E:\\file.dat"),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE){
		_tprintf(TEXT("Error CreateFile %u\n"), GetLastError());
	}

	dwFileSize = GetFileSize(hFile, NULL);

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE,
		0, dwFileSize + sizeof(WCHAR), NULL);

	lpFileAddress = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	if (lpFileAddress == NULL){
		_tprintf(TEXT("Error MapViewOfFile %u\n"), GetLastError());
	}
	else{
		LPBYTE pbArray = (LPBYTE)lpFileAddress;

		//write Hello to mapped file
		pbArray[0] = 'H';
		pbArray[1] = 'e';
		pbArray[2] = 'l';
		pbArray[3] = 'l';
		pbArray[4] = 'o';
		pbArray[5] = '\0';

		_tprintf(TEXT("Written to file\n"));

		UnmapViewOfFile(pbArray);
	}
	CloseHandle(hFileMapping);
	CloseHandle(hFile);

	getchar();

	return 0;
}




/*
Output :

Written to file


*/
