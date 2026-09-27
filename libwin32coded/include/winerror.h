#pragma once
#include "winnt.h"

#define S_OK             ((HRESULT)0L)
#define S_FALSE          ((HRESULT)1L)
#define FAILED(hr)       (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr)    (((HRESULT)(hr)) >= 0)

#define ERROR_IO_PENDING           997L
#define ERROR_CLASS_ALREADY_EXISTS 1410L
