#pragma once
#include "stdafx.h"

void No_Cap() {
	#if MsVer == 113
		DWORD No_CapAddr = 0x00B1CE28;
		*(double*)No_CapAddr = 300000.0;
	#elif MsVer == 120
		double *DamageCap = new double(300000.00);
		DWORD v120No_CapAddr1 = 0x00867A6C;
		DWORD v120No_CapAddr2 = 0x00867CD6;
		DWORD v120No_CapAddr3 = 0x00868D0E;
		DWORD v120No_CapAddr4 = 0x00869D41;
		DWORD v120No_CapAddr5 = 0x0086A213;
		DWORD v120No_CapAddr6 = 0x0086A463;
		*(DWORD*)(v120No_CapAddr1) = *(DWORD*)(&DamageCap);
		*(DWORD*)(v120No_CapAddr2) = *(DWORD*)(&DamageCap);
		*(DWORD*)(v120No_CapAddr3) = *(DWORD*)(&DamageCap);
		*(DWORD*)(v120No_CapAddr4) = *(DWORD*)(&DamageCap);
		*(DWORD*)(v120No_CapAddr5) = *(DWORD*)(&DamageCap);
		*(DWORD*)(v120No_CapAddr6) = *(DWORD*)(&DamageCap);
	#endif
}
