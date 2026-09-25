#pragma once
#include "windows.h"

HRESULT DwmSetWindowAttribute(HWND hwnd, DWORD attribute, const void *value,
                              DWORD size);
