#pragma once
// https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#include "basetsd.h"

#ifndef WINAPI
#define WINAPI
#endif
#ifndef CALLBACK
#define CALLBACK
#endif

#define VOID void
typedef void *PVOID;
typedef PVOID HANDLE;

typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint32_t ULONG;

typedef char CHAR;
typedef wchar_t WCHAR;
typedef int32_t LONG;
typedef int16_t SHORT;
typedef int64_t LONGLONG;
typedef uint64_t ULONGLONG;
typedef int32_t HRESULT;

#define __nullterminated
#define CONST const
typedef __nullterminated CONST CHAR *LPCSTR;
typedef __nullterminated CONST CHAR *LPCCH;
typedef __nullterminated CONST WCHAR *LPCWSTR;
typedef __nullterminated WCHAR *LPWSTR;
typedef CHAR *LPSTR;

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

// memory protection constants
// https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
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

#define MEM_COMMIT  0x00001000
#define MEM_RESERVE 0x00002000
#define MEM_RELEASE 0x00008000

typedef struct _MEMORY_BASIC_INFORMATION {
	PVOID BaseAddress;
	PVOID AllocationBase;
	DWORD AllocationProtect;
	SIZE_T RegionSize;
	DWORD State;
	DWORD Protect;
	DWORD Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

// Generic Access Rights
#define GENERIC_READ    0x80000000
#define GENERIC_WRITE   0x40000000

// File Sharing
#define FILE_SHARE_READ  0x00000001
#define FILE_SHARE_WRITE 0x00000002

// File Attributes
#define FILE_ATTRIBUTE_READONLY              0x00000001
#define FILE_ATTRIBUTE_HIDDEN                0x00000002
#define FILE_ATTRIBUTE_SYSTEM                0x00000004
#define FILE_ATTRIBUTE_DIRECTORY             0x00000010
#define FILE_ATTRIBUTE_ARCHIVE               0x00000020
#define FILE_ATTRIBUTE_DEVICE                0x00000040
#define FILE_ATTRIBUTE_NORMAL                0x00000080
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

// Thread Access Rights
#define THREAD_SUSPEND_RESUME            0x0002
#define THREAD_GET_CONTEXT               0x0008
#define THREAD_QUERY_INFORMATION         0x0040
#define THREAD_QUERY_LIMITED_INFORMATION 0x0800

#define IMAGE_FILE_MACHINE_I386 0x014c

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

// defined under ntdef.h
typedef struct _LIST_ENTRY {
	struct _LIST_ENTRY *Flink;
	struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, PRLIST_ENTRY;

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

VOID YieldProcessor();
