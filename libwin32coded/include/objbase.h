#pragma once
#include "windows.h"

struct IStream { ULONG Release(); };
HRESULT CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease,
                              IStream **ppstm);
