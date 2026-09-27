#pragma once
#include "windef.h"

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

BOOL CloseHandle(HANDLE hObject);
