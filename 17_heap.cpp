
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<Psapi.h>
#include<stdlib.h>
#include<string.h>

void ShowHeapWalk(HANDLE hHeap)
{
	PROCESS_HEAP_ENTRY phe;
	phe.lpData = NULL;
	if (HeapWalk(hHeap, &phe)){
		_tprintf(TEXT("HeapWalk : %x\n"), hHeap);
		_tprintf(TEXT("  lpData : %x\n"), phe.lpData);
		_tprintf(TEXT("  cbData : %u\n"), phe.cbData);
		_tprintf(TEXT("  cbOverhead : %u\n"), phe.cbOverhead);
		_tprintf(TEXT("  iRegionIndex : %u\n"), phe.iRegionIndex);
		_tprintf(TEXT("  wFlags : %u\n"), phe.wFlags);
		_tprintf(TEXT("  Block.hMem : %x\n"), phe.Block.hMem);
		_tprintf(TEXT("  Region.dwCommittedSize : %u\n"), phe.Region.dwCommittedSize);
		_tprintf(TEXT("  Region.dwUnCommittedSize : %u\n"), phe.Region.dwUnCommittedSize);
		_tprintf(TEXT("  Region.lpFirstBlock : %x\n"), phe.Region.lpFirstBlock);
		_tprintf(TEXT("  Region.lpLastBlock : %x\n"), phe.Region.lpLastBlock);
	}
}

typedef struct _NODE{
	DWORD dData;
	struct _NODE* pNext;
}NODE, *PNODE;

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	HANDLE hHeap;
	HANDLE hProcessHeap;
	PNODE head = NULL, temp = NULL;
	PNODE node_1, node_2, node_3;

	// 0 for growable heap, here 4096 - 1M size
	hHeap = HeapCreate(0, 4096, 1024 * 1024);

	//Heap lock/unlock for thread synchronization
	HeapLock(hHeap);

	hProcessHeap = GetProcessHeap();

	ShowHeapWalk(hProcessHeap);
	ShowHeapWalk(hHeap);

	node_1 = (PNODE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(NODE));
	node_2 = (PNODE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(NODE));
	node_3 = (PNODE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(NODE));

	node_1->dData = 100;
	node_2->dData = 200;
	node_3->dData = 300;
	node_1->pNext = node_2;
	node_2->pNext = node_3;
	node_3->pNext = NULL;
	head = node_1;

	_tprintf(TEXT("node_1 : data  %u, node %x, next %x\n"),
		head->dData, head, head->pNext);
	_tprintf(TEXT("node_2 : data  %u, node %x, next %x\n"),
		head->pNext->dData, head->pNext, head->pNext->pNext);
	_tprintf(TEXT("node_3 : data  %u, node %x, next %x\n"),
		head->pNext->pNext->dData, head->pNext->pNext, head->pNext->pNext->pNext);

	temp = head->pNext;
	HeapFree(hHeap, 0, (PVOID)head);
	head = temp;
	temp = head->pNext;
	HeapFree(hHeap, 0, (PVOID)head);
	head = temp;
	HeapFree(hHeap, 0, (PVOID)head);
	head = NULL;

	HeapUnlock(hHeap);

	HeapDestroy(hHeap);

	getchar();

	return 0;
}




/*
Output :

HeapWalk : 7c0000
lpData : 7c0000
cbData : 1176
cbOverhead : 0
iRegionIndex : 0
wFlags : 1
Block.hMem : e000
Region.dwCommittedSize : 57344
Region.dwUnCommittedSize : 991232
Region.lpFirstBlock : 7c04a8
Region.lpLastBlock : 8bf000
HeapWalk : a60000
lpData : a60000
cbData : 1176
cbOverhead : 0
iRegionIndex : 0
wFlags : 1
Block.hMem : 1000
Region.dwCommittedSize : 4096
Region.dwUnCommittedSize : 1044480
Region.lpFirstBlock : a604a8
Region.lpLastBlock : b60000
node_1 : data  100, node a604a0, next a604c0
node_2 : data  200, node a604c0, next a604e0
node_3 : data  300, node a604e0, next 0


*/
