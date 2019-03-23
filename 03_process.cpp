
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>


int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	TCHAR szCmdLine[] = TEXT("notepad");
	BOOL result;
	DWORD dwExitCodeThread, dwExitCodeProcess;
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	HMODULE hModule = GetModuleHandle(NULL);
	if (hModule){
		_tprintf(TEXT("base address %x\n"), hModule);
	}

	si.dwX = 400;
	si.dwY = 200;
	si.dwXSize = 100;
	si.dwYSize = 50;
	si.lpTitle = TEXT("hello");
	si.dwFlags = STARTF_USESIZE | STARTF_USEPOSITION;

	result = CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (result){
		_tprintf(TEXT("process id %d\n"), pi.dwProcessId);
		_tprintf(TEXT("thread id %d\n"), pi.dwThreadId);
		_tprintf(TEXT("hprocess %d\n"), pi.hProcess);
		_tprintf(TEXT("hthread %d\n"), pi.hThread);
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hThread, INFINITE);
		GetExitCodeThread(pi.hThread, &dwExitCodeThread);
		GetExitCodeProcess(pi.hProcess, &dwExitCodeProcess);
		_tprintf(TEXT("thread exit %d, process exit %d\n"), dwExitCodeThread, dwExitCodeProcess);
		CloseHandle(pi.hProcess);
	}

	sei.lpVerb = TEXT("runas");
	sei.lpFile = TEXT("cmd.exe");
	sei.nShow = SW_SHOWNORMAL;

	if (!ShellExecuteEx(&sei)){
		DWORD err = GetLastError();
		_tprintf(TEXT("shell exec err %d\n"), err);
	}


	getchar();

	return 0;
}


/*
Output :

base address 8d0000
process id 13684
thread id 4896
hprocess 508
hthread 348
thread exit -858993460, process exit 259


*/
