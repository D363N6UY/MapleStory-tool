// ijl15.cpp : 定義 DLL 應用程式的匯出函式。
//

#include "stdafx.h"


HMODULE SeData_Base = LoadLibrary(L"ori_ijl15.dll");
DWORD ijlErrorStr_Proc = (DWORD)GetProcAddress(SeData_Base, "ijlErrorStr");
DWORD ijlFree_Proc = (DWORD)GetProcAddress(SeData_Base, "ijlFree");
DWORD ijlGetLibVersion_Proc = (DWORD)GetProcAddress(SeData_Base, "ijlGetLibVersion");
DWORD ijlInit_Proc = (DWORD)GetProcAddress(SeData_Base, "ijlInit");
DWORD ijlRead_Proc = (DWORD)GetProcAddress(SeData_Base, "ijlRead");
DWORD ijlWrite_Proc = (DWORD)GetProcAddress(SeData_Base, "ijlWrite");



extern "C" __declspec(dllexport) void __stdcall ijlErrorStr() { __asm { jmp dword ptr[ijlErrorStr_Proc]		} }
extern "C" __declspec(dllexport) void __stdcall ijlFree() { __asm { jmp dword ptr[ijlFree_Proc] 			} }
extern "C" __declspec(dllexport) void __stdcall ijlGetLibVersion() { __asm { jmp dword ptr[ijlGetLibVersion_Proc] 	} }
extern "C" __declspec(dllexport) void __stdcall ijlInit() { __asm { jmp dword ptr[ijlInit_Proc] 			} }
extern "C" __declspec(dllexport) void __stdcall ijlRead() { __asm { jmp dword ptr[ijlRead_Proc] 			} }
extern "C" __declspec(dllexport) void __stdcall ijlWrite() { __asm { jmp dword ptr[ijlWrite_Proc] 			} }