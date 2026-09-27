#pragma once
#include "windows.h"

typedef struct tagNMHDR {
	HWND hwndFrom;
	UINT_PTR idFrom;
	UINT code;
} NMHDR;

typedef struct tagNMCUSTOMDRAW {
	NMHDR hdr;
	DWORD dwDrawStage;
	HDC hdc;
	RECT rc;
	ULONG_PTR dwItemSpec;
	UINT uItemState;
	LPARAM lItemlParam;
} NMCUSTOMDRAW;

#define CDDS_PREPAINT       0x00000001
#define CDDS_ITEMPREPAINT   0x00010001
#define CDRF_NOTIFYITEMDRAW 0x00000020
#define CDRF_NEWFONT        0x00000002

typedef struct tagINITCOMMONCONTROLSEX {
	DWORD dwSize;
	DWORD dwICC;
} INITCOMMONCONTROLSEX;

BOOL InitCommonControlsEx(const INITCOMMONCONTROLSEX *lpInitCtrls);

typedef struct tagNMLINK {
	struct {
		wchar_t szUrl[2048];
	} item;
} NMLINK;

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

typedef struct tagLVITEMW {
	UINT mask; int iItem; int iSubItem; UINT state; UINT stateMask;
	LPWSTR pszText; int cchTextMax; int iImage; LPARAM lParam;
} LVITEMW;
typedef struct tagTCITEMW {
	UINT mask; DWORD dwState; DWORD dwStateMask; LPWSTR pszText;
	int cchTextMax; int iImage; LPARAM lParam;
} TCITEMW;
typedef struct tagNMITEMACTIVATE {
	NMHDR hdr; int iItem; int iSubItem; UINT uNewState; UINT uOldState;
	UINT uChanged; POINT ptAction; LPARAM lParam;
} NMITEMACTIVATE;
typedef struct tagLVCOLUMNW {
	UINT mask; int fmt; int cx; LPWSTR pszText; int cchTextMax;
	int iSubItem; int iImage; int iOrder;
} LVCOLUMNW;

#define ICC_LISTVIEW_CLASSES       0x00000001
#define ICC_TAB_CLASSES            0x00000008
#define ICC_BAR_CLASSES            0x00000004
#define ICC_STANDARD_CLASSES       0x00004000
#define WS_EX_CLIENTEDGE           0x00000200
#define WC_LISTVIEWW               L"SysListView32"
#define WC_TABCONTROLW             L"SysTabControl32"
#define LVS_REPORT                 0x0001
#define LVS_SINGLESEL              0x0004
#define LVS_SHOWSELALWAYS          0x0008
#define LVS_EX_FULLROWSELECT       0x0020
#define LVS_EX_HEADERDRAGDROP      0x0010
#define LVIS_SELECTED              0x0002
#define LVIS_FOCUSED               0x0001
#define TCS_OWNERDRAWFIXED         0x0001
#define TCIF_TEXT                  0x0001
#define TCN_SELCHANGE              (-551)
#define GWLP_WNDPROC               (-4)
#define WM_NOTIFY                  0x004E
#define NM_CUSTOMDRAW              ((UINT)-12)
#define LVM_INSERTITEMW            (WM_USER + 77)
#define LVM_SETITEMTEXTW           (WM_USER + 116)
#define LVM_GETNEXTITEM            (WM_USER + 12)
#define LVM_GETITEMW               (WM_USER + 75)
#define LVNI_SELECTED              0x0002
#define LVIF_TEXT                  0x0001
#define LVIF_PARAM                 0x0004
#define LVCF_WIDTH                 0x0002
#define LVCF_TEXT                  0x0004
#define LVM_INSERTCOLUMNW          (WM_USER + 97)
#define LVM_GETITEMW               (WM_USER + 75)
#define HDM_GETITEMCOUNT           (WM_USER + 0)
#define WM_SETREDRAW               0x000B

#define ListView_InsertItem(hwnd, item) \
	((int)SendMessageW((hwnd), LVM_INSERTITEMW, 0, (LPARAM)(item)))
#define ListView_SetItemText(hwnd, item, subitem, text) \
	((void)SendMessageW((hwnd), LVM_SETITEMTEXTW, (WPARAM)(item), (LPARAM)(text)))
#define ListView_GetNextItem(hwnd, item, flags) \
	((int)SendMessageW((hwnd), LVM_GETNEXTITEM, (WPARAM)(item), (LPARAM)(flags)))
#define ListView_GetItem(hwnd, item) \
	((BOOL)SendMessageW((hwnd), LVM_GETITEMW, 0, (LPARAM)(item)))
#define ListView_SetBkColor(hwnd, color) ((void)SendMessageW((hwnd), 0x1001, 0, (LPARAM)(color)))
#define ListView_SetTextBkColor(hwnd, color) ((void)SendMessageW((hwnd), 0x1026, 0, (LPARAM)(color)))
#define ListView_SetTextColor(hwnd, color) ((void)SendMessageW((hwnd), 0x1024, 0, (LPARAM)(color)))
#define ListView_GetHeader(hwnd) ((HWND)SendMessageW((hwnd), 0x101F, 0, 0))
#define TabCtrl_AdjustRect(hwnd, enlarge, rect) ((void)SendMessageW((hwnd), 0x1328, (WPARAM)(enlarge), (LPARAM)(rect)))
#define TabCtrl_GetCurSel(hwnd) ((int)SendMessageW((hwnd), 0x130B, 0, 0))
#define TabCtrl_InsertItem(hwnd, item, data) ((int)SendMessageW((hwnd), 0x1307, (WPARAM)(item), (LPARAM)(data)))
#define TabCtrl_GetItem(hwnd, item, data) ((BOOL)SendMessageW((hwnd), 0x1305, (WPARAM)(item), (LPARAM)(data)))
#define ListView_InsertColumn(hwnd, col, data) ((int)SendMessageW((hwnd), LVM_INSERTCOLUMNW, (WPARAM)(col), (LPARAM)(data)))
#define ListView_SetExtendedListViewStyle(hwnd, style) ((void)SendMessageW((hwnd), 0x1036, 0, (LPARAM)(style)))
#define ListView_SetItemState(hwnd, item, state, mask) ((void)SendMessageW((hwnd), 0x102B, (WPARAM)(item), (LPARAM)((((DWORD)(mask)) << 16) | (state))))
#define ListView_DeleteAllItems(hwnd) ((void)SendMessageW((hwnd), 0x1009, 0, 0))
#define ListView_DeleteColumn(hwnd, col) ((BOOL)SendMessageW((hwnd), 0x100C, (WPARAM)(col), 0))
