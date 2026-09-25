#pragma once
#include "windows.h"
#include "timeapi.h"

typedef HANDLE HWAVEOUT;
typedef HANDLE HWAVEIN;
typedef struct tagWAVEFORMATEX {
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	WORD cbSize;
} WAVEFORMATEX;
typedef struct tagWAVEHDR {
	LPSTR lpData;
	DWORD dwBufferLength;
	DWORD dwBytesRecorded;
	DWORD_PTR dwUser;
	DWORD dwFlags;
	DWORD dwLoops;
	LPVOID lpNext;
	DWORD_PTR reserved;
} WAVEHDR, *LPWAVEHDR;

#define WHDR_DONE               0x00000001
#define WHDR_PREPARED           0x00000002
#define MM_WOM_DONE             0x03BD
#define WAVE_FORMAT_PCM         1
#define WAVE_FORMAT_DIRECT      0x0008
#define WAVE_MAPPER              0xFFFFFFFFu
#define MMSYSERR_NOERROR        0
#define CALLBACK_THREAD         0x00020000

MMRESULT waveOutOpen(HWAVEOUT *phwo, UINT uDeviceID,
                     const WAVEFORMATEX *pwfx, DWORD_PTR dwCallback,
                     DWORD_PTR dwInstance, DWORD fdwOpen);
MMRESULT waveOutClose(HWAVEOUT hwo);
MMRESULT waveOutReset(HWAVEOUT hwo);
MMRESULT waveOutPrepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveOutUnprepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveOutWrite(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInOpen(HWAVEIN *phwi, UINT uDeviceID,
                    const WAVEFORMATEX *pwfx, DWORD_PTR dwCallback,
                    DWORD_PTR dwInstance, DWORD fdwOpen);
MMRESULT waveInClose(HWAVEIN hwi);
MMRESULT waveInReset(HWAVEIN hwi);
MMRESULT waveInPrepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInUnprepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInAddBuffer(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInStart(HWAVEIN hwi);
