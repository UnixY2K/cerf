#pragma once

#include "win_definitions.h"
#ifndef __WIN32
#include <cerrno>
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

using SOCKET = int;
using WSAPOLLFD = struct pollfd;
struct WSADATA {};
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#ifndef POLLRDNORM
#define POLLRDNORM POLLIN
#endif
#ifndef POLLWRNORM
#define POLLWRNORM POLLOUT
#endif
#ifndef POLLRDBAND
#define POLLRDBAND POLLPRI
#endif
#define WSAEWOULDBLOCK EWOULDBLOCK
#define MAKEWORD(low, high) 0
inline int WSAStartup(unsigned short, WSADATA*) { return 0; }
inline int WSACleanup() { return 0; }
inline int WSAGetLastError() { return errno; }
inline int WSAPoll(WSAPOLLFD* fds, unsigned long count, int timeout) {
    return poll(fds, count, timeout);
}
inline int closesocket(SOCKET s) { return close(s); }
inline int ioctlsocket(SOCKET s, long request, unsigned long* value) {
    return ioctl(s, request, value);
}
#endif
#ifdef __cplusplus
#include <cstdarg>
#else
#include <stdarg.h>
#endif

// winbase.h

int lstrlenW(LPCWSTR lpString // in
);

LPWSTR lstrcpynW(LPWSTR lpString1,  // out
                 LPCWSTR lpString2, // in
                 int iMaxLength     // in
);

// winuser.h
int MessageBoxW(HWND hWnd,         // in, optional
                LPCWSTR lpText,    // in, optional
                LPCWSTR lpCaption, // in,optional
                UINT uType         // in
);
BOOL AllowSetForegroundWindow(DWORD dwProcessId // in
);
int MessageBoxA(HWND hWnd,        // in, optional
                LPCSTR lpText,    // in, optional
                LPCSTR lpCaption, // in,optional
                UINT uType        // in
);

// processthreadsapi.h

BOOL CreateProcessW(LPCWSTR lpApplicationName, // in, optional
                    LPWSTR lpCommandLine,      // in, out, optional
                    LPSECURITY_ATTRIBUTES lpProcessAttributes, // in, optional
                    LPSECURITY_ATTRIBUTES lpThreadAttributes,  // in, optional
                    BOOL bInheritHandles,                      // in
                    DWORD dwCreationFlags,                     // in
                    LPVOID lpEnvironment,                      // in, optional
                    LPCWSTR lpCurrentDirectory,                // in, optional
                    LPSTARTUPINFOW lpStartupInfo,              // in
                    LPPROCESS_INFORMATION lpProcessInformation // out
);

DWORD GetCurrentProcessId(void);
DWORD GetCurrentThreadId(void);
HANDLE GetCurrentProcess(void);
HANDLE GetCurrentThread(void);
int MulDiv(int nNumber, int nNumerator, int nDenominator);
HWND GetCapture(void);
BOOL ReleaseCapture(void);
HWND SetCapture(HWND hWnd);
BOOL ScreenToClient(HWND hWnd, PPOINT lpPoint);
BOOL ClientToScreen(HWND hWnd, PPOINT lpPoint);
BOOL SetFocus(HWND hWnd);
int SetCursorPos(int X, int Y);
int ShowCursor(BOOL bShow);
BOOL GetCursorPos(PPOINT lpPoint);
HANDLE SetCursor(HANDLE hCursor);
BOOL PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);
HMENU CreateMenu(void);
HMENU CreatePopupMenu(void);
BOOL AppendMenuW(HMENU hMenu, UINT uFlags, UINT_PTR uIdNewItem,
                 LPCWSTR lpNewItem);
