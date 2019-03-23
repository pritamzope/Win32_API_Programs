
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

HANDLE hMutex;

int g_X = 0;

DWORD WINAPI ThreadFunc(PVOID pvParam) {
	DWORD dWaitResult;
	DWORD dCount = 0;
	while (dCount < 2){
		dWaitResult = WaitForSingleObject(hMutex, INFINITE);
		if (dWaitResult == WAIT_OBJECT_0){
			g_X++;
			_tprintf(TEXT("Executing threadid %u, g_X = %d\n"), GetCurrentThreadId(), g_X);
			dCount++;
			Sleep(5);
			ReleaseMutex(hMutex);
		}
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

	hMutex = CreateMutex(NULL, FALSE, TEXT("MyMutex"));
	if (hMutex == NULL)
	{
		_tprintf(TEXT("CreateMutex error %d\n"), GetLastError());
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

	CloseHandle(hMutex);

	getchar();

	return 0;
}



/*
Output :

Executing threadid 13680, g_X = 1
Executing threadid 9476, g_X = 2
Executing threadid 5584, g_X = 3
Executing threadid 1620, g_X = 4
Executing threadid 8284, g_X = 5
Executing threadid 9864, g_X = 6
Executing threadid 6528, g_X = 7
Executing threadid 16060, g_X = 8
Executing threadid 12204, g_X = 9
Executing threadid 15656, g_X = 10
Executing threadid 13680, g_X = 11
Executing threadid 9476, g_X = 12
g_X = 12


*/
