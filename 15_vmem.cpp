
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<Psapi.h>
#include<stdlib.h>
#include<string.h>

#define PAGESIZE 4096

void ShowMemoryState(LPCVOID lpAddress)
{
	MEMORY_BASIC_INFORMATION mbi;
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

void UseAllocatedRegion(LPCVOID pRegion)
{
	PDWORD pArray = (DWORD*)pRegion;
	DWORD i;
	for (i = 0; i < 10; i++){
		pArray[i] = i;
	}
}

void PrintUsedRegion(LPCVOID pRegion)
{
	PDWORD pArray = (DWORD*)pRegion;
	DWORD i;
	for (i = 0; i < 10; i++){
		_tprintf(TEXT("i = %u\n"), pArray[i]);
	}
}

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	PVOID pRegion = NULL;
	PVOID pRegionCommit = NULL;
	ULONG ulNoOfPages = 10;
	ULONG_PTR PageArray[10];

	pRegion = VirtualAlloc(NULL, 10 * GetLargePageMinimum(), MEM_RESERVE, PAGE_READWRITE);
	if (pRegion == NULL){
		_tprintf(TEXT("Error Reserve Memory\n"));
	}
	else{
		_tprintf(TEXT("pRegion : %x\n"), pRegion);
	}

	pRegionCommit = VirtualAlloc((PVOID)pRegion, 10 * GetLargePageMinimum(), MEM_COMMIT, PAGE_READWRITE);
	if (pRegionCommit == NULL){
		_tprintf(TEXT("Error Commit Memory\n"));
	}
	else{
		_tprintf(TEXT("pRegionCommit : %x\n"), pRegionCommit);

		//allocate pages of RAM
		AllocateUserPhysicalPages(GetCurrentProcess(), &ulNoOfPages, PageArray);

		MapUserPhysicalPages(pRegionCommit, ulNoOfPages, PageArray);

		ShowMemoryState(pRegionCommit);

		UseAllocatedRegion(pRegionCommit);
		PrintUsedRegion(pRegionCommit);

		FreeUserPhysicalPages(GetCurrentProcess(), &ulNoOfPages, PageArray);

		VirtualFree(pRegionCommit, 0, MEM_DECOMMIT);
		VirtualFree(pRegion, 0, MEM_RELEASE);

		ShowMemoryState(pRegionCommit);
	}

	getchar();

	return 0;
}




/*
Output :

pRegion : 13f0000
pRegionCommit : 13f0000
MemoryState :
AllocationBase : 13f0000
AllocationProtect : 4
BaseAddress : 13f0000
Protect : 4
RegionSize : 20971520
State : RESERVE
Type : 131072
i = 0
i = 1
i = 2
i = 3
i = 4
i = 5
i = 6
i = 7
i = 8
i = 9
MemoryState :
AllocationBase : 0
AllocationProtect : 0
BaseAddress : 13f0000
Protect : 1
RegionSize : 1882456064
State : FREE
Type : 0


*/
