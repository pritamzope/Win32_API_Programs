
#include "stdafx.h"
#include<tchar.h>
#include<Windows.h>
#include<Shlwapi.h>

BOOL StringReverseW(PWSTR pWideCharStr, DWORD cchLength)
{
	PWSTR pEndOfStr = pWideCharStr + wcsnlen_s(pWideCharStr, cchLength) - 1;
	wchar_t cCharT;
	while (pWideCharStr < pEndOfStr) {
		cCharT = *pWideCharStr;
		*pWideCharStr = *pEndOfStr;
		*pEndOfStr = cCharT;
		pWideCharStr++;
		pEndOfStr--;
	}
	return(TRUE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	CHAR ch[10] = "abcd";
	TCHAR aa[10] = L"hello";
	TCHAR bb[10] = L"world";
	CHAR buf[5], buf2[40];
	HRESULT hr;
	LCID lcid;
	LPCVOID v;
	CHAR wc[100];
	HLOCAL hlc;
	TCHAR rvws[20] = L"??,??";
	TCHAR rvws2[20] = L"?? ,??";

	lcid = GetThreadLocale();
	hr = CompareString(lcid, NORM_IGNORECASE, aa, _countof(aa), bb, _countof(bb));
	if (hr == CSTR_EQUAL){
		printf("equal %d\n", lcid);
	}
	else if (hr == CSTR_GREATER_THAN){
		printf("string1 greater than 2\n");
	}
	else if (hr == CSTR_LESS_THAN){
		printf("string1 less than 2\n");
	}

	hr = WideCharToMultiByte(0, 0, bb, _countof(bb), buf, _countof(buf), NULL, FALSE);
	if (hr){
		printf("buf = %s\n", buf);
	}
	else{
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			GetLastError(), lcid, (PWSTR)&hlc, 0, NULL);

		printf("error %ws\n", hlc);
		LocalFree(hlc);
	}

	if (IsTextUnicode(ch, _countof(ch), NULL)){
		printf("Unicode\n");
	}
	else{
		printf("Not unicode\n");
	}

	StringReverseW(rvws, _countof(rvws));
	hr = CompareString(lcid, NORM_IGNORECASE, rvws, _countof(rvws), rvws2, _countof(rvws2));
	if (hr == CSTR_EQUAL){
		printf("equal\n");
	}
	else if (hr == CSTR_GREATER_THAN){
		printf("string1 greater than 2\n");
	}
	else if (hr == CSTR_LESS_THAN){
		printf("string1 less than 2\n");
	}

	getchar();

	return 0;
}

/*
Output :

string1 less than 2
error The data area passed to a system call is too small.

Not unicode
string1 greater than 2

*/
