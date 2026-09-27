#pragma once
// most of these definitions come from
// Windows data types
// https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
// Memory protection constants
// https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
#ifdef __cplusplus
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cwchar>
#else
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#endif

// windows type annotations (SAL/source annotation language)
#define __nullterminated
#define _Post_equals_last_error_
#define _In_NLS_string_(size)
#ifndef WINAPI
#define WINAPI
#endif
#ifndef CALLBACK
#define CALLBACK
#endif

#ifndef __int32
#define __int32 int
#endif

// windows data types

typedef void VOID;
typedef void *LPVOID;
typedef void *PVOID;
typedef PVOID HANDLE;
typedef const void *LPCVOID;
typedef unsigned char BYTE;

typedef uintptr_t UINT_PTR;
typedef intptr_t INT_PTR;
typedef uint64_t ULONGLONG;

// designed to perform pointer arithmetic
typedef intptr_t LONG_PTR;
typedef uintptr_t ULONG_PTR;

typedef ULONG_PTR SIZE_T;
typedef ULONG_PTR DWORD_PTR;

// typedef unsigned int UINT;
typedef uint32_t UINT;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;

// typedef unsigned long DWORD;
typedef uint32_t DWORD;
typedef DWORD *LPDWORD;
typedef uint64_t DWORD64;
// typedef unsigned short WORD;
typedef uint16_t WORD;
typedef WORD *LPWORD;
typedef uint16_t USHORT;
typedef WORD ATOM;
typedef int16_t SHORT;

// typedef long LONG;
typedef int32_t LONG;
// typedef unsigned long ULONG;
typedef uint32_t ULONG;
// typedef long long LONGLONG;
typedef int64_t LONGLONG;

typedef DWORD COLORREF;

// HANDLE to Device Context
typedef HANDLE HDC;
typedef HANDLE HINSTANCE;
typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HMENU;
typedef HANDLE HBRUSH;
typedef HANDLE HBITMAP;
typedef HANDLE HFONT;
typedef HANDLE HPEN;
typedef HANDLE HGDIOBJ;
typedef HANDLE HHOOK;
typedef HANDLE HLOCAL;
typedef HANDLE HGLOBAL;
typedef HANDLE HRSRC;
typedef void *FARPROC;

// defined as is
typedef int BOOL;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define STD_INPUT_HANDLE  ((DWORD)-10)
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#define STD_ERROR_HANDLE  ((DWORD)-12)

#define INFINITE 0xFFFFFFFF

typedef char CHAR;
typedef wchar_t WCHAR;

#define CONST const
typedef __nullterminated CONST CHAR *LPCSTR;
typedef __nullterminated CONST CHAR *LPCCH;
typedef __nullterminated CONST WCHAR *LPCWSTR;
typedef __nullterminated WCHAR *LPWSTR;
typedef CHAR *LPSTR;
typedef struct _OVERLAPPED *LPOVERLAPPED;

typedef struct _OVERLAPPED {
	ULONG_PTR Internal;
	ULONG_PTR InternalHigh;
	union {
		struct { DWORD Offset; DWORD OffsetHigh; };
		LPVOID Pointer;
	};
	HANDLE hEvent;
} OVERLAPPED;

typedef HANDLE HWND;
typedef HANDLE HMODULE;
typedef HANDLE HMONITOR;

