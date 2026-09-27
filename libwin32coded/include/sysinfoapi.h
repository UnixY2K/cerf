#pragma once
#include "minwinbase.h"

typedef struct _SYSTEM_INFO {
    DWORD dwAllocationGranularity;
} SYSTEM_INFO, *LPSYSTEM_INFO;

VOID GetSystemInfo(SYSTEM_INFO *lpSystemInfo);
DWORD GetTickCount(void);
ULONGLONG GetTickCount64(void);
VOID GetLocalTime(LPSYSTEMTIME lpSystemTime);
