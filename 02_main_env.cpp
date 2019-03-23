
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>

void DumpEnvVariables(PTSTR pEnvBlock[])
{
	int current = 0;
	PTSTR* pElement = (PTSTR*)pEnvBlock;
	PTSTR pCurrent = NULL;
	while (pElement != NULL) {
		pCurrent = (PTSTR)(*pElement);
		if (pCurrent == NULL) {
			pElement = NULL;
		}
		else {
			_tprintf(TEXT("[%u] %s\r\n"), current, pCurrent);
			current++;
			pElement++;
		}
	}
}
int _tmain(int argc, _TCHAR* argv[], TCHAR* env[])
{
	DumpEnvVariables(env);

	getchar();

	return 0;
}



/*
Output :

[0] ALLUSERSPROFILE=C:\ProgramData
[1] APPDATA=C:\Users\Pritam\AppData\Roaming
[2] CommonProgramFiles=C:\Program Files (x86)\Common Files
[3] CommonProgramFiles(x86)=C:\Program Files (x86)\Common Files
[4] CommonProgramW6432=C:\Program Files\Common Files
[5] COMPUTERNAME=DESKTOP-O1CL405
[6] ComSpec=C:\WINDOWS\system32\cmd.exe
[7] DriverData=C:\Windows\System32\Drivers\DriverData
[8] DXSDK_DIR=C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\
[9] FPS_BROWSER_APP_PROFILE_STRING=Internet Explorer
[10] FPS_BROWSER_USER_PROFILE_STRING=Default
[11] GTK_BASEPATH=C:\Program Files (x86)\GtkSharp\2.12\
[12] HOMEDRIVE=C:
[13] HOMEPATH=\Users\Pritam
[14] LOCALAPPDATA=C:\Users\Pritam\AppData\Local
[15] LOGONSERVER=\\DESKTOP-O1CL405
[16] MSBuildLoadMicrosoftTargetsReadOnly=true
[17] MSBuildTreatAllToolsVersionsAsCurrent=true
[18] NUMBER_OF_PROCESSORS=4
[19] OneDrive=C:\Users\Pritam\OneDrive
[20] OS=Windows_NT
[21] PATH=C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\ProgramData\Oracle\Java\javapath;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\Microsoft SQL Server\130\Tools\Binn\;C:\Program Files\dotnet\;C:\Program Files\Git\cmd;C:\Program Files (x86)\GtkSharp\2.12\bin;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\MinGW\bin\;C:\Program Files (x86)\Notepad++;C:\Program Files (x86)\AMD\ATI.ACE\Core-Static;C:\WINDOWS\System32\OpenSSH\;C:\Program Files\Microsoft VS Code\bin;;C:\Program Files\MongoDB\Server\3.6\bin;C:\Users\Pritam\AppData\Local\Microsoft\WindowsApps;C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\Extensions\Microsoft\VsGraphics;C:\Program Files (x86)\Microsoft Visual Studio 12.0\;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin
[22] PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC
[23] PkgDefApplicationConfigFile=C:\Users\Pritam\AppData\Local\Microsoft\VisualStudio\12.0\devenv.exe.config
[24] PROCESSOR_ARCHITECTURE=x86
[25] PROCESSOR_ARCHITEW6432=AMD64
[26] PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 69 Stepping 1, GenuineIntel
[27] PROCESSOR_LEVEL=6
[28] PROCESSOR_REVISION=4501
[29] ProgramData=C:\ProgramData
[30] ProgramFiles=C:\Program Files (x86)
[31] ProgramFiles(x86)=C:\Program Files (x86)
[32] ProgramW6432=C:\Program Files
[33] PSModulePath=C:\WINDOWS\system32\WindowsPowerShell\v1.0\Modules\
[34] PUBLIC=C:\Users\Public
[35] SESSIONNAME=Console
[36] SystemDrive=C:
[37] SystemRoot=C:\WINDOWS
[38] TEMP=C:\Users\Pritam\AppData\Local\Temp
[39] TMP=C:\Users\Pritam\AppData\Local\Temp
[40] USERDOMAIN=DESKTOP-O1CL405
[41] USERDOMAIN_ROAMINGPROFILE=DESKTOP-O1CL405
[42] USERNAME=Pritam
[43] USERPROFILE=C:\Users\Pritam
[44] VisualStudioDir=C:\Users\Pritam\Documents\Visual Studio 2013
[45] VisualStudioEdition=Microsoft Visual Studio Professional 2013
[46] VisualStudioVersion=12.0
[47] VS120COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\
[48] VSLANG=1033
[49] windir=C:\WINDOWS
[50] _PTVS_PID=3720


*/
