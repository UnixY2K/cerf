#pragma once
#ifdef __cplusplus
#include <cstdarg>
#else
#include <stdarg.h>
#endif

#include "windef.h"
#include "wingdi.h"

typedef struct tagWINDOWPLACEMENT {
    UINT length;
    UINT flags;
    UINT showCmd;
    POINT ptMinPosition;
    POINT ptMaxPosition;
    RECT rcNormalPosition;
} WINDOWPLACEMENT, *PWINDOWPLACEMENT;

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
typedef BOOL (CALLBACK *MONITORENUMPROC)(HMONITOR, HDC, LPRECT, LPARAM);

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

typedef struct tagSCROLLINFO {
    UINT cbSize;
    UINT fMask;
    int nMin;
    int nMax;
    UINT nPage;
    int nPos;
    int nTrackPos;
} SCROLLINFO, *LPSCROLLINFO;

// Window Styles & Messages
#define WS_OVERLAPPEDWINDOW    0x00CF0000
#define WS_POPUP               0x80000000
#define WS_CHILD               0x40000000
#define WS_VISIBLE             0x10000000
#define WS_CLIPSIBLINGS        0x04000000
#define WS_CLIPCHILDREN        0x02000000
#define WS_CAPTION             0x00C00000
#define WS_BORDER              0x00800000
#define WS_DLGFRAME            0x00400000
#define WS_VSCROLL             0x00200000
#define WS_SYSMENU             0x00080000
#define WS_GROUP               0x00020000
#define WS_TABSTOP             0x00010000

#define WS_EX_DLGMODALFRAME    0x00000001
#define WS_EX_TOPMOST          0x00000008
#define WS_EX_TOOLWINDOW       0x00000080

#define CS_VREDRAW             0x0001
#define CS_HREDRAW             0x0002
#define CW_USEDEFAULT          ((int)0x80000000)

#define SW_HIDE                0
#define SW_SHOWNORMAL          1
#define SW_SHOW                5
#define SW_RESTORE             9
#define SIZE_MINIMIZED         1

#define WM_DESTROY             0x0002
#define WM_SIZE                0x0005
#define WM_ACTIVATE            0x0006
#define WM_CLOSE               0x0010
#define WM_QUIT                0x0012
#define WM_ERASEBKGND          0x0014
#define WM_PAINT               0x000F
#define WM_SETCURSOR           0x0020
#define WM_GETMINMAXINFO       0x0024
#define WM_DRAWITEM            0x002B
#define WM_SETFONT             0x0030
#define WM_NCCREATE            0x0081
#define WM_NCPAINT             0x0085
#define WM_NCACTIVATE          0x0086
#define WM_KEYDOWN             0x0100
#define WM_KEYUP               0x0101
#define WM_CHAR                0x0102
#define WM_SYSKEYDOWN          0x0104
#define WM_SYSKEYUP            0x0105
#define WM_COMMAND             0x0111
#define WM_SYSCOMMAND          0x0112
#define WM_TIMER               0x0113
#define WM_HSCROLL             0x0114
#define WM_VSCROLL             0x0115
#define WM_INITMENUPOPUP       0x0117
#define WM_CTLCOLOREDIT        0x0133
#define WM_CTLCOLORLISTBOX     0x0134
#define WM_CTLCOLORBTN         0x0135
#define WM_CTLCOLORDLG         0x0136
#define WM_CTLCOLORSTATIC      0x0138
#define WM_MOUSEMOVE           0x0200
#define WM_LBUTTONDOWN         0x0201
#define WM_LBUTTONUP           0x0202
#define WM_RBUTTONDOWN         0x0204
#define WM_RBUTTONUP           0x0205
#define WM_MBUTTONDOWN         0x0207
#define WM_MBUTTONUP           0x0208
#define WM_MOUSEWHEEL          0x020A
#define WM_CAPTURECHANGED      0x0215
#define WM_ENTERSIZEMOVE       0x0231
#define WM_EXITSIZEMOVE        0x0232
#define WM_THEMECHANGED        0x031A
#define WM_USER                0x0400
#define WM_APP                 0x8000

// Control & Dialog messages/styles
#define CB_ADDSTRING           0x0143
#define CB_SETCURSEL           0x014E
#define CB_GETCURSEL           0x0147
#define CB_ERR                 (-1)
#define CBS_DROPDOWNLIST       0x0003
#define CBN_SELCHANGE          1
#define CBN_DROPDOWN           7

#define BM_GETCHECK            0x00F0
#define BM_SETCHECK            0x00F1
#define BST_UNCHECKED          0
#define BST_CHECKED            1
#define BS_PUSHBUTTON          0x00000000
#define BS_DEFPUSHBUTTON       0x00000001
#define BS_AUTOCHECKBOX        0x00000003
#define BN_CLICKED             0

