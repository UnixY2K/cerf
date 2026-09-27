#pragma once
#include "windows.h"

typedef struct _FILE_ZERO_DATA_INFORMATION {
	LARGE_INTEGER FileOffset;
	LARGE_INTEGER BeyondFinalZero;
} FILE_ZERO_DATA_INFORMATION;

#define FSCTL_SET_SPARSE    0x000900C4
#define FSCTL_SET_ZERO_DATA 0x000980C8
