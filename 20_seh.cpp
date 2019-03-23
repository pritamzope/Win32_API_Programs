
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<windowsx.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

BOOL FileReadWithTryFinally()
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	PVOID pvBuf = NULL;
	DWORD dwNumBytesRead;
	BOOL bOk, bSuccess = FALSE;

	__try{
		hFile = CreateFile(TEXT("E:\\file.dat"), GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if (hFile != INVALID_HANDLE_VALUE) {

			pvBuf = VirtualAlloc(NULL, 1024, MEM_COMMIT, PAGE_READWRITE);
			if (pvBuf != NULL) {

				bOk = ReadFile(hFile, pvBuf, 1024, &dwNumBytesRead, NULL);
				if (bOk && (dwNumBytesRead != 0)) {
					LPBYTE lpbBuf = (LPBYTE)pvBuf;
					while (*lpbBuf != '\0'){
						_tprintf(L"%c", *lpbBuf);
						lpbBuf++;
					}
					_tprintf(L"\n");
					bSuccess = TRUE;
				}
				else{
					__leave;
				}
			}
		}
		else{
			_tprintf(TEXT("Error CreateFile %u\n"), GetLastError());
			__leave;
		}
	}
	__finally{
		if (pvBuf != NULL)
			VirtualFree(pvBuf, 1024, MEM_RELEASE | MEM_DECOMMIT);
		if (hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
	}
	return(bSuccess);
}

void PrintBinary(DWORD dwNum)
{
	while (dwNum > 0){
		if (dwNum & 1){
			_tprintf(TEXT("1"));
		}
		else{
			_tprintf(TEXT("0"));
		}
		dwNum >>= 1;
	}
	_tprintf(TEXT("\n"));
}

void ShowContext(PCONTEXT pContext)
{
	_tprintf(TEXT("  EFLAGS : "));
	PrintBinary(pContext->EFlags);
	_tprintf(TEXT("  Dr0 : %x\n"), pContext->Dr0);
	_tprintf(TEXT("  Dr1 : %x\n"), pContext->Dr1);
	_tprintf(TEXT("  Dr2 : %x\n"), pContext->Dr2);
	_tprintf(TEXT("  Dr3 : %x\n"), pContext->Dr3);
	_tprintf(TEXT("  Dr4 : %x\n"), pContext->Dr6);
	_tprintf(TEXT("  Dr7 : %x\n"), pContext->Dr7);
	_tprintf(TEXT("  Eax : %x\n"), pContext->Eax);
	_tprintf(TEXT("  Ebx : %x\n"), pContext->Ebx);
	_tprintf(TEXT("  Ecx : %x\n"), pContext->Ecx);
	_tprintf(TEXT("  Edx : %x\n"), pContext->Edx);
	_tprintf(TEXT("  Esi : %x\n"), pContext->Esi);
	_tprintf(TEXT("  Edi : %x\n"), pContext->Edi);
	_tprintf(TEXT("  Ebp : %x\n"), pContext->Ebp);
	_tprintf(TEXT("  Eip : %x\n"), pContext->Eip);
	_tprintf(TEXT("  Esp : %x\n"), pContext->Esp);
	_tprintf(TEXT("  SegCs : %x\n"), pContext->SegCs);
	_tprintf(TEXT("  SegDs : %x\n"), pContext->SegDs);
	_tprintf(TEXT("  SegEs : %x\n"), pContext->SegEs);
	_tprintf(TEXT("  SegFs : %x\n"), pContext->SegFs);
	_tprintf(TEXT("  SegGs : %x\n"), pContext->SegGs);
	_tprintf(TEXT("  SegSs : %x\n"), pContext->SegSs);
}

void PrintExceptionInformation(PEXCEPTION_POINTERS pexPtr)
{
	PCONTEXT pContext = pexPtr->ContextRecord;
	PEXCEPTION_RECORD pexRecord = pexPtr->ExceptionRecord;
	_tprintf(L"\nException Information : \n");
	_tprintf(L"Context :\n");
	ShowContext(pContext);
	_tprintf(L"Exception Record :\n");
	_tprintf(TEXT("  ExceptionAddress : %x\n"), pexRecord->ExceptionAddress);
	_tprintf(TEXT("  ExceptionCode : %u\n"), pexRecord->ExceptionCode);
	_tprintf(TEXT("  ExceptionFlags : %u\n"), pexRecord->ExceptionFlags);
	_tprintf(TEXT("  NumberParameters : %u\n"), pexRecord->NumberParameters);

}


BOOL TryExcept()
{
	DWORD dWX = 10, dwY = 0, dwResult;
	PEXCEPTION_POINTERS pexPtr = NULL;

	__try{
		dwResult = dWX / dwY;
	}
	__except (EXCEPTION_EXECUTE_HANDLER){
		//pexPtr = GetExceptionInformation();
		_tprintf(TEXT("Divide by zero\n"));
	}
	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	FileReadWithTryFinally();
	TryExcept();
	getchar();

	return 0;
}




/*
Output :

Hello
Divide by zero


*/