#define SS_LEFT                0x00000000
#define SS_LEFTNOWORDWRAP      0x000C
#define ES_AUTOHSCROLL         0x0080
#define EM_SETLIMITTEXT        0x00C5

#define ODT_TAB                101
#define NM_RCLICK              ((UINT)-5)

#define IDOK                   1
#define IDCANCEL               2
#define IDYES                  6
#define IDNO                   7

#define MB_OK                  0x00000000L
#define MB_YESNO               0x00000004L
#define MB_ICONERROR           0x00000010L
#define MB_ICONQUESTION        0x00000020L
#define MB_ICONWARNING         0x00000030L
#define MB_ICONINFORMATION     0x00000040L
#define MB_DEFBUTTON2          0x00000100L
#define MB_TASKMODAL           0x00002000L
#define MB_TOPMOST             0x00040000L

#define SC_MAXIMIZE            0xF030
#define WA_INACTIVE            0

// Menus
#define MF_STRING              0x0000
#define MF_BYCOMMAND           0x0000
#define MF_UNCHECKED           0x0000
#define MF_GRAYED              0x0001
#define MF_CHECKED             0x0008
#define MF_POPUP               0x0010
#define MF_BYPOSITION          0x0400
#define MF_SEPARATOR           0x0800
#define MIIM_STRING            0x00000040
#define OBJID_MENU             ((LONG_PTR)-3)
#define TPM_LEFTBUTTON         0x0000
#define TPM_LEFTALIGN          0x0000
#define TPM_RIGHTBUTTON        0x0002
#define TPM_RETURNCMD          0x0100
#define ODS_SELECTED           0x0001
#define ODS_GRAYED             0x0002
#define ODS_DISABLED           0x0004
#define ODS_HOTLIGHT           0x0040
#define ODS_NOACCEL            0x0100

// Cursor, Icon, Mouse, Keyboard
#define IMAGE_ICON             1
#define LR_DEFAULTCOLOR        0x00000000
#define DI_NORMAL              0x0003
#define MAKEINTRESOURCEA(value) ((LPCSTR)(uintptr_t)(value))
#define MAKEINTRESOURCEW(value) ((LPCWSTR)(uintptr_t)(value))
#define IDC_ARROW              MAKEINTRESOURCEW(32512)
#define IDC_HAND               MAKEINTRESOURCEW(32649)
#define RT_RCDATA              MAKEINTRESOURCEW(10)
#define HC_ACTION              0
#define WH_KEYBOARD_LL         13
#define MAPVK_VK_TO_VSC        0
#define HTCLIENT               1
#define MK_LBUTTON             0x0001
#define MK_RBUTTON             0x0002
#define MK_MBUTTON             0x0010

#define VK_CANCEL              0x03
#define VK_RETURN              0x0D
#define VK_SHIFT               0x10
#define VK_CONTROL             0x11
#define VK_MENU                0x12
#define VK_ESCAPE              0x1B
#define VK_SPACE               0x20
#define VK_PRIOR               0x21
#define VK_NEXT                0x22
#define VK_END                 0x23
#define VK_HOME                0x24
#define VK_LEFT                0x25
#define VK_UP                  0x26
#define VK_RIGHT               0x27
#define VK_DOWN                0x28
#define VK_SNAPSHOT            0x2C
#define VK_INSERT              0x2D
#define VK_DELETE              0x2E
#define VK_LWIN                0x5B
#define VK_RWIN                0x5C
#define VK_APPS                0x5D
#define VK_DIVIDE              0x6F
#define VK_LSHIFT              0xA0
#define VK_RSHIFT              0xA1
#define VK_LCONTROL            0xA2
#define VK_RCONTROL            0xA3
#define VK_LMENU               0xA4
#define VK_RMENU               0xA5
#define VK_OEM_PLUS            0xBB
#define VK_OEM_MINUS           0xBD
#define VK_OEM_4               0xDB
#define VK_OEM_6               0xDD

// Window Longs & Hierarchy
#define GW_HWNDNEXT            2
#define GW_CHILD               5
#define GWL_STYLE              (-16)
#define GWL_EXSTYLE            (-20)
#define GWLP_USERDATA          (-21)
#define HWND_TOP               ((HWND)0)
#define HWND_MESSAGE           ((HWND)(LONG_PTR)-3)
#define SWP_NOSIZE             0x0001
#define SWP_NOMOVE             0x0002
#define SWP_NOZORDER           0x0004
#define SWP_FRAMECHANGED       0x0020
#define SWP_NOOWNERZORDER      0x0200

// Queue & Loop
#define QS_ALLINPUT            0x04FF
#define PM_NOREMOVE            0x0000
#define PM_REMOVE              0x0001
#define MWMO_INPUTAVAILABLE    0x0004