// structs defined in windef.h
typedef struct tagRECT {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

typedef struct tagPOINT {
	LONG x;
	LONG y;
} POINT, *PPOINT;

typedef struct tagWINDOWPLACEMENT {
	UINT length;
	UINT flags;
	UINT showCmd;
	POINT ptMinPosition;
	POINT ptMaxPosition;
	RECT rcNormalPosition;
} WINDOWPLACEMENT, *PWINDOWPLACEMENT;

typedef struct tagDRAWITEMSTRUCT DRAWITEMSTRUCT;
typedef struct tagDRAWITEMSTRUCT {
	UINT CtlType;
	UINT CtlID;
	UINT itemID;
	UINT itemAction;
	UINT itemState;
	HWND hwndItem;
	HDC hDC;
	RECT rcItem;
	ULONG_PTR itemData;
} DRAWITEMSTRUCT;
typedef struct tagNONCLIENTMETRICSW NONCLIENTMETRICSW;

typedef struct tagLOGFONTW {
	LONG lfHeight;
	LONG lfWidth;
	LONG lfEscapement;
	LONG lfOrientation;
	LONG lfWeight;
	BYTE lfItalic;
	BYTE lfUnderline;
	BYTE lfStrikeOut;
	BYTE lfCharSet;
	BYTE lfOutPrecision;
	BYTE lfClipPrecision;
	BYTE lfQuality;
	BYTE lfPitchAndFamily;
	WCHAR lfFaceName[32];
} LOGFONTW, *PLOGFONTW;

typedef struct tagNONCLIENTMETRICSW {
	UINT cbSize;
	int iBorderWidth;
	int iScrollWidth;
	int iScrollHeight;
	int iCaptionWidth;
	int iCaptionHeight;
	LOGFONTW lfCaptionFont;
	int iSmCaptionWidth;
	int iSmCaptionHeight;
	LOGFONTW lfSmCaptionFont;
	int iMenuWidth;
	int iMenuHeight;
	LOGFONTW lfMenuFont;
	LOGFONTW lfStatusFont;
	LOGFONTW lfMessageFont;
} NONCLIENTMETRICSW;

typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (CALLBACK *WNDENUMPROC)(HWND, LPARAM);
typedef LRESULT (CALLBACK *HOOKPROC)(int, WPARAM, LPARAM);

typedef struct tagKBDLLHOOKSTRUCT {
	DWORD vkCode;
	DWORD scanCode;
	DWORD flags;
	DWORD time;
	ULONG_PTR dwExtraInfo;
} KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

typedef struct tagWNDCLASSEXW {
	UINT cbSize;
	UINT style;
	WNDPROC lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCWSTR lpszMenuName;
	LPCWSTR lpszClassName;
	HICON hIconSm;
} WNDCLASSEXW, *PWNDCLASSEXW;

typedef struct tagMSG {
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
} MSG, *LPMSG;

typedef struct tagCREATESTRUCTW {
	LPVOID lpCreateParams;
	HINSTANCE hInstance;
	HMENU hMenu;
	HWND hwndParent;
	int cy;
	int cx;
	int y;
	int x;
	LONG style;
	LPCWSTR lpszName;
	LPCWSTR lpszClass;
	DWORD dwExStyle;
} CREATESTRUCTW, *LPCREATESTRUCTW;

typedef struct tagMINMAXINFO {
	POINT ptReserved;
	POINT ptMaxSize;
	POINT ptMaxPosition;
	POINT ptMinTrackSize;
	POINT ptMaxTrackSize;
} MINMAXINFO, *LPMINMAXINFO;

typedef struct tagTTTOOLINFOW {
	UINT cbSize;
	UINT uFlags;
	HWND hwnd;
	UINT_PTR uId;
	RECT rect;
	HINSTANCE hinst;
	LPCWSTR lpszText;
	LPARAM lParam;
} TTTOOLINFOW, *LPTTTOOLINFOW;

typedef struct tagNMLINK {
	struct {
		wchar_t szUrl[2048];
	} item;
} NMLINK;

typedef struct tagOPENFILENAMEW {
	DWORD lStructSize;
	HWND hwndOwner;
	HINSTANCE hInstance;
	LPCWSTR lpstrFilter;
	LPWSTR lpstrCustomFilter;
	DWORD nMaxCustFilter;
	DWORD nFilterIndex;
	LPWSTR lpstrFile;
	DWORD nMaxFile;
	LPWSTR lpstrFileTitle;
	DWORD nMaxFileTitle;
	LPCWSTR lpstrInitialDir;
	LPCWSTR lpstrTitle;
	DWORD Flags;
	WORD nFileOffset;
	WORD nFileExtension;
	LPCWSTR lpstrDefExt;
	LPARAM lCustData;
	LPVOID lpfnHook;
	LPCWSTR lpTemplateName;
} OPENFILENAMEW;

typedef struct tagINITCOMMONCONTROLSEX {
	DWORD dwSize;
	DWORD dwICC;
} INITCOMMONCONTROLSEX;

typedef struct tagPAINTSTRUCT {
	HDC hdc;
	BOOL fErase;
	RECT rcPaint;
	BOOL fRestore;
	BOOL fIncUpdate;
	BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT;

typedef uint32_t MMRESULT;
typedef HANDLE HWAVEOUT;
typedef HANDLE HWAVEIN;
typedef struct tagWAVEFORMATEX {
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	WORD cbSize;
} WAVEFORMATEX;
typedef struct tagWAVEHDR {
	LPSTR lpData;
	DWORD dwBufferLength;
	DWORD dwBytesRecorded;
	DWORD_PTR dwUser;
	DWORD dwFlags;
	DWORD dwLoops;
	LPVOID lpNext;
	DWORD_PTR reserved;
} WAVEHDR, *LPWAVEHDR;
typedef void (CALLBACK *LPTIMECALLBACK)(UINT, UINT, DWORD_PTR, DWORD_PTR,
                                         DWORD_PTR);

typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME;

typedef struct tagMONITORINFO {
	DWORD cbSize;
	RECT rcMonitor;
	RECT rcWork;
	DWORD dwFlags;
} MONITORINFO, *LPMONITORINFO;

typedef struct _SYSTEM_INFO {
	DWORD dwAllocationGranularity;
} SYSTEM_INFO;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD bmiColors[1];
} BITMAPINFO;

typedef struct tagBLENDFUNCTION {
	BYTE BlendOp;
	BYTE BlendFlags;
	BYTE SourceConstantAlpha;
	BYTE AlphaFormat;
} BLENDFUNCTION;

typedef struct tagTEXTMETRICW {
	LONG tmHeight;
	LONG tmAscent;
	LONG tmDescent;
	LONG tmInternalLeading;
	LONG tmExternalLeading;
	LONG tmAveCharWidth;
	LONG tmMaxCharWidth;
	LONG tmWeight;
	LONG tmOverhang;
	LONG tmDigitizedAspectX;
	LONG tmDigitizedAspectY;
	WCHAR tmFirstChar;
	WCHAR tmLastChar;
	WCHAR tmDefaultChar;
	WCHAR tmBreakChar;
	BYTE tmItalic;
	BYTE tmUnderlined;
	BYTE tmStruckOut;
	BYTE tmPitchAndFamily;
	BYTE tmCharSet;
} TEXTMETRICW;

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR) - 1)

