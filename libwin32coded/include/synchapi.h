#pragma once
#include "windef.h"
#include "wtypesbase.h"

#define CREATE_WAITABLE_TIMER_MANUAL_RESET    0x00000001
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002
#define TIMER_ALL_ACCESS                      0x001F0003
#define WAIT_OBJECT_0                         0x00000000L
#define INFINITE                              0xFFFFFFFF

typedef VOID (*PTIMERAPCROUTINE)(
    LPVOID lpArgToCompletionRoutine,
    DWORD dwTimerLowValue,
    DWORD dwTimerHighValue
);

BOOL InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

VOID Sleep(DWORD dwMilliseconds);
HANDLE CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset,
                    BOOL bInitialState, LPCWSTR lpName);
BOOL SetEvent(HANDLE hEvent);
BOOL ResetEvent(HANDLE hEvent);

HANDLE CreateWaitableTimerW(LPSECURITY_ATTRIBUTES lpTimerAttributes,
                            BOOL bManualReset, LPCWSTR lpTimerName);
HANDLE CreateWaitableTimerExW(LPSECURITY_ATTRIBUTES lpTimerAttributes,
                              LPCWSTR lpTimerName, DWORD dwFlags,
                              DWORD dwDesiredAccess);
BOOL SetWaitableTimer(HANDLE hTimer, const LARGE_INTEGER *lpDueTime,
                      LONG lPeriod, PTIMERAPCROUTINE pfnCompletionRoutine,
                      LPVOID lpArgToCompletionRoutine, BOOL fResume);
BOOL CancelWaitableTimer(HANDLE hTimer);

DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE *lpHandles,
                             BOOL bWaitAll, DWORD dwMilliseconds);
