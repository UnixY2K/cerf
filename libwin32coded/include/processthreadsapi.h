#pragma once
#include "windef.h"
#include "wtypesbase.h"

typedef struct _STARTUPINFOW {
    DWORD cb;
    LPWSTR lpReserved;
    LPWSTR lpDesktop;
    LPWSTR lpTitle;
    DWORD dwX;
    DWORD dwY;
    DWORD dwXSize;
    DWORD dwYSize;
    DWORD dwXCountChars;
    DWORD dwYCountChars;
    DWORD dwFillAttribute;
    DWORD dwFlags;
    WORD wShowWindow;
    WORD cbReserved2;
    unsigned char *lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct _PROCESS_POWER_THROTTLING_STATE {
    DWORD Version;
    DWORD ControlMask;
    DWORD StateMask;
} PROCESS_POWER_THROTTLING_STATE;

#define PROCESS_POWER_THROTTLING_CURRENT_VERSION         1
#define PROCESS_POWER_THROTTLING_EXECUTION_SPEED         0x1
#define PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION 0x4
#define ProcessPowerThrottling 4

typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);

BOOL CreateProcessW(LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
                    LPSECURITY_ATTRIBUTES lpProcessAttributes,
                    LPSECURITY_ATTRIBUTES lpThreadAttributes,
                    BOOL bInheritHandles, DWORD dwCreationFlags,
                    LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
                    LPSTARTUPINFOW lpStartupInfo,
                    LPPROCESS_INFORMATION lpProcessInformation);

DWORD GetCurrentProcessId(void);
DWORD GetCurrentThreadId(void);
HANDLE GetCurrentProcess(void);
HANDLE GetCurrentThread(void);
BOOL TerminateProcess(HANDLE hProcess, UINT uExitCode);
BOOL GetExitCodeProcess(HANDLE hProcess, LPDWORD lpExitCode);
VOID ExitProcess(UINT uExitCode);

HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize,
                    LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter,
                    DWORD dwCreationFlags, LPDWORD lpThreadId);
BOOL SetProcessInformation(HANDLE hProcess, int ProcessInformationClass,
                           LPVOID ProcessInformation,
                           SIZE_T ProcessInformationSize);

HANDLE OpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
DWORD SuspendThread(HANDLE hThread);
BOOL GetThreadContext(HANDLE hThread, LPCONTEXT lpContext);