HMENU GetSubMenu(HMENU hMenu, int nPos);
int GetMenuItemCount(HMENU hMenu);
BOOL DeleteMenu(HMENU hMenu, UINT uPosition, UINT uFlags);
BOOL GetWindowPlacement(HWND hWnd, PWINDOWPLACEMENT lpwndpl);
LONG GetWindowLongW(HWND hWnd, int nIndex);
LONG SetWindowLongW(HWND hWnd, int nIndex, LONG dwNewLong);
HMENU GetMenu(HWND hWnd);
BOOL SetMenu(HWND hWnd, HMENU hMenu);
HMONITOR MonitorFromWindow(HWND hwnd, DWORD dwFlags);
BOOL GetMonitorInfoW(HMONITOR hMonitor, LPMONITORINFO lpmi);
BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx,
                  int cy, UINT uFlags);
BOOL SetWindowPlacement(HWND hWnd, const WINDOWPLACEMENT *lpwndpl);
ATOM RegisterClassExW(const WNDCLASSEXW *lpwcx);
HCURSOR LoadCursorW(HINSTANCE hInstance, LPCWSTR lpCursorName);
HICON LoadIconW(HINSTANCE hInstance, LPCWSTR lpIconName);
HWND CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
                     DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
                     HWND hWndParent, HMENU hMenu, HINSTANCE hInstance,
                     LPVOID lpParam);
BOOL ShowWindow(HWND hWnd, int nCmdShow);
BOOL IsZoomed(HWND hWnd);
BOOL CheckMenuRadioItem(HMENU hMenu, UINT idFirst, UINT idLast, UINT idCheck,
                        UINT uFlags);
DWORD CheckMenuItem(HMENU hMenu, UINT uIDCheckItem, UINT uCheck);
BOOL DestroyMenu(HMENU hMenu);
BOOL SetForegroundWindow(HWND hWnd);
BOOL GetClientRect(HWND hWnd, LPRECT lpRect);
BOOL UpdateWindow(HWND hWnd);
HWND GetWindow(HWND hWnd, UINT uCmd);
LONG_PTR SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
LONG_PTR GetWindowLongPtrW(HWND hWnd, int nIndex);
LRESULT DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL SetWindowTextW(HWND hWnd, LPCWSTR lpString);
BOOL DestroyWindow(HWND hWnd);
int GetSysColor(int nIndex);
LRESULT SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam);
HHOOK SetWindowsHookExW(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId);
BOOL UnhookWindowsHookEx(HHOOK hhk);
HWND GetForegroundWindow(void);
UINT MapVirtualKeyW(UINT uCode, UINT uMapType);
int GetKeyNameTextW(LONG lParam, LPWSTR lpString, int cchSize);
SHORT GetKeyState(int nVirtKey);
UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, LPVOID lpTimerFunc);
BOOL KillTimer(HWND hWnd, UINT_PTR uIDEvent);
BOOL InitCommonControlsEx(const INITCOMMONCONTROLSEX *lpInitCtrls);
BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint);
BOOL EqualRect(const RECT *lprc1, const RECT *lprc2);
int MapWindowPoints(HWND hWndFrom, HWND hWndTo, PPOINT lpPoints, UINT cPoints);
UINT TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved,
                    HWND hWnd, const RECT *prcRect);
DWORD GetMessagePos(void);
HDC BeginPaint(HWND hWnd, PPAINTSTRUCT lpPaint);
BOOL EndPaint(HWND hWnd, const PAINTSTRUCT *lpPaint);
HDC CreateCompatibleDC(HDC hdc);
HBITMAP CreateCompatibleBitmap(HDC hdc, int cx, int cy);
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ h);
int FillRect(HDC hDC, const RECT *lprc, HBRUSH hbr);
HBRUSH GetSysColorBrush(int nIndex);
BOOL FrameRect(HDC hDC, const RECT *lprc, HBRUSH hbr);
BOOL InflateRect(LPRECT lprc, int dx, int dy);
BOOL MoveToEx(HDC hdc, int x, int y, PPOINT lpPoint);
BOOL LineTo(HDC hdc, int x, int y);
BOOL BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO *pbmi, UINT usage,
                         LPVOID *ppvBits, HANDLE hSection, DWORD offset);