#define FILE_ATTRIBUTE_READONLY              0x00000001
#define FILE_ATTRIBUTE_HIDDEN                0x00000002
#define FILE_ATTRIBUTE_SYSTEM                0x00000004
#define FILE_ATTRIBUTE_DIRECTORY             0x00000010
#define FILE_ATTRIBUTE_ARCHIVE               0x00000020
#define FILE_ATTRIBUTE_DEVICE                0x00000040
#define FILE_ATTRIBUTE_NORMAL                0x00000080
#define FILE_MAP_READ                         0x0004
#define OPEN_EXISTING                        3
#define OPEN_ALWAYS                          4
#define FILE_BEGIN                           0
#define FILE_CURRENT                         1
#define FILE_END                             2
#define FILE_ATTRIBUTE_TEMPORARY             0x00000100
#define FILE_ATTRIBUTE_SPARSE_FILE           0x00000200
#define FILE_ATTRIBUTE_REPARSE_POINT         0x00000400
#define FILE_ATTRIBUTE_COMPRESSED            0x00000800
#define FILE_ATTRIBUTE_OFFLINE               0x00001000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED   0x00020000
#define FILE_ATTRIBUTE_ENCRYPTED             0x00004000
#define FILE_ATTRIBUTE_INTEGRITY_STREAM      0x00008000
#define FILE_ATTRIBUTE_VIRTUAL               0x00010000
#define FILE_ATTRIBUTE_NO_SCRUB_DATA         0x00020000
#define FILE_ATTRIBUTE_EA                    0x00040000
#define FILE_ATTRIBUTE_PINNED                0x00080000
#define FILE_ATTRIBUTE_UNPINNED              0x00100000
#define FILE_ATTRIBUTE_RECALL_ON_OPEN        0x00040000
#define FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS 0x00400000

#define INVALID_FILE_ATTRIBUTES ((DWORD) - 1)

