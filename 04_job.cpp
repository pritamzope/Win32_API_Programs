
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>


int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	BOOL bInJob = FALSE;
	HANDLE hjob;
	JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };
	JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	TCHAR szCmdLine[16] = TEXT("notepad.exe");
	HANDLE handles[2];
	DWORD dwResult;

	//check if already in job, even if in default job, continue to next
	IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);
	if (bInJob) {
		_tprintf(TEXT("Process is already in job\n"));
	}

	hjob = CreateJobObject(NULL, TEXT("MyJob"));
	// The process always runs in the idle priority class
	jobli.PriorityClass = IDLE_PRIORITY_CLASS;
	jobli.PerJobUserTimeLimit.QuadPart = 1;	// 1 ns
	jobli.LimitFlags = JOB_OBJECT_LIMIT_PRIORITY_CLASS | JOB_OBJECT_LIMIT_JOB_TIME;

	SetInformationJobObject(hjob, JobObjectBasicLimitInformation, &jobli, sizeof(jobli));

	_tprintf(TEXT("Notepad will close after few seconds automatically\n"));

	//set some UI restrictions.
	jobuir.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;
	// The process can't log off the system.
	jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;
	// The process can't access USER objects (such as other windows) in the system
	jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;

	SetInformationJobObject(hjob, JobObjectBasicUIRestrictions, &jobuir, sizeof(jobuir));

	// create process that is to be in the job
	BOOL bResult = CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE,
		CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	// Place the process in the job.
	AssignProcessToJobObject(hjob, pi.hProcess);

	// Now we can allow the child process thread to execute code
	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);

	// Wait for the process to terminate or
	// for all the job's allotted CPU time to be used
	dwResult = WaitForMultipleObjects(2, handles, FALSE, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(hjob);


	getchar();

	return 0;
}



/*
Output :

Process is already in job
Notepad will close after few seconds automatically

*/