BOOL DeleteObject(HGDIOBJ ho);
BOOL DeleteDC(HDC hdc);
HBRUSH CreateSolidBrush(COLORREF color);
HPEN CreatePen(int iStyle, int cWidth, COLORREF color);
BOOL Rectangle(HDC hdc, int left, int top, int right, int bottom);
BOOL Ellipse(HDC hdc, int left, int top, int right, int bottom);
BYTE GetRValue(COLORREF rgb);
BYTE GetGValue(COLORREF rgb);
BYTE GetBValue(COLORREF rgb);
BOOL AlphaBlend(HDC hdcDest, int xoriginDest, int yoriginDest, int wDest,
                int hDest, HDC hdcSrc, int xoriginSrc, int yoriginSrc,
                int wSrc, int hSrc, BLENDFUNCTION ftn);
int GetObjectW(HGDIOBJ h, int c, LPVOID pv);
HFONT CreateFontIndirectW(const LOGFONTW *lplf);
BOOL PtInRect(const RECT *lprc, POINT pt);
BOOL AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);
BOOL GetWindowRect(HWND hWnd, LPRECT lpRect);
BOOL TextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c);
BOOL GetTextMetricsW(HDC hdc, TEXTMETRICW *lptm);
BOOL TextOutW(HDC hdc, int x, int y, LPCWSTR lpString, int c);
BOOL MoveFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName,
                 DWORD dwFlags);
BOOL TerminateProcess(HANDLE hProcess, UINT uExitCode);
BOOL GetExitCodeProcess(HANDLE hProcess, LPDWORD lpExitCode);
BOOL DeleteFileW(LPCWSTR lpFileName);
BOOL SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove,
                      LARGE_INTEGER *lpNewFilePointer, DWORD dwMoveMethod);
BOOL GetFileSizeEx(HANDLE hFile, LARGE_INTEGER *lpFileSize);
VOID GetSystemInfo(SYSTEM_INFO *lpSystemInfo);
BOOL DeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode,
                     LPVOID lpInBuffer, DWORD nInBufferSize,
                     LPVOID lpOutBuffer, DWORD nOutBufferSize,
                     LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
MMRESULT timeSetEvent(UINT uDelay, UINT uResolution, LPTIMECALLBACK lpTimeProc,
                      DWORD_PTR dwUser, DWORD fuEvent);
MMRESULT timeKillEvent(UINT uTimerID);
MMRESULT waveOutOpen(HWAVEOUT *phwo, UINT uDeviceID,
                     const WAVEFORMATEX *pwfx, DWORD_PTR dwCallback,
                     DWORD_PTR dwInstance, DWORD fdwOpen);
MMRESULT waveOutClose(HWAVEOUT hwo);
MMRESULT waveOutReset(HWAVEOUT hwo);
MMRESULT waveOutPrepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveOutUnprepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveOutWrite(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInOpen(HWAVEIN *phwi, UINT uDeviceID,
                    const WAVEFORMATEX *pwfx, DWORD_PTR dwCallback,
                    DWORD_PTR dwInstance, DWORD fdwOpen);
MMRESULT waveInClose(HWAVEIN hwi);
MMRESULT waveInReset(HWAVEIN hwi);
MMRESULT waveInPrepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInUnprepareHeader(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInAddBuffer(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
MMRESULT waveInStart(HWAVEIN hwi);
DWORD MsgWaitForMultipleObjects(DWORD nCount, const HANDLE *pHandles,
                                BOOL fWaitAll, DWORD dwMilliseconds,
                                DWORD dwWakeMask);
DWORD MsgWaitForMultipleObjectsEx(DWORD nCount, const HANDLE *pHandles,
                                  DWORD dwMilliseconds, DWORD dwWakeMask,
                                  DWORD dwFlags);
BOOL EnableWindow(HWND hWnd, BOOL bEnable);
BOOL IsDialogMessageW(HWND hDlg, const MSG *lpMsg);
BOOL EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam);
BOOL GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin,
                 UINT wMsgFilterMax);
BOOL PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin,
                  UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL TranslateMessage(const MSG *lpMsg);
LRESULT DispatchMessageW(const MSG *lpMsg);
VOID PostQuitMessage(int nExitCode);
HFONT CreateFontW(int cHeight, int cWidth, int cEscapement, int cOrientation,
                  int cWeight, DWORD bItalic, DWORD bUnderline,
                  DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision,
                  DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily,
                  LPCWSTR pszFaceName);
int DrawTextW(HDC hdc, LPCWSTR lpchText, int cchText, LPRECT lprc,
              UINT format);
HDC GetDC(HWND hWnd);
int ReleaseDC(HWND hWnd, HDC hDC);
int GetDeviceCaps(HDC hdc, int index);
BOOL AdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu,
                        DWORD dwExStyle);
