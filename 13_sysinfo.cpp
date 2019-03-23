
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

void ShowProcessors()
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION pBuffer = NULL;
	DWORD dwSize = 0;
	DWORD procCoreCount;
	BOOL bResult = GetLogicalProcessorInformation(pBuffer, &dwSize);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		_tprintf(TEXT("Impossible to get processor information\n"));
		return;
	}
	pBuffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(dwSize);
	bResult = GetLogicalProcessorInformation(pBuffer, &dwSize);
	if (!bResult) {
		free(pBuffer);
		_tprintf(TEXT("Impossible to get processor information\n"));
		return;
	}
	_tprintf(TEXT(""), pBuffer->Cache);
	procCoreCount = 0;
	DWORD lpiCount = dwSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
	for (DWORD current = 0; current < lpiCount; current++) {
		if (pBuffer[current].Relationship == RelationProcessorCore) {
			if (pBuffer[current].ProcessorCore.Flags == 1) {
				_tprintf(TEXT(" + one CPU core (HyperThreading)\n"));
			}
			else {
				_tprintf(TEXT(" + one CPU socket\n"));
			}
			procCoreCount++;
		}
	}
	_tprintf(TEXT(" -> %d active CPU(s)\n"), procCoreCount);
	free(pBuffer);
}

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	SYSTEM_INFO si;
	BOOL bIsWow64;

	GetSystemInfo(&si);

	_tprintf(TEXT("System Info : \n"));
	_tprintf(TEXT("  dwActiveProcessorMask : %u\n"), si.dwActiveProcessorMask);
	_tprintf(TEXT("  dwAllocationGranularity : %u\n"), si.dwAllocationGranularity);
	_tprintf(TEXT("  dwNumberOfProcessors : %u\n"), si.dwNumberOfProcessors);
	_tprintf(TEXT("  dwOemId : %u\n"), si.dwOemId);
	_tprintf(TEXT("  dwPageSize : %u\n"), si.dwPageSize);
	_tprintf(TEXT("  dwProcessorType : %u\n"), si.dwProcessorType);
	_tprintf(TEXT("  lpMaximumApplicationAddress : %x\n"), si.lpMaximumApplicationAddress);
	_tprintf(TEXT("  lpMinimumApplicationAddress : %x\n"), si.lpMinimumApplicationAddress);
	_tprintf(TEXT("  wProcessorArchitecture : %u\n"), si.wProcessorArchitecture);
	_tprintf(TEXT("  wProcessorLevel : %u\n"), si.wProcessorLevel);
	_tprintf(TEXT("  wProcessorRevision : %u\n"), si.wProcessorRevision);
	_tprintf(TEXT("  wReserved : %u\n"), si.wReserved);

	GetNativeSystemInfo(&si);

	_tprintf(TEXT("Native System Info : \n"));
	_tprintf(TEXT("  dwActiveProcessorMask : %u\n"), si.dwActiveProcessorMask);
	_tprintf(TEXT("  dwAllocationGranularity : %u\n"), si.dwAllocationGranularity);
	_tprintf(TEXT("  dwNumberOfProcessors : %u\n"), si.dwNumberOfProcessors);
	_tprintf(TEXT("  dwOemId : %u\n"), si.dwOemId);
	_tprintf(TEXT("  dwPageSize : %u\n"), si.dwPageSize);
	_tprintf(TEXT("  dwProcessorType : %u\n"), si.dwProcessorType);
	_tprintf(TEXT("  lpMaximumApplicationAddress : %x\n"), si.lpMaximumApplicationAddress);
	_tprintf(TEXT("  lpMinimumApplicationAddress : %x\n"), si.lpMinimumApplicationAddress);
	_tprintf(TEXT("  wProcessorArchitecture : %u\n"), si.wProcessorArchitecture);
	_tprintf(TEXT("  wProcessorLevel : %u\n"), si.wProcessorLevel);
	_tprintf(TEXT("  wProcessorRevision : %u\n"), si.wProcessorRevision);
	_tprintf(TEXT("  wReserved : %u\n"), si.wReserved);

	ShowProcessors();

	if (IsWow64Process(GetCurrentProcess(), &bIsWow64)){
		_tprintf(TEXT("IsWow64Process %d\n"), bIsWow64);
	}


	getchar();

	return 0;
}




/*
Output :

System Info :
dwActiveProcessorMask : 15
dwAllocationGranularity : 65536
dwNumberOfProcessors : 4
dwOemId : 0
dwPageSize : 4096
dwProcessorType : 586
lpMaximumApplicationAddress : 7ffeffff
lpMinimumApplicationAddress : 10000
wProcessorArchitecture : 0
wProcessorLevel : 6
wProcessorRevision : 17665
wReserved : 0
Native System Info :
dwActiveProcessorMask : 15
dwAllocationGranularity : 65536
dwNumberOfProcessors : 4
dwOemId : 9
dwPageSize : 4096
dwProcessorType : 8664
lpMaximumApplicationAddress : fffeffff
lpMinimumApplicationAddress : 10000
wProcessorArchitecture : 9
wProcessorLevel : 6
wProcessorRevision : 17665
wReserved : 0
+ one CPU core (HyperThreading)
+ one CPU core (HyperThreading)
-> 2 active CPU(s)
IsWow64Process 1


*/
