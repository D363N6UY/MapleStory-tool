#pragma once
#include "stdafx.h"

void No_Cap() {
	#if MsVer == 113
		DWORD No_CapAddr = 0x00B1CE28;
		*(double*)No_CapAddr = 300000.0;
	#elif MsVer == 120
		double *DamageCap = new double(300000.00);
		DWORD v120No_CapAddr1 = 0x00867A6C;
		*(DWORD*)(v120No_CapAddr1) = *(DWORD*)(&DamageCap);
	#endif
}
