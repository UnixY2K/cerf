#pragma once
#ifdef __cplusplus
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <strings.h>
#else
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <strings.h>
#endif

#include "windef.h"
#include "minwinbase.h"
#include "synchapi.h"
#include "fileapi.h"
#include "memoryapi.h"
#include "processthreadsapi.h"
#include "handleapi.h"
#include "errhandlingapi.h"
#include "sysinfoapi.h"
#include "profileapi.h"
#include "libloaderapi.h"
#include "heapapi.h"
#include "ioapiset.h"
#include "processenv.h"
#include "stringapiset.h"

#define GMEM_MOVEABLE 0x0002

LPVOID GlobalAlloc(UINT uFlags, SIZE_T dwBytes);
LPVOID GlobalLock(HGLOBAL hMem);
BOOL GlobalUnlock(HGLOBAL hMem);
HGLOBAL GlobalFree(HGLOBAL hMem);

#define MOVEFILE_REPLACE_EXISTING 0x00000001
#define MOVEFILE_WRITE_THROUGH    0x00000008

#define ZeroMemory(ptr, size) std::memset((ptr), 0, (size))
int MulDiv(int nNumber, int nNumerator, int nDenominator);
BOOL IsBadReadPtr(const void *lp, SIZE_T ucb);

int lstrlenW(LPCWSTR lpString);
LPWSTR lstrcpynW(LPWSTR lpString1, LPCWSTR lpString2, int iMaxLength);
int lstrlenA(LPCSTR lpString);
LPSTR lstrcpynA(LPSTR lpString1, LPCSTR lpString2, int iMaxLength);
int lstrcmpiW(LPCWSTR string1, LPCWSTR string2);

#define _stricmp strcasecmp

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

#ifndef __WIN32
#define swprintf_s(buffer, format, ...) \
    swprintf((buffer), sizeof(buffer) / sizeof(*(buffer)), (format), __VA_ARGS__)
#endif

#ifdef __cplusplus
inline int wcscpy_s(wchar_t *destination, size_t destination_size,
                    const wchar_t *source) {
    if (!destination || destination_size == 0 || !source) return 22;
    const size_t length = std::wcslen(source);
    if (length >= destination_size) {
        destination[0] = L'\0';
        return 34;
    }
    std::wmemcpy(destination, source, length + 1);
    return 0;
}
inline int wcscpy_s(wchar_t *destination, const wchar_t *source) {
    if (!destination || !source) return 22;
    std::wcscpy(destination, source);
    return 0;
}
#define _snwprintf_s(buffer, truncate, format, ...) \
    swprintf((buffer), sizeof(buffer) / sizeof(*(buffer)), (format), __VA_ARGS__)
#define _snprintf_s(buffer, truncate, format, ...) \
    snprintf((buffer), sizeof(buffer) / sizeof(*(buffer)), (format), __VA_ARGS__)
#endif
