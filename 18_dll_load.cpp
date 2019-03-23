
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
	HMODULE hModule;
	TCHAR pszPathName[100];
	DWORD dResult;
	//pointer to function prototype same as GetThreadId() from Kernel32.dll
	typedef DWORD(__stdcall *PFN)(HANDLE);
	PFN pfn;

	hModule = GetModuleHandle(TEXT("Kernel32.dll"));
	if (hModule == NULL){
		_tprintf(TEXT("Error GetModuleHandle %u\n"), GetLastError());
		hModule = LoadLibrary(TEXT("Kernel32.dll"));
		if (hModule == NULL){
			_tprintf(TEXT("Error LoadLibrary %u\n"), GetLastError());
		}
		FreeLibrary(hModule);
	}
	else{
		dResult = GetModuleFileName(hModule, pszPathName, 100);
		_tprintf(TEXT("dResult %u, pszPathName %s\n"), dResult, pszPathName);
		//retrieve GetThreadId() procedure address
		pfn = (PFN)GetProcAddress(hModule, "GetThreadId");
		if (pfn != NULL){
			_tprintf(TEXT("ThreadId %u\n"), pfn(GetCurrentThread()));
		}
	}

	getchar();

	return 0;
}




/*
Output :

dResult 32, pszPathName C:\WINDOWS\System32\KERNEL32.DLL
ThreadId 4928


*/
