#pragma once
#include "windef.h"

#define STD_INPUT_HANDLE  ((DWORD)-10)
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#define STD_ERROR_HANDLE  ((DWORD)-12)

HANDLE GetStdHandle(DWORD nStdHandle);