BOOL SystemParametersInfoW(UINT uiAction, UINT uiParam, LPVOID pvParam,
                            UINT fWinIni);

typedef DWORD(WINAPI *LPTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);

HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,
                    SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress,
                    LPVOID lpParameter, DWORD dwCreationFlags,
                    LPDWORD lpThreadId);

BOOL InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
VOID LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

VOID Sleep(DWORD dwMilliseconds);
DWORD GetTickCount(void);
ULONGLONG GetTickCount64(void);
BOOL FlushFileBuffers(HANDLE hFile);
BOOL ResetEvent(HANDLE hEvent);
BOOL CancelIoEx(HANDLE hFile, LPOVERLAPPED lpOverlapped);
BOOL GetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped,
                         LPDWORD lpNumberOfBytesTransferred, BOOL bWait);

// fileapi.h

HANDLE GetStdHandle(DWORD nStdHandle);
DWORD GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
VOID GetLocalTime(LPSYSTEMTIME lpSystemTime);
int wsprintfA(LPSTR lpOut, LPCSTR lpFmt, ...);
int wvsprintfA(LPSTR lpOut, LPCSTR lpFmt, va_list arglist);
int lstrlenA(LPCSTR lpString);
LPSTR lstrcpynA(LPSTR lpString1, LPCSTR lpString2, int iMaxLength);

DWORD GetFileAttributesW(LPCWSTR lpFileName // in
);

HANDLE CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile);

HANDLE CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile);

BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
              LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
               LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

HANDLE CreateFileMappingW(HANDLE hFile, LPSECURITY_ATTRIBUTES lpAttributes,
                          DWORD flProtect, DWORD dwMaximumSizeHigh,
                          DWORD dwMaximumSizeLow, LPCWSTR lpName);

LPVOID MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess,
                     DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow,
                     SIZE_T dwNumberOfBytesToMap);

BOOL UnmapViewOfFile(LPCVOID lpBaseAddress);

// memoryapi.h
LPVOID VirtualAlloc(LPVOID lpAddress,       // in, optional
                    SIZE_T dwSize,          // in
                    DWORD flAllocationType, // in
                    DWORD flProtect         // in
);

BOOL VirtualFree(LPVOID lpAddress, // in
                 SIZE_T dwSize,    // in
                 DWORD dwFreeType  // in
);

// stringapiset.h
int MultiByteToWideChar(UINT CodePage,        // in
                        DWORD dwFlags,        // in
                        LPCCH lpMultiByteStr, // in
                        int cbMultiByte,      // in
                        LPWSTR lpWideCharStr, // out, optional
                        int cchWideChar       // in
);

// errhandlingapi.h
_Post_equals_last_error_ DWORD GetLastError();

// profileapi.h
BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency // out
);
BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount // out
);

// syncapi.h
HANDLE
CreateWaitableTimerExW(LPSECURITY_ATTRIBUTES lpTimerAttributes, // in, optional
                       LPCWSTR lpTimerName,                     // in, optional
                       DWORD dwFlags,                           // in
                       DWORD dwDesiredAccess                    // in
);

HANDLE
CreateWaitableTimerW(LPSECURITY_ATTRIBUTES lpTimerAttributes, // in, optional
                     BOOL bManualReset,                       // in
                     LPCWSTR lpTimerName                      // in, optional
);

