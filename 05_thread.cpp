
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

class CStopwatch
{
public:
	CStopwatch()
	{
		QueryPerformanceFrequency(&m_liPerfFreq);
		Start();
	}
	void Start()
	{
		QueryPerformanceCounter(&m_liPerfStart);
	}

	__int64 Now() const { // Returns # of milliseconds since Start was called
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		return(((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000)
			/ m_liPerfFreq.QuadPart);
	}

	__int64 NowInMicro() const { // Returns # of microseconds
		// since Start was called
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		return(((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000000)
			/ m_liPerfFreq.QuadPart);
	}

private:
	LARGE_INTEGER m_liPerfFreq; // Counts per second
	LARGE_INTEGER m_liPerfStart; // Starting count
};

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

void ShowThreadContext(CONTEXT context)
{
	_tprintf(TEXT("  EFLAGS : "));
	PrintBinary(context.EFlags);
	_tprintf(TEXT("  Dr0 : %x\n"), context.Dr0);
	_tprintf(TEXT("  Dr1 : %x\n"), context.Dr1);
	_tprintf(TEXT("  Dr2 : %x\n"), context.Dr2);
	_tprintf(TEXT("  Dr3 : %x\n"), context.Dr3);
	_tprintf(TEXT("  Dr4 : %x\n"), context.Dr6);
	_tprintf(TEXT("  Dr7 : %x\n"), context.Dr7);
	_tprintf(TEXT("  Eax : %x\n"), context.Eax);
	_tprintf(TEXT("  Ebx : %x\n"), context.Ebx);
	_tprintf(TEXT("  Ecx : %x\n"), context.Ecx);
	_tprintf(TEXT("  Edx : %x\n"), context.Edx);
	_tprintf(TEXT("  Esi : %x\n"), context.Esi);
	_tprintf(TEXT("  Edi : %x\n"), context.Edi);
	_tprintf(TEXT("  Ebp : %x\n"), context.Ebp);
	_tprintf(TEXT("  Eip : %x\n"), context.Eip);
	_tprintf(TEXT("  Esp : %x\n"), context.Esp);
	_tprintf(TEXT("  SegCs : %x\n"), context.SegCs);
	_tprintf(TEXT("  SegDs : %x\n"), context.SegDs);
	_tprintf(TEXT("  SegEs : %x\n"), context.SegEs);
	_tprintf(TEXT("  SegFs : %x\n"), context.SegFs);
	_tprintf(TEXT("  SegGs : %x\n"), context.SegGs);
	_tprintf(TEXT("  SegSs : %x\n"), context.SegSs);
}



DWORD WINAPI ThreadFunction(VOID* pvParam)
{
	INT x = *((INT*)pvParam);
	FILETIME ftCreation, ftExit, ftKernel, ftUser;
	HANDLE hThread = GetCurrentThread();

	_tprintf(TEXT("ReadThreadTimeStamp %u\n"), __rdtsc());
	GetThreadTimes(hThread, &ftCreation, &ftExit, &ftKernel, &ftUser);
	_tprintf(TEXT("Creation High: %u, Low: %u, %u\n"),
		ftCreation.dwHighDateTime, ftCreation.dwLowDateTime);
	_tprintf(TEXT("Exit High: %u, Low: %u\n"),
		ftExit.dwHighDateTime, ftExit.dwLowDateTime);
	_tprintf(TEXT("Kernel High: %u, Low: %u\n"),
		ftKernel.dwHighDateTime, ftKernel.dwLowDateTime);
	_tprintf(TEXT("User High: %u, Low: %u\n"),
		ftUser.dwHighDateTime, ftUser.dwLowDateTime);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	DWORD dThreadID;
	HANDLE hThread;
	INT x = 100;
	CONTEXT context;
	// Create a stopwatch timer (which defaults to the current time).
	CStopwatch stopwatch;
	DWORD dProcessAffinity, dSystemAffinity;

	// Execute the code I want to profile here.
	// Get how much time has elapsed up to now.
	__int64 qwElapsedTime = stopwatch.Now();
	// qwElapsedTime indicates how long the profiled code
	// executed in milliseconds.
	_tprintf(TEXT("ElapsedTime %u\n"), qwElapsedTime);

	GetProcessAffinityMask(GetCurrentProcess(), &dProcessAffinity, &dSystemAffinity);
	_tprintf(TEXT("ProcessAffinity %u , SystemAffinity %u\n"), dProcessAffinity, dSystemAffinity);

	hThread = CreateThread(NULL, 10, ThreadFunction, (VOID*)&x,
		THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME, &dThreadID);
	if (hThread == INVALID_HANDLE_VALUE || hThread == NULL){
		_tprintf(TEXT("error to create thread\n"));
		return 1;
	}
	else{
		_tprintf(TEXT("ReadTimeStamp %u\n"), __rdtsc());
		_tprintf(TEXT("ThreadId : %d\n"), dThreadID);

		//suspend thread to get context
		SuspendThread(hThread);
		ZeroMemory(&context, sizeof(CONTEXT));
		context.ContextFlags = CONTEXT_ALL;
		GetThreadContext(hThread, &context);
		_tprintf(TEXT("\nThreadContext : \n"));
		ShowThreadContext(context);
		//resume thread
		ResumeThread(hThread);

		WaitForSingleObject(hThread, 10);
		TerminateThread(hThread, 0);
	}
	CloseHandle(hThread);


	getchar();

	return 0;
}



/*
Output :

ElapsedTime 0
ProcessAffinity 15 , SystemAffinity 15
ReadTimeStamp 1389582202
ThreadId : 10284

ThreadContext :
EFLAGS : 0100100001
Dr0 : 0
Dr1 : 0
Dr2 : 0
Dr3 : 0
Dr4 : 0
Dr7 : 0
Eax : 2b1127
Ebx : f3f72c
Ecx : 0
Edx : 0
Esi : 0
Edi : 0
Ebp : 164fb24
Eip : 7729ad0c
Esp : 164fb10
SegCs : 23
SegDs : 2b
SegEs : 2b
SegFs : 53
SegGs : 2b
SegSs : 2b
ReadThreadTimeStamp 1396071282
Creation High: 30728646, Low: 277185493, 2822439
Exit High: 0, Low: 0
Kernel High: 0, Low: 0
User High: 0, Low: 0


*/
