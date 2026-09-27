#pragma once
// https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
#include "basetsd.h"
#include "winnt.h"
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

typedef uint32_t UINT;
typedef int32_t INT;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;

typedef DWORD *LPDWORD;
typedef WORD *LPWORD;
typedef uint16_t USHORT;
typedef WORD ATOM;

typedef DWORD COLORREF;

typedef void *LPVOID;
typedef const void *LPCVOID;

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
typedef HANDLE HWND;
typedef HINSTANCE HMODULE;
typedef HANDLE HMONITOR;
typedef void *FARPROC;

typedef int BOOL;

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

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

typedef struct tagSIZE {
	LONG cx;
	LONG cy;
} SIZE, *PSIZE, *LPSIZE;

#define MAX_PATH FILENAME_MAX

#define LOWORD(value) ((WORD)(((ULONG_PTR)(value)) & 0xffff))
#define HIWORD(value) ((WORD)((((ULONG_PTR)(value)) >> 16) & 0xffff))
#define MAKEWPARAM(low, high)                                                  \
	((WPARAM)((((WORD)(low)) & 0xffff) | (((ULONG_PTR)((WORD)(high))) << 16)))
#define MAKELPARAM(low, high)                                                  \
	((LPARAM)((((ULONG_PTR)(WORD)(low)) & 0xffff) |                            \
	          (((ULONG_PTR)(WORD)(high)) << 16)))