HANDLE CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, // in, optional
                    BOOL bManualReset,                       // in
                    BOOL bInitialState,                      // in
                    LPCWSTR lpName                           // in, optional
);

BOOL SetWaitableTimer(HANDLE hTimer,                         // in
                      const LARGE_INTEGER *lpDueTime,        // in
                      LONG lPeriod,                          // in
                      PTIMERAPCROUTINE pfnCompletionRoutine, // in, optional
                      LPVOID lpArgToCompletionRoutine,       // in, optional
                      BOOL fResume                           // in
);

BOOL CancelWaitableTimer(HANDLE hTimer // in
);

BOOL SetEvent(HANDLE hEvent // in
);

BOOL SetProcessInformation(HANDLE hProcess, int ProcessInformationClass,
                           LPVOID ProcessInformation,
                           SIZE_T ProcessInformationSize);
UINT timeBeginPeriod(UINT uPeriod);
UINT timeEndPeriod(UINT uPeriod);

DWORD WaitForSingleObject(HANDLE hHandle,      // in
                          DWORD dwMilliseconds // in
);

DWORD WaitForMultipleObjects(DWORD nCount,            // in
                             const HANDLE *lpHandles, // in
                             BOOL bWaitAll,           // in
                             DWORD dwMilliseconds     // in
);

// libloaderapi.h
HMODULE GetModuleHandleW(LPCWSTR lpModuleName);
HRSRC FindResourceW(HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType);
HGLOBAL LoadResource(HMODULE hModule, HRSRC hResInfo);
LPVOID LockResource(HGLOBAL hResData);
DWORD SizeofResource(HMODULE hModule, HRSRC hResInfo);
HANDLE ShellExecuteW(HWND hwnd, LPCWSTR operation, LPCWSTR file,
                     LPCWSTR parameters, LPCWSTR directory, int showCmd);
BOOL GetOpenFileNameW(OPENFILENAMEW *ofn);
BOOL GetSaveFileNameW(OPENFILENAMEW *ofn);
HMODULE GetModuleHandleA(LPCSTR lpModuleName);
HANDLE LoadImageW(HINSTANCE hInst, LPCWSTR name, UINT type, int cx, int cy,
                  UINT fuLoad);
BOOL DestroyIcon(HICON hIcon);
HANDLE AddFontMemResourceEx(LPVOID pbFont, DWORD cbFont, LPVOID pdv,
                            LPDWORD pcFonts);
BOOL RemoveFontMemResourceEx(HANDLE hFont);
HCURSOR CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth,
                     int nHeight, const void *pvANDPlane,
                     const void *pvXORPlane);
BOOL DestroyCursor(HCURSOR hCursor);
BOOL DrawIconEx(HDC hdc, int xLeft, int yTop, HICON hIcon, int cxWidth,
                int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw,
                UINT diFlags);
FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
HMODULE LoadLibraryW(LPCWSTR lpLibFileName);
BOOL FreeLibrary(HMODULE hModule);
DWORD GetModuleFileNameW(HMODULE hModule,   // in, optional
                         LPWSTR lpFilename, // out
                         DWORD nSize        // in
);

HANDLE GetProcessHeap(void);
LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);

// handleapi.h
BOOL CloseHandle(HANDLE hObject // in
);

SIZE_T VirtualQuery(LPCVOID lpAddress,
                    PMEMORY_BASIC_INFORMATION lpBuffer,
                    SIZE_T dwLength);
BOOL GetProcessMemoryInfo(HANDLE Process,
                          PPROCESS_MEMORY_COUNTERS ppsmemCounters,
                          DWORD cb);

// wingdi.h / winuser.h
HDC GetDC(HWND hWnd);
int ReleaseDC(HWND hWnd, HDC hDC);
HDC CreateCompatibleDC(HDC hdc);
BOOL DeleteDC(HDC hdc);
HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO *pbmi, UINT usage,
                         void **ppvBits, HANDLE hSection, DWORD offset);