#define GENERIC_WRITE     0x40000000
#define FILE_SHARE_READ   0x00000001
#define FILE_SHARE_WRITE  0x00000002
#define CREATE_ALWAYS     2
#define OPEN_EXISTING     3
#define GENERIC_READ      0x80000000
#define FILE_FLAG_OVERLAPPED 0x40000000
#define ERROR_IO_PENDING   997
#define BI_RGB             0
#define DIB_RGB_COLORS     0
#define HALFTONE           4
#define COLORONCOLOR       3
#define SRCCOPY            0x00CC0020
#define PS_SOLID           0
#define TRANSPARENT        1
#define DI_NORMAL          0x0003
#define RGB(r, g, b)       ((COLORREF)(((BYTE)(r) | ((WORD)(g) << 8)) | ((DWORD)(BYTE)(b) << 16)))
#ifndef __WIN32
#define swprintf_s(buffer, format, ...) \
	swprintf((buffer), sizeof(buffer) / sizeof(*(buffer)), (format), __VA_ARGS__)
#endif
#define IMAGE_ICON        1
#define LR_DEFAULTCOLOR   0x00000000
#define USER_DEFAULT_SCREEN_DPI 96
#define WM_APP                  0x8000
#define WM_MOUSEMOVE            0x0200
#define WM_LBUTTONDOWN          0x0201
#define WM_LBUTTONUP            0x0202
#define WM_RBUTTONDOWN          0x0204
#define WM_RBUTTONUP            0x0205
#define WM_MBUTTONDOWN          0x0207
#define WM_MBUTTONUP            0x0208
#define WM_MOUSEWHEEL           0x020A
#define WM_CAPTURECHANGED       0x0215
#define WM_SETFONT              0x0030
#define WM_KEYDOWN              0x0100
#define WM_KEYUP                0x0101
#define WM_SYSKEYDOWN           0x0104
#define WM_SYSKEYUP             0x0105
#define WM_NCCREATE             0x0081
#define WM_SIZE                 0x0005
#define WM_HSCROLL              0x0114
#define WM_VSCROLL              0x0115
#define WM_GETMINMAXINFO        0x0024
#define WM_INITMENUPOPUP        0x0117
#define WM_COMMAND              0x0111
#define WM_CLOSE                0x0010
#define WM_DESTROY              0x0002
#define WM_QUIT                 0x0012
#define DEFAULT_GUI_FONT        17
#define COLOR_BTNTEXT           18
#define COLOR_BTNFACE           15
#define DT_WORDBREAK            0x0010
#define DT_CALCRECT             0x0400
#define DT_CENTER               0x0001
#define DT_TOP                  0x0000
#define DT_SINGLELINE           0x0020
#define DT_VCENTER              0x0004
#define DT_NOPREFIX             0x0800
#define DT_END_ELLIPSIS         0x8000
#define DT_BOTTOM               0x0008
#define DT_LEFT                 0x0000
#define FW_BOLD                 700
#define FW_NORMAL               400
#define DEFAULT_CHARSET         1
#define OUT_DEFAULT_PRECIS     0
#define CLIP_DEFAULT_PRECIS    0
#define CLEARTYPE_QUALITY       5
#define VARIABLE_PITCH          2
#define FIXED_PITCH             1
#define FF_SWISS                32
#define FF_MODERN               48
#define WS_CHILD                0x40000000
#define WS_VISIBLE              0x10000000
#define WS_CLIPSIBLINGS         0x04000000
#define WS_POPUP                0x80000000
#define WS_EX_TOPMOST           0x00000008
#define WS_EX_TOOLWINDOW        0x00000080
#define WS_EX_DLGMODALFRAME     0x00000001
#define WS_CAPTION              0x00C00000
#define WS_SYSMENU              0x00080000
#define WS_DLGFRAME             0x00400000
#define WM_PAINT                0x000F
#define WM_ERASEBKGND           0x0014
#define CB_GETCURSEL            0x0147
#define CB_ERR                  (-1)
#define CB_ADDSTRING            0x0143
#define CB_SETCURSEL            0x014E
#define BM_GETCHECK             0x00F0
#define BM_SETCHECK             0x00F1
#define BST_UNCHECKED           0
#define BST_CHECKED             1
#define SS_LEFT                 0x00000000
#define CBS_DROPDOWNLIST        0x0003
#define WS_VSCROLL              0x00200000
#define BS_AUTOCHECKBOX         0x00000003
#define CBN_DROPDOWN            7
#define CBN_SELCHANGE           1
#define BN_CLICKED              0
#define GRAY_BRUSH              2
#define WM_DRAWITEM             0x002B
#define WM_CTLCOLORDLG          0x0136
#define WM_CTLCOLORSTATIC       0x0138
#define WM_CTLCOLOREDIT         0x0133
#define WM_CTLCOLORBTN          0x0135
#define WM_CTLCOLORLISTBOX      0x0134
#define QS_ALLINPUT             0x04FF
#define PM_REMOVE               0x0001
#define PM_NOREMOVE             0x0000
#define MWMO_INPUTAVAILABLE     0x0004
#define GET_X_LPARAM(lp)        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)        ((int)(short)HIWORD(lp))
#define WS_TABSTOP              0x00010000
#define WS_GROUP                0x00020000
#define BS_PUSHBUTTON           0x00000000
#define BS_DEFPUSHBUTTON        0x00000001
#define IDCANCEL                2
#define GW_CHILD                5
#define GW_HWNDNEXT             2
#define MF_STRING               0x0000
#define MF_POPUP                0x0010
#define MF_SEPARATOR            0x0800
#define MF_GRAYED               0x0001
#define HIWORD(value)           ((WORD)((((ULONG_PTR)(value)) >> 16) & 0xffff))
#define LOWORD(value)           ((WORD)(((ULONG_PTR)(value)) & 0xffff))
#define GWL_STYLE              (-16)
#define GWL_EXSTYLE            (-20)
#define GWLP_USERDATA          (-21)
#define WS_OVERLAPPEDWINDOW    0x00CF0000
#define WS_CLIPCHILDREN        0x02000000
#define CS_VREDRAW             0x0001
#define CS_HREDRAW             0x0002
#define CW_USEDEFAULT          ((int)0x80000000)
#define SW_RESTORE             9
#define SW_SHOW                5
#define SW_SHOWNORMAL          1
#define SW_HIDE                0
#define SIZE_MINIMIZED         1
#define WM_ENTERSIZEMOVE       0x0231
#define WM_EXITSIZEMOVE        0x0232
#define WM_SYSCOMMAND          0x0112
#define WM_ACTIVATE            0x0006
#define SC_MAXIMIZE            0xF030
#define WA_INACTIVE            0
#define AC_SRC_OVER            0x00
#define DIB_RGB_COLORS         0
#define NULL_BRUSH             5
#define MOVEFILE_REPLACE_EXISTING 0x00000001
#define MOVEFILE_WRITE_THROUGH    0x00000008
#define BLACK_BRUSH            4
#define ERROR_CLASS_ALREADY_EXISTS 1410
#define MF_BYCOMMAND           0x0000
#define MF_BYPOSITION          0x0400
#define MF_CHECKED             0x0008
#define MF_UNCHECKED           0x0000
#define MK_LBUTTON             0x0001
#define MK_RBUTTON             0x0002
#define MK_MBUTTON             0x0010
#define WM_SETCURSOR           0x0020
#define WM_TIMER               0x0113
#define HTCLIENT               1
#define IDC_HAND               ((LPCWSTR)(uintptr_t)32649)
#define HC_ACTION               0
#define WH_KEYBOARD_LL          13
#define MAPVK_VK_TO_VSC         0
#define GET_WHEEL_DELTA_WPARAM(wp) ((SHORT)HIWORD(wp))
#define VK_SHIFT                0x10
#define VK_CONTROL              0x11
#define VK_MENU                 0x12
#define VK_ESCAPE               0x1B
#define VK_SPACE                0x20
#define VK_PRIOR                0x21
#define VK_NEXT                 0x22
#define VK_END                  0x23
#define VK_HOME                 0x24
#define VK_LEFT                 0x25
#define VK_UP                   0x26
#define VK_RIGHT                0x27
#define VK_DOWN                 0x28
#define VK_INSERT               0x2D
#define VK_DELETE               0x2E
#define VK_LWIN                 0x5B
#define VK_RWIN                 0x5C
#define VK_APPS                 0x5D
#define VK_CANCEL               0x03
#define VK_SNAPSHOT             0x2C
#define VK_DIVIDE               0x6F
#define VK_LSHIFT               0xA0
#define VK_RSHIFT               0xA1
#define VK_LCONTROL             0xA2
#define VK_RCONTROL             0xA3
#define VK_LMENU                0xA4
#define VK_RMENU                0xA5
#define VK_OEM_MINUS            0xBD
#define VK_OEM_PLUS             0xBB
#define VK_OEM_4                0xDB
#define VK_OEM_6                0xDD
#define TOOLTIPS_CLASSW        L"tooltips_class32"
#define TTS_ALWAYSTIP          0x0001
#define TTS_NOPREFIX           0x0002
#define TTS_BALLOON            0x0040
#define TTF_IDISHWND           0x0001
#define TTF_TRACK              0x0020
#define TTTOOLINFOW_V1_SIZE    ((UINT)(sizeof(TTTOOLINFOW)))
#define TTM_ADDTOOLW           (WM_USER + 50)
#define TTM_DELTOOLW           (WM_USER + 51)
#define TTM_RELAYEVENT        (WM_USER + 7)
#define TTM_TRACKACTIVATE     (WM_USER + 17)
#define TTM_TRACKPOSITION     (WM_USER + 18)
#define TTM_UPDATETIPTEXTW    (WM_USER + 57)
#define WM_USER                0x0400
#define MM_WOM_DONE             0x03BD
#define WAVE_FORMAT_PCM         1
#define WAVE_FORMAT_DIRECT      0x0008
#define WAVE_MAPPER              0xFFFFFFFFu
#define MMSYSERR_NOERROR        0
#define CALLBACK_THREAD         0x00020000
#define WHDR_DONE               0x00000001
#define WHDR_PREPARED           0x00000002
#define TIME_PERIODIC           0x0001
#define TIME_CALLBACK_FUNCTION  0x0000
#define LOGPIXELSX             88
#define SPI_GETNONCLIENTMETRICS 0x0029
#define OFN_OVERWRITEPROMPT    0x00000002
#define OFN_HIDEREADONLY       0x00000004
#define OFN_NOCHANGEDIR        0x00000008
#define OFN_PATHMUSTEXIST      0x00000800
#define OFN_FILEMUSTEXIST      0x00001000
#define ICC_BAR_CLASSES         0x00000004
#define ICC_STANDARD_CLASSES    0x00004000
#define TPM_RETURNCMD           0x0100
#define TPM_LEFTALIGN           0x0000
#define TPM_LEFTBUTTON          0x0000
#define TPM_RIGHTBUTTON         0x0002
#define MAKELPARAM(low, high)   ((LPARAM)((((ULONG_PTR)(WORD)(low)) & 0xffff) | (((ULONG_PTR)(WORD)(high)) << 16)))
#define IDC_ARROW              ((LPCWSTR)(uintptr_t)32512)
#define MAKEINTRESOURCEW(value) ((LPCWSTR)(uintptr_t)(value))
#define RT_RCDATA              MAKEINTRESOURCEW(10)
#define MB_ICONWARNING         0x00000030L
#define MB_DEFBUTTON2          0x00000100L
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

