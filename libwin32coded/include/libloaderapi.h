#pragma once
#include "windef.h"

#define LOAD_LIBRARY_SEARCH_SYSTEM32 0x00000800

HMODULE LoadLibraryW(LPCWSTR lpLibFileName);
HMODULE LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
BOOL FreeLibrary(HMODULE hModule);
FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

HMODULE GetModuleHandleW(LPCWSTR lpModuleName);
HMODULE GetModuleHandleA(LPCSTR lpModuleName);
DWORD GetModuleFileNameW(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
DWORD GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);

HRSRC FindResourceW(HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType);
HGLOBAL LoadResource(HMODULE hModule, HRSRC hResInfo);
LPVOID LockResource(HGLOBAL hResData);
DWORD SizeofResource(HMODULE hModule, HRSRC hResInfo);
