#pragma once
// most of these definitions come from
// Windows data types
// https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
#include "winnt.h"
#include "windef.h"
#include "wtypesbase.h"
#ifdef __cplusplus
#include <cstdio>
#include <cstring>
#include <cwchar>
#else
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#endif

// windows type annotations (SAL/source annotation language)
#define _Post_equals_last_error_
#define _In_NLS_string_(size)

#ifndef __int32
#define __int32 int
#endif

typedef struct _CLSID { unsigned char data[16]; } CLSID;

#define S_OK ((HRESULT)0)
#define GMEM_MOVEABLE 0x0002
#define CF_DIB 8
#define LOAD_LIBRARY_SEARCH_SYSTEM32 0x00000800
#define MAKEINTRESOURCEA(value) ((LPCSTR)(uintptr_t)(value))
#define FAILED(hr) ((HRESULT)(hr) < 0)
#define ZeroMemory(ptr, size) std::memset((ptr), 0, (size))

#define STD_INPUT_HANDLE  ((DWORD)-10)
#define STD_OUTPUT_HANDLE ((DWORD)-11)
#define STD_ERROR_HANDLE  ((DWORD)-12)

#define INFINITE 0xFFFFFFFF

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
typedef struct tagMENUBARINFO {
	DWORD cbSize;
	RECT rcBar;
	HMENU hMenu;
	HWND hwndMenu;
	BOOL fBarFocused;
	BOOL fFocused;
} MENUBARINFO;

typedef struct tagMENUITEMINFOW {
	UINT cbSize;
	UINT fMask;
	UINT fType;
	UINT fState;
	UINT wID;
	HMENU hSubMenu;
	HBRUSH hbmpChecked;
	HBRUSH hbmpUnchecked;
	LPWSTR dwTypeData;
	UINT cch;
	ULONG_PTR hbmpItem;
} MENUITEMINFOW;
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

typedef struct tagPAINTSTRUCT {
	HDC hdc;
	BOOL fErase;
	RECT rcPaint;
	BOOL fRestore;
	BOOL fIncUpdate;
	BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT;

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
typedef struct tagMONITORINFOEXW {
	DWORD cbSize;
	RECT rcMonitor;
	RECT rcWork;
	DWORD dwFlags;
	WCHAR szDevice[32];
} MONITORINFOEXW;
typedef struct tagDEVMODEW {
	WCHAR dmDeviceName[32];
	WORD dmSpecVersion, dmDriverVersion, dmSize, dmDriverExtra;
	DWORD dmFields;
	DWORD dmDisplayFrequency;
} DEVMODEW;
typedef BOOL (CALLBACK *MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);
#define ENUM_CURRENT_SETTINGS ((DWORD)-1)

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
constexpr int PixelFormat32bppRGB = 0x26200A;
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
#define DT_HIDEPREFIX           0x00100000
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
#define WM_NCPAINT              0x0085
#define WM_NCACTIVATE           0x0086
#define WM_THEMECHANGED         0x031A
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
#define WS_BORDER               0x00800000
#define ES_AUTOHSCROLL          0x0080
#define SS_LEFTNOWORDWRAP       0x000C
#define EM_SETLIMITTEXT         0x00C5
#define DC_BRUSH                18
#define ODT_TAB                 101
#define NM_RCLICK               ((UINT)-5)
#define VK_RETURN               0x0D
#define WM_CHAR                 0x0102
#define MAKEWPARAM(low, high)   ((WPARAM)((((WORD)(low)) & 0xffff) | (((ULONG_PTR)((WORD)(high))) << 16)))
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
#define OBJID_MENU             ((LONG_PTR)-3)
#define MIIM_STRING             0x00000040
#define ODS_SELECTED            0x0001
#define ODS_GRAYED              0x0002
#define ODS_DISABLED            0x0004
#define ODS_HOTLIGHT            0x0040
#define ODS_NOACCEL             0x0100
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
#define WM_USER                0x0400
#define LOGPIXELSX             88
#define SPI_GETNONCLIENTMETRICS 0x0029
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

#define CP_ACP  0
#define CP_UTF8 65001

// some functions that are 1-1 match
#define _stricmp strcasecmp

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
BOOL GetMonitorInfoW(HMONITOR hMonitor, MONITORINFOEXW *lpmi);
BOOL EnumDisplayMonitors(HDC hdc, const RECT *clip, MONITORENUMPROC proc,
                         LPARAM data);
BOOL EnumDisplaySettingsW(LPCWSTR device, DWORD mode, DEVMODEW *devmode);
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
LRESULT CallWindowProcW(WNDPROC prev, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL IsIconic(HWND hWnd);
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
HWND GetParent(HWND hWnd);
int GetWindowTextW(HWND hWnd, LPWSTR text, int maxCount);
COLORREF SetDCBrushColor(HDC hdc, COLORREF color);
BOOL TextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c);
BOOL GetTextMetricsW(HDC hdc, TEXTMETRICW *lptm);
BOOL TextOutW(HDC hdc, int x, int y, LPCWSTR lpString, int c);
BOOL MoveFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName,
                 DWORD dwFlags);
