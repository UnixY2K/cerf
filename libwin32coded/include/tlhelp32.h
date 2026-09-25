#pragma once
#include "windows.h"

#define TH32CS_SNAPTHREAD        0x00000004
#define THREAD_SUSPEND_RESUME    0x0002
#define THREAD_GET_CONTEXT       0x0008
#define THREAD_QUERY_INFORMATION 0x0040
#define THREAD_QUERY_LIMITED_INFORMATION 0x0800

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
HANDLE OpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
DWORD SuspendThread(HANDLE hThread);
BOOL GetThreadContext(HANDLE hThread, LPCONTEXT lpContext);
BOOL IsBadReadPtr(const void *lp, SIZE_T ucb);