// Monitor & Display
#define MONITOR_DEFAULTTONEAREST 2
#define ENUM_CURRENT_SETTINGS    ((DWORD)-1)

// Colors, Metrics, DrawText
#define COLOR_BTNFACE          15
#define COLOR_BTNTEXT          18
#define USER_DEFAULT_SCREEN_DPI 96
#define SPI_GETNONCLIENTMETRICS 0x0029
#define CF_DIB                 8

#define DT_TOP                 0x0000
#define DT_LEFT                0x0000
#define DT_CENTER              0x0001
#define DT_VCENTER             0x0004
#define DT_BOTTOM              0x0008
#define DT_WORDBREAK           0x0010
#define DT_SINGLELINE          0x0020
#define DT_CALCRECT            0x0400
#define DT_NOPREFIX            0x0800
#define DT_END_ELLIPSIS        0x8000
#define DT_HIDEPREFIX          0x00100000

// Scrollbars
#define SB_HORZ                0
#define SB_VERT                1
#define SB_LINEUP              0
#define SB_LINELEFT            0
#define SB_LINEDOWN            1
#define SB_LINERIGHT           1
#define SB_PAGEUP              2
#define SB_PAGELEFT            2
#define SB_PAGEDOWN            3
#define SB_PAGERIGHT           3
#define SB_THUMBPOSITION       4
#define SB_THUMBTRACK          5
#define SB_TOP                 6
#define SB_BOTTOM              7
#define SB_ENDSCROLL           8
#define SIF_RANGE              0x0001
#define SIF_PAGE               0x0002
#define SIF_POS                0x0004
#define SIF_TRACKPOS           0x0010
#define SIF_ALL                (SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS)

// Functions
HWND CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName,
                     DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
                     HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
BOOL DestroyWindow(HWND hWnd);
BOOL ShowWindow(HWND hWnd, int nCmdShow);
BOOL UpdateWindow(HWND hWnd);
LRESULT DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CallWindowProcW(WNDPROC prev, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
ATOM RegisterClassExW(const WNDCLASSEXW *lpwcx);

BOOL IsZoomed(HWND hWnd);
BOOL IsIconic(HWND hWnd);
BOOL SetWindowTextW(HWND hWnd, LPCWSTR lpString);
int GetWindowTextW(HWND hWnd, LPWSTR text, int maxCount);
int GetClassNameW(HWND hwnd, LPWSTR className, int maxCount);
BOOL GetClientRect(HWND hWnd, LPRECT lpRect);
BOOL GetWindowRect(HWND hWnd, LPRECT lpRect);
BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint);
BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
BOOL SetWindowPlacement(HWND hWnd, const WINDOWPLACEMENT *lpwndpl);
BOOL GetWindowPlacement(HWND hWnd, PWINDOWPLACEMENT lpwndpl);
BOOL AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);
BOOL AdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);

LONG GetWindowLongW(HWND hWnd, int nIndex);
LONG SetWindowLongW(HWND hWnd, int nIndex, LONG dwNewLong);
LONG_PTR GetWindowLongPtrW(HWND hWnd, int nIndex);
LONG_PTR SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong);
HWND GetWindow(HWND hWnd, UINT uCmd);
HWND GetParent(HWND hWnd);
BOOL SetFocus(HWND hWnd);
BOOL EnableWindow(HWND hWnd, BOOL bEnable);
BOOL EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam);
BOOL IsDialogMessageW(HWND hDlg, const MSG *lpMsg);
BOOL AllowSetForegroundWindow(DWORD dwProcessId);
BOOL SetForegroundWindow(HWND hWnd);
HWND GetForegroundWindow(void);

LRESULT SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL TranslateMessage(const MSG *lpMsg);
LRESULT DispatchMessageW(const MSG *lpMsg);
VOID PostQuitMessage(int nExitCode);
DWORD MsgWaitForMultipleObjects(DWORD nCount, const HANDLE *pHandles, BOOL fWaitAll,
                                DWORD dwMilliseconds, DWORD dwWakeMask);
DWORD MsgWaitForMultipleObjectsEx(DWORD nCount, const HANDLE *pHandles, DWORD dwMilliseconds,
                                  DWORD dwWakeMask, DWORD dwFlags);
DWORD GetMessagePos(void);

HDC BeginPaint(HWND hWnd, PPAINTSTRUCT lpPaint);
BOOL EndPaint(HWND hWnd, const PAINTSTRUCT *lpPaint);
HDC GetDC(HWND hWnd);
int ReleaseDC(HWND hWnd, HDC hDC);
HDC GetWindowDC(HWND hwnd);
BOOL InvalidateRect(HWND hWnd, const RECT *lpRect, BOOL bErase);
int FillRect(HDC hDC, const RECT *lprc, HBRUSH hbr);
BOOL FrameRect(HDC hDC, const RECT *lprc, HBRUSH hbr);
int DrawTextW(HDC hdc, LPCWSTR lpchText, int cchText, LPRECT lprc, UINT format);

int MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);
int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

HMENU CreateMenu(void);
HMENU CreatePopupMenu(void);
BOOL AppendMenuW(HMENU hMenu, UINT uFlags, UINT_PTR uIdNewItem, LPCWSTR lpNewItem);
HMENU GetSubMenu(HMENU hMenu, int nPos);
int GetMenuItemCount(HMENU hMenu);
BOOL DeleteMenu(HMENU hMenu, UINT uPosition, UINT uFlags);
HMENU GetMenu(HWND hWnd);
BOOL SetMenu(HWND hWnd, HMENU hMenu);
BOOL DestroyMenu(HMENU hMenu);
DWORD CheckMenuItem(HMENU hMenu, UINT uIDCheckItem, UINT uCheck);
BOOL CheckMenuRadioItem(HMENU hMenu, UINT idFirst, UINT idLast, UINT idCheck, UINT uFlags);
UINT TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved,
                    HWND hWnd, const RECT *prcRect);
BOOL GetMenuBarInfo(HWND hwnd, LONG idObject, LONG idItem, MENUBARINFO *pmbi);
BOOL GetMenuItemInfoW(HMENU hMenu, UINT item, BOOL byPosition, MENUITEMINFOW *info);

HWND GetCapture(void);
BOOL ReleaseCapture(void);
HWND SetCapture(HWND hWnd);
BOOL ScreenToClient(HWND hWnd, PPOINT lpPoint);
BOOL ClientToScreen(HWND hWnd, PPOINT lpPoint);
int SetCursorPos(int X, int Y);
int ShowCursor(BOOL bShow);
BOOL GetCursorPos(PPOINT lpPoint);
HANDLE SetCursor(HANDLE hCursor);
HCURSOR LoadCursorW(HINSTANCE hInstance, LPCWSTR lpCursorName);
HCURSOR CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight,
                     const void *pvANDPlane, const void *pvXORPlane);
BOOL DestroyCursor(HCURSOR hCursor);
HICON LoadIconW(HINSTANCE hInstance, LPCWSTR lpIconName);
BOOL DestroyIcon(HICON hIcon);
HANDLE LoadImageW(HINSTANCE hInst, LPCWSTR name, UINT type, int cx, int cy, UINT fuLoad);
BOOL DrawIconEx(HDC hdc, int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth,
                UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags);

UINT MapVirtualKeyW(UINT uCode, UINT uMapType);
int GetKeyNameTextW(LONG lParam, LPWSTR lpString, int cchSize);
SHORT GetKeyState(int nVirtKey);
HHOOK SetWindowsHookExW(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId);
BOOL UnhookWindowsHookEx(HHOOK hhk);
LRESULT CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam);

UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, LPVOID lpTimerFunc);
BOOL KillTimer(HWND hWnd, UINT_PTR uIDEvent);

HMONITOR MonitorFromWindow(HWND hwnd, DWORD dwFlags);
BOOL GetMonitorInfoW(HMONITOR hMonitor, LPMONITORINFO lpmi);
BOOL GetMonitorInfoW(HMONITOR hMonitor, MONITORINFOEXW *lpmi);
BOOL EnumDisplayMonitors(HDC hdc, const RECT *clip, MONITORENUMPROC proc, LPARAM data);
BOOL EnumDisplaySettingsW(LPCWSTR device, DWORD mode, DEVMODEW *devmode);

int ShowScrollBar(HWND hWnd, int wBar, BOOL bShow);
int SetScrollInfo(HWND hwnd, int nBar, LPSCROLLINFO lpsi, BOOL redraw);
BOOL GetScrollInfo(HWND hwnd, int nBar, LPSCROLLINFO lpsi);

BOOL OpenClipboard(HWND hWndNewOwner);
BOOL CloseClipboard(void);
BOOL EmptyClipboard(void);
HANDLE SetClipboardData(UINT uFormat, HANDLE hMem);

BOOL SystemParametersInfoW(UINT uiAction, UINT uiParam, LPVOID pvParam, UINT fWinIni);
int GetSysColor(int nIndex);
HBRUSH GetSysColorBrush(int nIndex);

int MapWindowPoints(HWND hWndFrom, HWND hWndTo, PPOINT lpPoints, UINT cPoints);
BOOL OffsetRect(LPRECT rect, int dx, int dy);
BOOL InflateRect(LPRECT lprc, int dx, int dy);
BOOL PtInRect(const RECT *lprc, POINT pt);
BOOL EqualRect(const RECT *lprc1, const RECT *lprc2);

int wsprintfA(LPSTR lpOut, LPCSTR lpFmt, ...);
int wvsprintfA(LPSTR lpOut, LPCSTR lpFmt, va_list arglist);
