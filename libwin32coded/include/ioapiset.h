#pragma once
#include "minwinbase.h"

BOOL DeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode,
                     LPVOID lpInBuffer, DWORD nInBufferSize,
                     LPVOID lpOutBuffer, DWORD nOutBufferSize,
                     LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
BOOL CancelIoEx(HANDLE hFile, LPOVERLAPPED lpOverlapped);
BOOL GetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped,
                         LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
