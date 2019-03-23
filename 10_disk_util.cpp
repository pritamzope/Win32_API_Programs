
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>
#include<WinBase.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<list>
#include<string>

LPCWSTR wszDrive = TEXT("\\\\.\\PhysicalDrive0");

#define LOG_ERROR 0x01
#define LOG_INFO 0x02

void Log(PSTR *logmsg, int logtype)
{
	if (logtype == LOG_ERROR){
		_tprintf(TEXT("Log Error : %s\n"), logmsg);
	}
	else if (logtype == LOG_INFO){
		_tprintf(TEXT("Log Info : %s\n"), logmsg);
	}
}

LONG GetRegKeyValueData(LPCWSTR regPath, LPCWSTR value, LPBYTE data)
{
	HKEY hKey;
	DWORD size = 512;
	DWORD Type;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, value, NULL, &Type, data, &size);
	}

	RegCloseKey(hKey);

	return GetLastError();
}


BOOL GetDriveGeometry(DISK_GEOMETRY *pdg, LPCWSTR drivePath)
{
	HANDLE hDevice;               // handle to the drive to be examined
	BOOL bResult;                 // results flag
	DWORD junk;                   // discard results

	hDevice = CreateFile(drivePath,  // drive to open
		0,                // no access to the drive
		FILE_SHARE_READ | // share mode
		FILE_SHARE_WRITE,
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL);            // do not copy file attributes

	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		printf("CreateFile() failed!\n");
		return (FALSE);
	}

	bResult = DeviceIoControl(hDevice,  // device to be queried
		IOCTL_DISK_GET_DRIVE_GEOMETRY,  // operation to perform
		NULL, 0, // no input buffer
		pdg, sizeof(*pdg),     // output buffer
		&junk,                 // # bytes returned
		(LPOVERLAPPED)NULL);  // synchronous I/O

	CloseHandle(hDevice);

	return (bResult);
}

DWORD GetDWORDFromLPBYTE(LPBYTE lpData)
{
	DWORD dWordData = *(unsigned char *)lpData;
	return dWordData;
}

DWORD GetDriveCount()
{
	DWORD diskCount = 0;
	LPCWSTR wszRegKeyPath = TEXT("SYSTEM\\CurrentControlSet\\Services\\disk\\Enum");
	LPCWSTR subKey = TEXT("Count");

	BYTE data[64];
	GetRegKeyValueData(wszRegKeyPath, subKey, (LPBYTE)data);

	diskCount = GetDWORDFromLPBYTE((LPBYTE)data);

	return diskCount;
}

std::list<std::wstring> GetPhysicalDrivesPathList()
{
	std::list<std::wstring> phyDrivesPathList;
	DWORD driveCount = GetDriveCount();
	std::wstring defaultDrivePath = TEXT("\\\\.\\PhysicalDrive");
	std::wstring temp;

	for (int drive = 0; drive < driveCount; drive++){
		temp = defaultDrivePath;
		temp += std::to_wstring(drive);
		phyDrivesPathList.push_back(temp);
		temp.clear();
	}
	return phyDrivesPathList;
}

void PrintGeometryStructure(LPBYTE drivePath, DISK_GEOMETRY diskGeometry)
{
	ULONGLONG diskSize;

	_tprintf(TEXT("Drive path = %s\n"), drivePath);
	_tprintf(TEXT("Cylinders = %I64d\n"), diskGeometry.Cylinders);
	_tprintf(TEXT("Tracks/cylinder = %ld\n"), (ULONG)diskGeometry.TracksPerCylinder);
	_tprintf(TEXT("Sectors/track = %ld\n"), (ULONG)diskGeometry.SectorsPerTrack);
	_tprintf(TEXT("Bytes/sector = %ld\n"), (ULONG)diskGeometry.BytesPerSector);

	diskSize = diskGeometry.Cylinders.QuadPart *
		(ULONG)diskGeometry.TracksPerCylinder *
		(ULONG)diskGeometry.SectorsPerTrack *
		(ULONG)diskGeometry.BytesPerSector;

	_tprintf(TEXT("Disk size : \n"));
	_tprintf(TEXT("\tBytes = %I64d\n"), diskSize);
	_tprintf(TEXT("\tGB = %I64d\n"), diskSize / (1024 * 1024 * 1024));
}