#ifdef __cplusplus
inline int wcscpy_s(wchar_t *destination, size_t destination_size,
                    const wchar_t *source) {
	if (!destination || destination_size == 0 || !source) return 22;
	const size_t length = std::wcslen(source);
	if (length >= destination_size) {
		destination[0] = L'\0';
		return 34;
	}
	std::wmemcpy(destination, source, length + 1);
	return 0;
}
inline int wcscpy_s(wchar_t *destination, const wchar_t *source) {
	if (!destination || !source) return 22;
	std::wcscpy(destination, source);
	return 0;
}
#define _snwprintf_s(buffer, truncate, format, ...) \
	swprintf((buffer), sizeof(buffer) / sizeof(*(buffer)), (format), __VA_ARGS__)
#define _snprintf_s(buffer, truncate, format, ...) \
	snprintf((buffer), sizeof(buffer) / sizeof(*(buffer)), (format), __VA_ARGS__)
#endif
#define MONITOR_DEFAULTTONEAREST 2
#define HWND_TOP               ((HWND)0)
#define HWND_MESSAGE            ((HWND)(LONG_PTR)-3)
#define SWP_NOSIZE             0x0001
#define SWP_NOMOVE             0x0002
#define SWP_NOZORDER           0x0004
#define SWP_NOOWNERZORDER      0x0200
#define SWP_FRAMECHANGED       0x0020

