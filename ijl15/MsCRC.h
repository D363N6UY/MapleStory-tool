#pragma once
#include "stdafx.h"
#include <Windows.h>
// 在此修改版本
#define MsVer 120

DWORD ulMsCrcStart = 0x00401000; // static
DWORD ulMsCrcEnd = ulMsCrcStart; // modify as needed
void* Allocation;
#define jmp(frm, to) (int)(((int)to - (int)frm) - 5)

#if MsVer == 113
#define CrcAddress 0x00A11481
void __declspec(naked) MsCrcAsm()
{
	__asm
	{
			cmp ecx, [ulMsCrcStart]
			jb Normal
			cmp ecx, [ulMsCrcEnd]
			ja Normal
			sub ecx, [ulMsCrcStart]
			add ecx, Allocation

		Normal :
			movzx ecx, byte ptr[ecx]
			mov edx, [ebp + 0x14]
			ret
	}
}
#elif MsVer == 120
#define CrcAddress 0x00B84329
#define CrcAddress2 0x00B84612
void __declspec(naked) MsCrcAsm()
{
	__asm
	{
			push eax
			lea eax, [esi + edx * 4]
			cmp eax, [ulMsCrcStart]
			jb Normal
			cmp eax, [0x00C01000]
			jae Normal
			mov esi, Allocation

	Normal :
			pop eax
			mov ecx,[ecx]
			xor ecx,[esi+edx*4]
			ret
	}
}
#endif


void MsCrcBypass()
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((void*)ulMsCrcStart, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	ulMsCrcEnd += mbi.RegionSize;
	//Allocate some space for the unmodified memory
	Allocation = VirtualAlloc(NULL, ulMsCrcEnd - ulMsCrcStart, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//Unprotect MS' memory
	unsigned long ulProtect;
	VirtualProtect((void*)ulMsCrcStart, ulMsCrcEnd - ulMsCrcStart, PAGE_EXECUTE_READWRITE, &ulProtect);
	
	//Copy the original, unedited memory
	memcpy(Allocation, (void*)ulMsCrcStart, ulMsCrcEnd - ulMsCrcStart);

	//inline call
#if MsVer == 113
	*(unsigned char*)CrcAddress = 0xE8;
	*(unsigned long*)(CrcAddress + 1) = jmp(CrcAddress, MsCrcAsm);
	*(unsigned char*)(CrcAddress + 5) = 0x90;
#elif MsVer == 120
	* (unsigned char*)CrcAddress = 0xE8;
	*(unsigned long*)(CrcAddress + 1) = jmp(CrcAddress, MsCrcAsm);
	*(unsigned char*)CrcAddress2 = 0xE8;
	*(unsigned long*)(CrcAddress2 + 1) = jmp(CrcAddress2, MsCrcAsm);
#endif
	//Skip HS
	//74 21 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8
	//v113
#if MsVer == 113
	*(unsigned char*)0x00A11996 = 0xEB;
	*(unsigned char*)0x00A6D7F6 = 0xC3;
	*(unsigned char*)0x00A6D834 = 0xC3;
	//v120
#elif MsVer == 120
	*(unsigned char*)0x00B84EB9 = 0xEB;
	*(unsigned char*)0x00BEDE16 = 0xC3;
	*(unsigned char*)0x00BEDDD8 = 0xC3;
#endif
	return;
}

