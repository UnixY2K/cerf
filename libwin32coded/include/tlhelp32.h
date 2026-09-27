#pragma once
#include "windows.h"

#define TH32CS_SNAPTHREAD        0x00000004

typedef struct _THREADENTRY32 {
	DWORD dwSize;
	DWORD cntUsage;
	DWORD th32ThreadID;
	DWORD th32OwnerProcessID;
	LONG tpBasePri;
	LONG tpDeltaPri;
	DWORD dwFlags;
} THREADENTRY32, *LPTHREADENTRY32;

HANDLE CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
BOOL Thread32First(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
BOOL Thread32Next(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
