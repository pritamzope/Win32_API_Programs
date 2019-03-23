
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>

PTP_WORK g_pWork = NULL;
volatile int g_X = 0;

void CALLBACK TaskHandler(PTP_CALLBACK_INSTANCE pInstance, PVOID Context, PTP_WORK work)
{
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(Context);
	UNREFERENCED_PARAMETER(work);
	g_X++;
	Sleep(5);
}

int main(int argc, char *argv[])
{
	TP_CALLBACK_ENVIRON tpCallEnviron;
	PTP_POOL tpPool;

	InitializeThreadpoolEnvironment(&tpCallEnviron);

	tpPool = CreateThreadpool(NULL);
	if (tpPool == NULL){
		_tprintf(TEXT("Error CreateThreadpool %u\n"), GetLastError());
	}

	SetThreadpoolThreadMaximum(tpPool, 1);

	SetThreadpoolCallbackPool(&tpCallEnviron, tpPool);

	g_pWork = CreateThreadpoolWork(TaskHandler, NULL, &tpCallEnviron);
	if (g_pWork == NULL){
		_tprintf(TEXT("Error CreateThreadpoolWork %u\n"), GetLastError());
	}
	else{
		//submit work 3 times, g_X = 3
		SubmitThreadpoolWork(g_pWork);
		SubmitThreadpoolWork(g_pWork);
		SubmitThreadpoolWork(g_pWork);

		WaitForThreadpoolWorkCallbacks(g_pWork, FALSE);

		CloseThreadpoolWork(g_pWork);
	}

	_tprintf(TEXT("g_X = %d\n"), g_X);

	getchar();


	return 0;

}



/*
Output :

g_X = 3


*/
