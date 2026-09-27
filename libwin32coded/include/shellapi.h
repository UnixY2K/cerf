#pragma once
#include "windows.h"

HANDLE ShellExecuteW(HWND hwnd, LPCWSTR operation, LPCWSTR file,
                     LPCWSTR parameters, LPCWSTR directory, int showCmd);