void DisplayPhysicalDrivesGeometry(std::list<std::wstring>& drivesList)
{
	DISK_GEOMETRY diskGeometry;
	BOOL bResult;

	std::list<std::wstring>::iterator it = drivesList.begin();

	while (it != drivesList.end()){
		bResult = GetDriveGeometry(&diskGeometry, (*it).c_str());
		if (bResult){
			PrintGeometryStructure((LPBYTE)(*it).c_str(), diskGeometry);
			std::cout << std::endl;
			memset(&diskGeometry, 0, sizeof(DISK_GEOMETRY));
		}
		else{
			_tprintf(TEXT("Error to get disk geometry information..."));
		}
		it++;
	}
}

void PrintLogicalVolumesSpace(LPCWSTR volumeName)
{
	DWORD dSectorsPerCluster;
	DWORD dBytesPerSector;
	DWORD dNumberOfFreeClusters;
	DWORD dTotalNumberOfClusters;
	LONG64 ulGBSize;

	GetDiskFreeSpace(volumeName, &dSectorsPerCluster, &dBytesPerSector,
		&dNumberOfFreeClusters, &dTotalNumberOfClusters);

	_tprintf(TEXT("  dSectorsPerCluster %u\n"), dSectorsPerCluster);
	_tprintf(TEXT("  dBytesPerSector %u\n"), dBytesPerSector);
	_tprintf(TEXT("  dNumberOfFreeClusters %u\n"), dNumberOfFreeClusters);
	_tprintf(TEXT("  dTotalNumberOfClusters %u\n"), dTotalNumberOfClusters);
	ulGBSize = ((LONG64)dTotalNumberOfClusters * (LONG64)dBytesPerSector * (LONG64)dSectorsPerCluster);
	_tprintf(TEXT("  GB = %I64d\n\n"), ulGBSize / (1024 * 1024 * 1024));
}


void PrintLogicalDrives()
{
	DWORD dwSize = MAX_PATH;
	TCHAR szLogicalDrives[MAX_PATH] = { 0 };
	DWORD dwResult = GetLogicalDriveStrings(dwSize, szLogicalDrives);

	if (dwResult > 0 && dwResult <= MAX_PATH)
	{
		PWSTR szSingleDrive = szLogicalDrives;
		while (*szSingleDrive)
		{
			_tprintf(TEXT("Drive: %s\n"), szSingleDrive);
			PrintLogicalVolumesSpace(szSingleDrive);

			// get the next drive
			szSingleDrive += wcsnlen_s(szSingleDrive, 255) + 1;
		}
	}
}

BOOL GetBootAreaInfo(BOOT_AREA_INFO *bootInfo, LPCWSTR drivePath)
{
	HANDLE hDevice;               // handle to the drive to be examined
	BOOL bResult;                 // results flag
	DWORD junk;                   // discard results

	hDevice = CreateFile(drivePath,  // drive to open
		0,                // no access to the drive
		FILE_SHARE_READ | // share mode
		FILE_SHARE_WRITE,
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL);            // do not copy file attributes

	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		_tprintf(TEXT("CreateFile() failed!\n"));
		return (FALSE);
	}

	bResult = DeviceIoControl(hDevice,  // device to be queried
		FSCTL_GET_BOOT_AREA_INFO,  // operation to perform
		NULL, 0, // no input buffer
		bootInfo,
		sizeof(*bootInfo),     // output buffer
		&junk,                 // # bytes returned
		(LPOVERLAPPED)NULL);  // synchronous I/O

	CloseHandle(hDevice);

	return (bResult);
}



int main(int argc, char *argv[])
{

	std::list<std::wstring> drivesList = GetPhysicalDrivesPathList();

	DisplayPhysicalDrivesGeometry(drivesList);

	PrintLogicalDrives();

	getchar();


	return 0;

}



/*
Output :

Drive path = \\.\PhysicalDrive0
Cylinders = 121601
Tracks/cylinder = 255
Sectors/track = 63
Bytes/sector = 512
Disk size :
Bytes = 1000202273280
GB = 931

Drive: C:\
dSectorsPerCluster 8
dBytesPerSector 512
dNumberOfFreeClusters 41266656
dTotalNumberOfClusters 76671743
GB = 292

Drive: E:\
dSectorsPerCluster 8
dBytesPerSector 512
dNumberOfFreeClusters 46647765
dTotalNumberOfClusters 138534399
GB = 528

Drive: F:\
dSectorsPerCluster 3435973836
dBytesPerSector 0
dNumberOfFreeClusters 3435973836
dTotalNumberOfClusters 3435973836
GB = 0



*/
