#pragma once
#include "winnt.h"

#define _Post_equals_last_error_

_Post_equals_last_error_ DWORD GetLastError(void);
LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(
    LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
