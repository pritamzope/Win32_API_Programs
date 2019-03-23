
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

CRITICAL_SECTION g_CS;

// Define a global variable.
volatile long g_x = 0;

DWORD WINAPI ThreadFunc1(PVOID pvParam) {
	InterlockedAdd(&g_x, 1);
	return(0);
}

DWORD WINAPI ThreadFunc2(PVOID pvParam) {
	EnterCriticalSection(&g_CS);
	g_x++;
	LeaveCriticalSection(&g_CS);
	Sleep(100);
	return(0);
}

DWORD WINAPI ThreadFunc3(PVOID pvParam) {
	EnterCriticalSection(&g_CS);
	g_x++;
	LeaveCriticalSection(&g_CS);
	Sleep(100);
	return(0);
}

DWORD WINAPI ThreadFunc4(PVOID pvParam) {
	InterlockedAdd(&g_x, 1);
	return(0);
}


int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	DWORD dThreadID1, dThreadID2;
	HANDLE hThread1, hThread2;
	HANDLE handles[2];
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION slpi;
	DWORD dReturnLength;


	GetLogicalProcessorInformation(&slpi, &dReturnLength);
	_tprintf(TEXT("Cache level %u, line size %u\n"), slpi.Cache.Level, slpi.Cache.LineSize);

	InitializeCriticalSection(&g_CS);


	hThread1 = CreateThread(NULL, 10, ThreadFunc3, NULL, 0, &dThreadID1);
	hThread2 = CreateThread(NULL, 10, ThreadFunc4, NULL, 0, &dThreadID2);

	handles[0] = hThread1;
	handles[1] = hThread2;

	WaitForMultipleObjects(2, handles, TRUE, INFINITE);

	CloseHandle(hThread2);
	CloseHandle(hThread1);

	DeleteCriticalSection(&g_CS);

	_tprintf(TEXT("g_x = %d\n"), g_x);


	getchar();

	return 0;
}



/*
Output :

Cache level 204, line size 52428
g_x = 2


*/
