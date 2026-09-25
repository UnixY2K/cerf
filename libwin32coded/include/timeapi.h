#pragma once
#include "windows.h"

typedef uint32_t MMRESULT;

#define TIME_PERIODIC           0x0001
#define TIME_CALLBACK_FUNCTION  0x0000

MMRESULT timeSetEvent(UINT uDelay, UINT uResolution, LPTIMECALLBACK lpTimeProc,
                      DWORD_PTR dwUser, DWORD fuEvent);
MMRESULT timeKillEvent(UINT uTimerID);
UINT timeBeginPeriod(UINT uPeriod);
UINT timeEndPeriod(UINT uPeriod);
