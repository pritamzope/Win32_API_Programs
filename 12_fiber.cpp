
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

void CALLBACK FiberFunc(PVOID pvParam)
{
	PVOID pData = GetFiberData();
	DWORD dY = *((DWORD*)pData);
	_tprintf(TEXT("Fiber = %x, Y = %u\n"), GetCurrentFiber(), dY);
}

DWORD WINAPI ThreadFunc(PVOID pvParam)
{
	LPVOID pMainFiber;
	LPVOID pFiber;
	DWORD dY = 100;

	_tprintf(TEXT("ThreadFunc\n"));

	pMainFiber = ConvertThreadToFiber(NULL);

	pFiber = CreateFiber(255, FiberFunc, (PVOID)&dY);
	if (pFiber == NULL){
		_tprintf(TEXT("Error CreateFiber %u\n"), GetLastError());
	}
	else{
		_tprintf(TEXT("StitchToFiber = %x\n"), pFiber);
		SwitchToFiber(pFiber);

		DeleteFiber(pFiber);
	}

	DeleteFiber(pMainFiber);

	return(0);
}


int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	DWORD dThreadID;
	HANDLE hThread;

	hThread = CreateThread(NULL, 10, ThreadFunc, NULL, 0, &dThreadID);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);

	getchar();

	return 0;
}




/*
Output :

ThreadFunc
StitchToFiber = e2ca78
Fiber = e2ca78, Y = 100


*/
