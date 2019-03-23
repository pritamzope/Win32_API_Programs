
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<Psapi.h>
#include<stdlib.h>
#include<string.h>

//for static TLS when thread is created
__declspec(thread) DWORD g_dwStartTime = 0;

//for dynamic TLS
DWORD g_dwTlsIndex;

void func()
{
	DWORD dwVal = *(LPDWORD)TlsGetValue(g_dwTlsIndex);
	_tprintf(TEXT("dwVal : %u\n"), dwVal);
}

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	DWORD dwVal = 12345;

	g_dwTlsIndex = TlsAlloc();
	if (g_dwTlsIndex != TLS_OUT_OF_INDEXES){
		TlsSetValue(g_dwTlsIndex, (LPVOID)&dwVal);

		func();

		TlsFree(g_dwTlsIndex);
	}


	getchar();

	return 0;
}




/*
Output :

dwVal : 12345


*/