BOOL TerminateProcess(HANDLE hProcess, UINT uExitCode);
BOOL GetExitCodeProcess(HANDLE hProcess, LPDWORD lpExitCode);
BOOL DeleteFileW(LPCWSTR lpFileName);
BOOL CreateDirectoryW(LPCWSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
BOOL OpenClipboard(HWND hWndNewOwner);
BOOL EmptyClipboard(void);
HANDLE SetClipboardData(UINT uFormat, HANDLE hMem);
BOOL CloseClipboard(void);
BOOL SetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove,
                      LARGE_INTEGER *lpNewFilePointer, DWORD dwMoveMethod);
BOOL GetFileSizeEx(HANDLE hFile, LARGE_INTEGER *lpFileSize);
VOID GetSystemInfo(SYSTEM_INFO *lpSystemInfo);
BOOL DeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode,
                     LPVOID lpInBuffer, DWORD nInBufferSize,
                     LPVOID lpOutBuffer, DWORD nOutBufferSize,
                     LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);
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

SIZE_T VirtualQuery(LPCVOID lpAddress,
                    PMEMORY_BASIC_INFORMATION lpBuffer,
                    SIZE_T dwLength);

// stringapiset.h
int MultiByteToWideChar(UINT CodePage,        // in
                        DWORD dwFlags,        // in
                        LPCCH lpMultiByteStr, // in
                        int cbMultiByte,      // in
                        LPWSTR lpWideCharStr, // out, optional
                        int cchWideChar       // in
);

int WideCharToMultiByte(UINT CodePage,          // in
                        DWORD dwFlags,          // in
                        LPCWSTR lpWideCharStr,  // in
                        int cchWideChar,        // in
                        LPSTR lpMultiByteStr,   // out, optional
                        int cbMultiByte,       // in
                        LPCSTR lpDefaultChar,  // in, optional
                        BOOL *lpUsedDefaultChar // out, optional
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
HMODULE LoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
LPVOID GlobalAlloc(UINT uFlags, SIZE_T dwBytes);
LPVOID GlobalLock(HGLOBAL hMem);
BOOL GlobalUnlock(HGLOBAL hMem);
HGLOBAL GlobalFree(HGLOBAL hMem);
int GetClassNameW(HWND hwnd, LPWSTR className, int maxCount);
int lstrcmpiW(LPCWSTR string1, LPCWSTR string2);
BOOL GetMenuBarInfo(HWND hwnd, LONG idObject, LONG idItem, MENUBARINFO *pmbi);
BOOL GetMenuItemInfoW(HMENU hMenu, UINT item, BOOL byPosition,
                      MENUITEMINFOW *info);
BOOL OffsetRect(LPRECT rect, int dx, int dy);
HDC GetWindowDC(HWND hwnd);
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

// wingdi.h / winuser.h
BOOL InvalidateRect(HWND hWnd, const RECT *lpRect, BOOL bErase);
int SetStretchBltMode(HDC hdc, int mode);
BOOL SetBrushOrgEx(HDC hdc, int x, int y, POINT *old);
BOOL StretchBlt(HDC hdc, int x, int y, int cx, int cy, HDC src,
               int sx, int sy, int scx, int scy, DWORD rop);
int SetBkMode(HDC hdc, int mode);
COLORREF SetTextColor(HDC hdc, COLORREF color);
COLORREF SetBkColor(HDC hdc, COLORREF color);
BOOL GdiFlush(void);
int ShowScrollBar(HWND hWnd, int wBar, BOOL bShow);
int SetScrollInfo(HWND hwnd, int nBar, LPSCROLLINFO lpsi, BOOL redraw);
BOOL GetScrollInfo(HWND hwnd, int nBar, LPSCROLLINFO lpsi);
HGDIOBJ GetStockObject(int i);

// winbase.h
VOID ExitProcess(UINT uExitCode);

// errhandlingapi.h
LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilter(
    LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