HBITMAP CreateCompatibleBitmap(HDC hdc, int cx, int cy);
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ h);
BOOL DeleteObject(HGDIOBJ ho);
HPEN CreatePen(int fnPenStyle, int nWidth, COLORREF crColor);
HBRUSH CreateSolidBrush(COLORREF color);
HGDIOBJ GetStockObject(int i);
int FillRect(HDC hDC, const RECT *lprc, HBRUSH hbr);
BOOL Rectangle(HDC hdc, int left, int top, int right, int bottom);
BOOL MoveToEx(HDC hdc, int x, int y, POINT *lpPoint);
BOOL LineTo(HDC hdc, int x, int y);
BOOL InvalidateRect(HWND hWnd, const RECT *lpRect, BOOL bErase);
int SetStretchBltMode(HDC hdc, int mode);
BOOL SetBrushOrgEx(HDC hdc, int x, int y, POINT *old);
BOOL StretchBlt(HDC hdc, int x, int y, int cx, int cy, HDC src,
               int sx, int sy, int scx, int scy, DWORD rop);
BOOL BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC src,
            int sx, int sy, DWORD rop);
int SetBkMode(HDC hdc, int mode);
COLORREF SetTextColor(HDC hdc, COLORREF color);
COLORREF SetBkColor(HDC hdc, COLORREF color);
BOOL GdiFlush(void);
int ShowScrollBar(HWND hWnd, int wBar, BOOL bShow);
int SetScrollInfo(HWND hwnd, int nBar, LPSCROLLINFO lpsi, BOOL redraw);
BOOL GetScrollInfo(HWND hwnd, int nBar, LPSCROLLINFO lpsi);

// tlhelp32.h
#define TH32CS_SNAPTHREAD        0x00000004
#define THREAD_SUSPEND_RESUME    0x0002
#define THREAD_GET_CONTEXT       0x0008
#define THREAD_QUERY_INFORMATION 0x0040
#define THREAD_QUERY_LIMITED_INFORMATION 0x0800

HANDLE CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
BOOL Thread32First(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
BOOL Thread32Next(HANDLE hSnapshot, LPTHREADENTRY32 lpte);
HANDLE OpenThread(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwThreadId);
DWORD SuspendThread(HANDLE hThread);
BOOL GetThreadContext(HANDLE hThread, LPCONTEXT lpContext);
BOOL IsBadReadPtr(const void *lp, SIZE_T ucb);

// dbghelp.h
BOOL SymInitialize(HANDLE hProcess, LPCSTR UserSearchPath, BOOL fInvadeProcess);
BOOL SymFromAddr(HANDLE hProcess, DWORD64 Address, DWORD64 *Displacement,
                 PSYMBOL_INFO Symbol);
BOOL SymCleanup(HANDLE hProcess);
LPVOID SymFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase);
DWORD64 SymGetModuleBase64(HANDLE hProcess, DWORD64 AddrBase);

typedef BOOL(WINAPI *PREAD_PROCESS_MEMORY_ROUTINE64)(HANDLE, DWORD64, LPVOID,
                                                     DWORD, LPDWORD);
typedef LPVOID(WINAPI *PFUNCTION_TABLE_ACCESS_ROUTINE64)(HANDLE, DWORD64);
typedef DWORD64(WINAPI *PGET_MODULE_BASE_ROUTINE64)(HANDLE, DWORD64);

BOOL StackWalk64(DWORD MachineType, HANDLE hProcess, HANDLE hThread,
                 LPSTACKFRAME64 StackFrame, LPVOID ContextRecord,
                 PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
                 PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
                 PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
                 LPVOID TranslateAddress);
USHORT CaptureStackBackTrace(DWORD FramesToSkip, DWORD FramesToCapture,
                             LPVOID *BackTrace, DWORD *BackTraceHash);

// winbase.h
VOID ExitProcess(UINT uExitCode);

// errhandlingapi.h
LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(
    LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);

// winnt.h

VOID YieldProcessor();