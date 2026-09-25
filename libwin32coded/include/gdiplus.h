#pragma once
#include "windows.h"

namespace Gdiplus {

using REAL = float;
using Status = int;
using Unit = int;
using InterpolationMode = int;
using PixelOffsetMode = int;

constexpr Status Ok = 0;
constexpr Unit UnitPixel = 2;
constexpr InterpolationMode InterpolationModeHighQualityBicubic = 7;
constexpr InterpolationMode InterpolationModeNearestNeighbor = 5;
constexpr PixelOffsetMode PixelOffsetModeHalf = 4;
constexpr int SmoothingModeAntiAlias = 4;
constexpr int PixelFormat32bppRGB = 0x26200A;

struct ColorMatrix { float m[5][5]; };
struct Rect {
    Rect(int x, int y, int width, int height);
    int X, Y, Width, Height;
};
struct PointF {
    PointF(REAL x, REAL y);
    PointF();
    REAL X, Y;
};
struct Color { Color(BYTE red, BYTE green, BYTE blue); };
struct GdiplusStartupInput {};
using ::CLSID;
struct ImageCodecInfo { const wchar_t* MimeType; CLSID Clsid; };

class ImageAttributes {
public:
    void SetColorMatrix(const ColorMatrix* matrix);
};

class SolidBrush {
public:
    explicit SolidBrush(const Color& color);
};

class Pen {
public:
    Pen(const Color& color, REAL width);
};

class Bitmap {
public:
    Bitmap(unsigned width, unsigned height, int stride, int format, BYTE* scan0);
    static Bitmap* FromStream(void* stream);
    unsigned GetWidth() const;
    unsigned GetHeight() const;
    Status GetLastStatus() const;
    Status Save(const wchar_t* filename, const CLSID* encoder, const void* params);
};

class Graphics {
public:
    explicit Graphics(HDC dc);
    void SetInterpolationMode(InterpolationMode mode);
    void SetPixelOffsetMode(PixelOffsetMode mode);
    void SetSmoothingMode(int mode);
    void FillEllipse(SolidBrush* brush, REAL x, REAL y, REAL width, REAL height);
    void DrawEllipse(Pen* pen, REAL x, REAL y, REAL width, REAL height);
    void FillPolygon(SolidBrush* brush, PointF* points, int count);
    void DrawPolygon(Pen* pen, PointF* points, int count);
    void DrawImage(Bitmap* image, const Rect& destination, int source_x,
                   int source_y, int source_width, int source_height,
                   Unit unit, ImageAttributes* attributes);
    void DrawImage(Bitmap* image, const Rect& destination, int source_x,
                   int source_y, int source_width, int source_height,
                   Unit unit);
};

void GdiplusStartup(ULONG_PTR* token, const GdiplusStartupInput* input,
                    void* output);
void GdiplusShutdown(ULONG_PTR token);
void GetImageEncodersSize(UINT* count, UINT* size);
Status GetImageEncoders(UINT count, UINT size, ImageCodecInfo* codecs);

}
