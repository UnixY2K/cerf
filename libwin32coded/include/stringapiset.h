#pragma once
#include "windef.h"

#define _In_NLS_string_(size)

int MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr,
                        int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar);
int WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr,
                        int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
                        LPCSTR lpDefaultChar, BOOL *lpUsedDefaultChar);
