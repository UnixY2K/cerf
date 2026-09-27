#pragma once
#include "minwinbase.h"
#include "wtypesbase.h"

#define OPEN_EXISTING           3
#define OPEN_ALWAYS             4
#define CREATE_ALWAYS           2
#define FILE_BEGIN              0
#define FILE_CURRENT            1
#define FILE_END                2
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#define FILE_FLAG_OVERLAPPED    0x40000000

HANDLE CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile);
HANDLE CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile);
BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
              LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
               LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
BOOL FlushFileBuffers(HANDLE hFile);
DWORD GetFileAttributesW(LPCWSTR lpFileName);
BOOL DeleteFileW(LPCWSTR lpFileName);
BOOL CreateDirectoryW(LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
BOOL MoveFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, DWORD dwFlags);
BOOL SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove,
                      LARGE_INTEGER *lpNewFilePointer, DWORD dwMoveMethod);
BOOL GetFileSizeEx(HANDLE hFile, LARGE_INTEGER *lpFileSize);