#define MAX_PATH FILENAME_MAX

#define CP_ACP  0
#define CP_UTF8 65001

// some functions that are 1-1 match
#define _stricmp strcasecmp

// memory protection constants
#define PAGE_EXECUTE           0x10
#define PAGE_EXECUTE_READ      0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_EXECUTE_WRITECOPY 0x80
#define PAGE_NOACCESS          0x01
#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04
#define PAGE_WRITECOPY         0x08
#define PAGE_TARGETS_INVALID   0x40000000
#define PAGE_TARGETS_NO_UPDATE 0x40000000

// defined under memoryapi.h
#define MEM_COMMIT  0x00001000
#define MEM_RESERVE 0x00002000
#define MEM_RELEASE 0x00008000

// defined under winuser.h
#define MB_OK              0x00000000L
#define MB_YESNO           0x00000004L
#define MB_ICONERROR       0x00000010L
#define MB_ICONINFORMATION 0x00000040L
#define MB_ICONQUESTION    0x00000020L
#define MB_TASKMODAL       0x00002000L
#define MB_TOPMOST         0x00040000L

#define IDOK  1
#define IDYES 6
#define IDNO  7

// processthreadsapi.h

typedef struct _STARTUPINFOW {
	DWORD cb;
	LPWSTR lpReserved;
	LPWSTR lpDesktop;
	LPWSTR lpTitle;
	DWORD dwX;
	DWORD dwY;
	DWORD dwXSize;
	DWORD dwYSize;
	DWORD dwXCountChars;
	DWORD dwYCountChars;
	DWORD dwFillAttribute;
	DWORD dwFlags;
	WORD wShowWindow;
	WORD cbReserved2;
	unsigned char *lpReserved2;
	HANDLE hStdInput;
	HANDLE hStdOutput;
	HANDLE hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

typedef struct _PROCESS_INFORMATION {
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwProcessId;
	DWORD dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct _SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

typedef struct _CONTEXT {
	DWORD ContextFlags;
	DWORD Eip;
	DWORD Esp;
	DWORD Ebp;
} CONTEXT, *LPCONTEXT;

#define CONTEXT_CONTROL 0x00000001
#define CONTEXT_INTEGER 0x00000002

typedef struct _EXCEPTION_RECORD {
	DWORD ExceptionCode;
	DWORD ExceptionFlags;
	void *ExceptionRecord;
	void *ExceptionAddress;
	DWORD NumberParameters;
	ULONG_PTR ExceptionInformation[15];
} EXCEPTION_RECORD, *PEXCEPTION_RECORD;

typedef struct _EXCEPTION_POINTERS {
	PEXCEPTION_RECORD ExceptionRecord;
	LPCONTEXT ContextRecord;
} EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;

typedef LONG (WINAPI *LPTOP_LEVEL_EXCEPTION_FILTER)(PEXCEPTION_POINTERS);

#define EXCEPTION_ACCESS_VIOLATION 0xC0000005

typedef struct _THREADENTRY32 {
	DWORD dwSize;
	DWORD cntUsage;
	DWORD th32ThreadID;
	DWORD th32OwnerProcessID;
	LONG tpBasePri;
	LONG tpDeltaPri;
	DWORD dwFlags;
} THREADENTRY32, *LPTHREADENTRY32;

typedef struct _SYMBOL_INFO {
	DWORD SizeOfStruct;
	DWORD TypeIndex;
	DWORD64 Reserved[2];
	DWORD Index;
	DWORD Size;
	DWORD64 ModBase;
	DWORD Flags;
	DWORD64 Value;
	DWORD64 Address;
	DWORD Register;
	DWORD Scope;
	DWORD Tag;
	DWORD NameLen;
	DWORD MaxNameLen;
	char Name[1];
} SYMBOL_INFO, *PSYMBOL_INFO;

typedef struct _ADDRESS64 {
	DWORD64 Offset;
	WORD Segment;
	DWORD Mode;
} ADDRESS64;

typedef struct _STACKFRAME64 {
	ADDRESS64 AddrPC;
	ADDRESS64 AddrReturn;
	ADDRESS64 AddrFrame;
	ADDRESS64 AddrStack;
	ADDRESS64 AddrBStore;
	LPVOID FuncTableEntry;
	DWORD64 Params[4];
	DWORD64 Far;
	DWORD64 Virtual;
	DWORD64 Reserved[3];
	BYTE *KdHelp;
} STACKFRAME64, *LPSTACKFRAME64;

#define AddrModeFlat 3
#define IMAGE_FILE_MACHINE_I386 0x014c


// syncapi.h

#define CREATE_WAITABLE_TIMER_MANUAL_RESET    0x00000001
#define CREATE_WAITABLE_TIMER_HIGH_RESOLUTION 0x00000002

#define TIMER_ALL_ACCESS 0x001F0003

#define WAIT_OBJECT_0 0x00000000L

typedef struct tagSCROLLINFO {
	UINT cbSize;
	UINT fMask;
	int nMin;
	int nMax;
	UINT nPage;
	int nPos;
	int nTrackPos;
} SCROLLINFO, *LPSCROLLINFO;

#define SB_HORZ          0
#define SB_VERT          1
#define SB_LINELEFT      0
#define SB_LINERIGHT     1
#define SB_LINEUP        0
#define SB_LINEDOWN      1
#define SB_PAGELEFT      2
#define SB_PAGERIGHT     3
#define SB_PAGEUP        2
#define SB_PAGEDOWN      3
#define SB_THUMBPOSITION 4
#define SB_THUMBTRACK    5
#define SB_TOP           6
#define SB_BOTTOM        7
#define SB_ENDSCROLL     8
#define SIF_RANGE        0x0001
#define SIF_PAGE         0x0002
#define SIF_POS          0x0004
#define SIF_TRACKPOS     0x0010
#define SIF_ALL          (SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS)

typedef struct _MEMORY_BASIC_INFORMATION {
	LPVOID BaseAddress;
	LPVOID AllocationBase;
	DWORD AllocationProtect;
	SIZE_T RegionSize;
	DWORD State;
	DWORD Protect;
	DWORD Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef struct _PROCESS_MEMORY_COUNTERS {
	DWORD cb;
	DWORD PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS, *PPROCESS_MEMORY_COUNTERS;

typedef struct _PROCESS_MEMORY_COUNTERS_EX {
	DWORD cb;
	DWORD PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX, *PPROCESS_MEMORY_COUNTERS_EX;

#define MEM_COMMIT  0x00001000
#define MEM_RESERVE 0x00002000

typedef struct _PROCESS_POWER_THROTTLING_STATE {
	DWORD Version;
	DWORD ControlMask;
	DWORD StateMask;
} PROCESS_POWER_THROTTLING_STATE;

#define PROCESS_POWER_THROTTLING_CURRENT_VERSION 1
#define PROCESS_POWER_THROTTLING_EXECUTION_SPEED 0x1
#define PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION 0x4
#define ProcessPowerThrottling 4

typedef VOID (*PTIMERAPCROUTINE)(
    LPVOID lpArgToCompletionRoutine, // in, optional
    DWORD dwTimerLowValue,           // in
    DWORD dwTimerHighValue           // in
);

// wtypesbase.h

typedef struct _SECURITY_ATTRIBUTES {
	DWORD nLength;
	LPVOID lpSecurityDescriptor;
	BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

// defined under ntdef.h
typedef struct _LIST_ENTRY {
	struct _LIST_ENTRY *Flink;
	struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, PRLIST_ENTRY;

// defined under winnt.h

typedef union _LARGE_INTEGER {
	struct {
		DWORD LowPart;
		LONG HighPart;
	} DUMMYSTRUCTNAME;
	struct {
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER;

struct _RTL_CRITICAL_SECTION;
typedef struct _RTL_CRITICAL_SECTION *PRTL_CRITICAL_SECTION;

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
	WORD Type;
	WORD CreatorBackTraceIndex;
	PRTL_CRITICAL_SECTION CriticalSection;
	LIST_ENTRY ProcessLocksList;
	ULONG EntryCount;
	ULONG ContentionCount;
	ULONG Flags;
	WORD CreatorBackTraceIndexHigh;
	WORD SpareUSHORT;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;

typedef struct _RTL_CRITICAL_SECTION {
	PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
	LONG LockCount;
	LONG RecursionCount;
	HANDLE OwningThread;
	HANDLE LockSemaphore;
	ULONG_PTR SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef CRITICAL_SECTION *PCRITICAL_SECTION;
typedef CRITICAL_SECTION *LPCRITICAL_SECTION;

#ifndef ARRAYSIZE
#define ARRAYSIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif
