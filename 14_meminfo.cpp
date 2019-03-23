
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<Psapi.h>
#include<stdlib.h>
#include<string.h>

void ShowGlobalMemoryStatus()
{
	MEMORYSTATUS ms;
	ms.dwLength = sizeof(MEMORYSTATUS);

	GlobalMemoryStatus(&ms);

	_tprintf(TEXT("Memory Status : \n"));
	_tprintf(TEXT("  dwAvailPageFile : %x\n"), ms.dwAvailPageFile);
	_tprintf(TEXT("  dwAvailPhys : %x\n"), ms.dwAvailPhys);
	_tprintf(TEXT("  dwAvailVirtual : %x\n"), ms.dwAvailVirtual);
	_tprintf(TEXT("  dwMemoryLoad : %u\n"), ms.dwMemoryLoad);
	_tprintf(TEXT("  dwTotalPageFile : %x\n"), ms.dwTotalPageFile);
	_tprintf(TEXT("  dwTotalPhys : %x\n"), ms.dwTotalPhys);
	_tprintf(TEXT("  dwTotalVirtual : %x\n"), ms.dwTotalVirtual);
}

void ShowProcessMemoryInfo()
{
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(PROCESS_MEMORY_COUNTERS));

	_tprintf(TEXT("Process Memory Info : \n"));
	_tprintf(TEXT("  PageFaultCount : %u\n"), pmc.PageFaultCount);
	_tprintf(TEXT("  PagefileUsage : %u\n"), pmc.PagefileUsage);
	_tprintf(TEXT("  PeakPagefileUsage : %u\n"), pmc.PeakPagefileUsage);
	_tprintf(TEXT("  PeakWorkingSetSize : %u\n"), pmc.PeakWorkingSetSize);
	_tprintf(TEXT("  QuotaNonPagedPoolUsage : %u\n"), pmc.QuotaNonPagedPoolUsage);
	_tprintf(TEXT("  QuotaPagedPoolUsage : %u\n"), pmc.QuotaPagedPoolUsage);
	_tprintf(TEXT("  QuotaPeakNonPagedPoolUsage : %u\n"), pmc.QuotaPeakNonPagedPoolUsage);
	_tprintf(TEXT("  QuotaPeakPagedPoolUsage : %u\n"), pmc.QuotaPeakPagedPoolUsage);
	_tprintf(TEXT("  WorkingSetSize : %u\n"), pmc.WorkingSetSize);
}

void ShowMemoryState()
{
	MEMORY_BASIC_INFORMATION mbi;
	LPCVOID lpAddress = NULL;
	VirtualQuery(lpAddress, &mbi, sizeof(mbi));
	_tprintf(TEXT("MemoryState : \n"));
	_tprintf(TEXT("  AllocationBase : %x\n"), mbi.AllocationBase);
	_tprintf(TEXT("  AllocationProtect : %u\n"), mbi.AllocationProtect);
	_tprintf(TEXT("  BaseAddress : %x\n"), mbi.BaseAddress);
	_tprintf(TEXT("  Protect : %u\n"), mbi.Protect);
	_tprintf(TEXT("  RegionSize : %u\n"), mbi.RegionSize);
	_tprintf(TEXT("  State : %s\n"), mbi.State == MEM_FREE ? TEXT("FREE") : TEXT("RESERVE"));
	_tprintf(TEXT("  Type : %u\n"), mbi.Type);

}

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	ShowGlobalMemoryStatus();

	ShowProcessMemoryInfo();

	ShowMemoryState();

	getchar();

	return 0;
}




/*
Output :

Memory Status :
dwAvailPageFile : 9e585000
dwAvailPhys : 596c8000
dwAvailVirtual : 7f116000
dwMemoryLoad : 64
dwTotalPageFile : ffffffff
dwTotalPhys : 7fffffff
dwTotalVirtual : 7ffe0000
Process Memory Info :
PageFaultCount : 749
PagefileUsage : 794624
PeakPagefileUsage : 794624
PeakWorkingSetSize : 2842624
QuotaNonPagedPoolUsage : 4544
QuotaPagedPoolUsage : 27112
QuotaPeakNonPagedPoolUsage : 5224
QuotaPeakPagedPoolUsage : 27112
WorkingSetSize : 2842624
MemoryState :
AllocationBase : 0
AllocationProtect : 0
BaseAddress : 0
Protect : 1
RegionSize : 458752
State : FREE
Type : 0


*/
