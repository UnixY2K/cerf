#pragma once
#include "windows.h"

typedef struct tagOPENFILENAMEW {
	DWORD lStructSize;
	HWND hwndOwner;
	HINSTANCE hInstance;
	LPCWSTR lpstrFilter;
	LPWSTR lpstrCustomFilter;
	DWORD nMaxCustFilter;
	DWORD nFilterIndex;
	LPWSTR lpstrFile;
	DWORD nMaxFile;
	LPWSTR lpstrFileTitle;
	DWORD nMaxFileTitle;
	LPCWSTR lpstrInitialDir;
	LPCWSTR lpstrTitle;
	DWORD Flags;
	WORD nFileOffset;
	WORD nFileExtension;
	LPCWSTR lpstrDefExt;
	LPARAM lCustData;
	LPVOID lpfnHook;
	LPCWSTR lpTemplateName;
} OPENFILENAMEW;

#define OFN_OVERWRITEPROMPT    0x00000002
#define OFN_HIDEREADONLY       0x00000004
#define OFN_NOCHANGEDIR        0x00000008
#define OFN_PATHMUSTEXIST      0x00000800
#define OFN_FILEMUSTEXIST      0x00001000

BOOL GetOpenFileNameW(OPENFILENAMEW *ofn);
BOOL GetSaveFileNameW(OPENFILENAMEW *ofn);
