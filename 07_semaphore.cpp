
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

HANDLE hSemaphore;

int g_X = 0;

DWORD WINAPI ThreadFunc(PVOID pvParam) {
	DWORD dWaitResult;
	dWaitResult = WaitForSingleObject(hSemaphore, 0);
	if (dWaitResult == WAIT_OBJECT_0){
		g_X++;
		_tprintf(TEXT("Executing threadid %u, g_X = %d\n"), GetCurrentThreadId(), g_X);
		Sleep(5);
		ReleaseSemaphore(hSemaphore, 1, NULL);
	}

	return(0);
}


int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	DWORD dThreadID[10];
	HANDLE handles[10];
	DWORD dReturnLength;
	SECURITY_ATTRIBUTES sa;
	DWORD dWait;
	int i;

	hSemaphore = CreateSemaphore(NULL, 10, 10, TEXT("MySemaphore"));
	if (hSemaphore == NULL)
	{
		_tprintf(TEXT("CreateSemaphore error %d\n"), GetLastError());
		getchar();
		return 1;
	}

	for (i = 0; i < 10; i++){
		handles[i] = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &dThreadID[i]);
		if (handles[i] == NULL){
			_tprintf(TEXT("Error to create thread %d\n"), i);
		}
	}

	WaitForMultipleObjects(10, handles, FALSE, INFINITE);

	_tprintf(TEXT("g_X = %d\n"), g_X);

	for (i = 0; i < 10; i++)
		CloseHandle(handles[i]);

	CloseHandle(hSemaphore);

	getchar();

	return 0;
}



/*
Output :

Executing threadid 13248, g_X = 2
Executing threadid 6700, g_X = 3
Executing threadid 14624, g_X = 4
Executing threadid 11680, g_X = 9
Executing threadid 5284, g_X = 10
Executing threadid 14988, g_X = 6
Executing threadid 11544, g_X = 7
Executing threadid 5560, g_X = 8
Executing threadid 8532, g_X = 1
Executing threadid 11928, g_X = 5
g_X = 10


*/
