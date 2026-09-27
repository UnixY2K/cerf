#pragma once
#include "minwinbase.h"
#include "wtypesbase.h"

#define FILE_MAP_READ 0x0004

HANDLE CreateFileMappingW(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes,
                          DWORD flProtect, DWORD dwMaximumSizeHigh,
                          DWORD dwMaximumSizeLow, LPCWSTR lpName);
LPVOID MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess,
                     DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow,
                     SIZE_T dwNumberOfBytesToMap);
BOOL UnmapViewOfFile(LPCVOID lpBaseAddress);

LPVOID VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType,
                    DWORD flProtect);
BOOL VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
SIZE_T VirtualQuery(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer,
                    SIZE_T dwLength);
