#pragma once
#include "windows.h"

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

#define AddrModeFlat 3
#define IMAGE_FILE_MACHINE_I386 0x014c

BOOL SymInitialize(HANDLE hProcess, LPCSTR UserSearchPath, BOOL fInvadeProcess);
BOOL SymFromAddr(HANDLE hProcess, DWORD64 Address, DWORD64 *Displacement,
                 PSYMBOL_INFO Symbol);
BOOL SymCleanup(HANDLE hProcess);
LPVOID SymFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase);
DWORD64 SymGetModuleBase64(HANDLE hProcess, DWORD64 AddrBase);

typedef BOOL(WINAPI *PREAD_PROCESS_MEMORY_ROUTINE64)(HANDLE, DWORD64, LPVOID,
                                                     DWORD, LPDWORD);
typedef LPVOID(WINAPI *PFUNCTION_TABLE_ACCESS_ROUTINE64)(HANDLE, DWORD64);
typedef DWORD64(WINAPI *PGET_MODULE_BASE_ROUTINE64)(HANDLE, DWORD64);

BOOL StackWalk64(DWORD MachineType, HANDLE hProcess, HANDLE hThread,
                 LPSTACKFRAME64 StackFrame, LPVOID ContextRecord,
                 PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
                 PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
                 PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
                 LPVOID TranslateAddress);
USHORT CaptureStackBackTrace(DWORD FramesToSkip, DWORD FramesToCapture,
                             LPVOID *BackTrace, DWORD *BackTraceHash);
