#pragma once
#include "stdafx.h"
#include <Windows.h>

#define jmp(frm, to) (int)(((int)to - (int)frm) - 5)


DWORD  ulMsCrcStart = 0x00401000; //static
DWORD  ulMsCrcEnd = ulMsCrcStart; //modify as needed
void* Allocation;


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

void MsCrcBypass()
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((void*)ulMsCrcStart, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
	ulMsCrcEnd += mbi.RegionSize;

	for (unsigned long ulMsCrc = ulMsCrcStart; ulMsCrc < ulMsCrcEnd; ulMsCrc++)
	{
		if (*(unsigned long*)ulMsCrc == 0x8B09B60F) //0F B6 09 8B
		{
			//Allocate some space for the unmodified memory
			Allocation = VirtualAlloc(NULL, ulMsCrcEnd - ulMsCrcStart, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			//Unprotect MS' memory
			unsigned long ulProtect;
			VirtualProtect((void*)ulMsCrcStart, ulMsCrcEnd - ulMsCrcStart, PAGE_EXECUTE_READWRITE, &ulProtect);

			//Copy the original, unedited memory
			memcpy(Allocation, (void*)ulMsCrcStart, ulMsCrcEnd - ulMsCrcStart);

			//inline call
			*(BYTE*)ulMsCrc = 0xE8;
			*(DWORD*)(ulMsCrc + 1) = jmp(ulMsCrc, MsCrcAsm);
			*(BYTE*)(ulMsCrc + 5) = 0x90;

			//Skip HS
			*(BYTE*)0x00A11996 = 0xEB;
			*(BYTE*)0x00A6D7F6 = 0xC3;
			*(BYTE*)0x00A6D834 = 0xC3;
			return;
		}
	}
}