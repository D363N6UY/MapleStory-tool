#pragma once
#include "stdafx.h"
DWORD No_CapAddr = 0x00B1CE2A;

void No_Cap() {
	*(BYTE*)No_CapAddr = 0x80;
	*(BYTE*)(No_CapAddr + 1 ) = 0xFF;
	*(BYTE*)(No_CapAddr + 2 ) = 0x64;
	*(BYTE*)(No_CapAddr + 4) = 0xCD;
	*(BYTE*)(No_CapAddr + 5) = 0xCD;
}