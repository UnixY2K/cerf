#pragma once
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

typedef struct _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} GUID;

typedef struct _CLSID {
    unsigned char data[16];
} CLSID;
