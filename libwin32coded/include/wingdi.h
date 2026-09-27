#pragma once
#include "windef.h"

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

typedef struct tagDEVMODEW {
    WCHAR dmDeviceName[32];
    WORD dmSpecVersion, dmDriverVersion, dmSize, dmDriverExtra;
    DWORD dmFields;
    DWORD dmDisplayFrequency;
} DEVMODEW;

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

#define RGB(r, g, b) ((COLORREF)(((BYTE)(r) | ((WORD)(g) << 8)) | ((DWORD)(BYTE)(b) << 16)))
#define GetRValue(rgb) ((BYTE)(rgb))
#define GetGValue(rgb) ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb) ((BYTE)((rgb) >> 16))

#define SRCCOPY         0x00CC0020
#define BI_RGB          0
#define DIB_RGB_COLORS  0
#define HALFTONE        4
#define COLORONCOLOR    3
#define PS_SOLID        0
#define TRANSPARENT     1
#define AC_SRC_OVER     0x00

#define DEFAULT_GUI_FONT 17
#define DC_BRUSH         18
#define GRAY_BRUSH       2
#define BLACK_BRUSH      4
#define NULL_BRUSH       5

#define FW_BOLD             700
#define FW_NORMAL           400
#define DEFAULT_CHARSET     1
#define OUT_DEFAULT_PRECIS  0
#define CLIP_DEFAULT_PRECIS 0
#define CLEARTYPE_QUALITY   5
#define VARIABLE_PITCH      2
#define FIXED_PITCH         1
#define FF_SWISS            32
#define FF_MODERN           48

#define LOGPIXELSX 88

HDC CreateCompatibleDC(HDC hdc);
HBITMAP CreateCompatibleBitmap(HDC hdc, int cx, int cy);
HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO *pbmi, UINT usage,
                         LPVOID *ppvBits, HANDLE hSection, DWORD offset);
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ h);
BOOL DeleteObject(HGDIOBJ ho);
BOOL DeleteDC(HDC hdc);

HBRUSH CreateSolidBrush(COLORREF color);
HPEN CreatePen(int iStyle, int cWidth, COLORREF color);
BOOL Rectangle(HDC hdc, int left, int top, int right, int bottom);
BOOL Ellipse(HDC hdc, int left, int top, int right, int bottom);
BOOL MoveToEx(HDC hdc, int x, int y, PPOINT lpPoint);
BOOL LineTo(HDC hdc, int x, int y);
BOOL BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
BOOL StretchBlt(HDC hdc, int x, int y, int cx, int cy, HDC src,
               int sx, int sy, int scx, int scy, DWORD rop);
BOOL AlphaBlend(HDC hdcDest, int xoriginDest, int yoriginDest, int wDest,
                int hDest, HDC hdcSrc, int xoriginSrc, int yoriginSrc,
                int wSrc, int hSrc, BLENDFUNCTION ftn);

int SetStretchBltMode(HDC hdc, int mode);
BOOL SetBrushOrgEx(HDC hdc, int x, int y, POINT *old);
int SetBkMode(HDC hdc, int mode);
COLORREF SetTextColor(HDC hdc, COLORREF color);
COLORREF SetBkColor(HDC hdc, COLORREF color);
COLORREF SetDCBrushColor(HDC hdc, COLORREF color);

int GetDeviceCaps(HDC hdc, int index);
int GetObjectW(HGDIOBJ h, int c, LPVOID pv);
BOOL GetTextMetricsW(HDC hdc, TEXTMETRICW *lptm);
BOOL TextOutA(HDC hdc, int x, int y, LPCSTR lpString, int c);
BOOL TextOutW(HDC hdc, int x, int y, LPCWSTR lpString, int c);
HFONT CreateFontW(int cHeight, int cWidth, int cEscapement, int cOrientation,
                  int cWeight, DWORD bItalic, DWORD bUnderline,
                  DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision,
                  DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily,
                  LPCWSTR pszFaceName);
HFONT CreateFontIndirectW(const LOGFONTW *lplf);
HANDLE AddFontMemResourceEx(LPVOID pbFont, DWORD cbFont, LPVOID pdv,
                            LPDWORD pcFonts);
BOOL RemoveFontMemResourceEx(HANDLE hFont);
HGDIOBJ GetStockObject(int i);
BOOL GdiFlush(void);
