#pragma once
#include "windows.h"

#define GET_X_LPARAM(lp)           ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)           ((int)(short)HIWORD(lp))
#define GET_WHEEL_DELTA_WPARAM(wp) ((SHORT)HIWORD(wp))
