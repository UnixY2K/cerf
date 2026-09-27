#pragma once
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

typedef intptr_t INT_PTR, *PINT_PTR;
typedef uintptr_t UINT_PTR, *PUINT_PTR;
typedef intptr_t LONG_PTR, *PLONG_PTR;
typedef uintptr_t ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
typedef ULONG_PTR SIZE_T, *PSIZE_T;
typedef intptr_t SSIZE_T, *PSSIZE_T;

typedef uint64_t DWORD64, *PDWORD64;
typedef int64_t LONG64, *PLONG64;
typedef uint64_t ULONG64, *PULONG64;

#ifndef __int32
#define __int32 int
#endif
