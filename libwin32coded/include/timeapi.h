#pragma once
#include "windef.h"

typedef struct mmtime_tag {
	UINT wType;
	union {
		DWORD ms;
		DWORD sample;
		DWORD cb;
		DWORD ticks;
		struct {
			BYTE hour;
			BYTE min;
			BYTE sec;
			BYTE frame;
			BYTE fps;
			BYTE dummy;
			BYTE pad[2];
		} smpte;
		struct {
			DWORD songptrpos;
		} midi;
	} u;
} MMTIME, *PMMTIME, *LPMMTIME;

typedef uint32_t MMRESULT;

#define TIME_PERIODIC          0x0001
#define TIME_CALLBACK_FUNCTION 0x0000

typedef void(CALLBACK *LPTIMECALLBACK)(UINT, UINT, DWORD_PTR, DWORD_PTR,
                                       DWORD_PTR);

MMRESULT timeSetEvent(UINT uDelay, UINT uResolution, LPTIMECALLBACK lpTimeProc,
                      DWORD_PTR dwUser, DWORD fuEvent);
MMRESULT timeKillEvent(UINT uTimerID);
UINT timeBeginPeriod(UINT uPeriod);
UINT timeEndPeriod(UINT uPeriod);
