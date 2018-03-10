// dllmain.cpp : 定義 DLL 應用程式的進入點。
#include "stdafx.h"
#include "MsCRC.h"
#include "MultiMs.h"
#include "MsPatch.h"

BOOL WINAPI Injected()
{
	//Waitting fo Maplestory.exe load
	Sleep(3000);
	//多開
	MultiClient();
	//HS Bypass
	MsCrcBypass();
	//Patch function
	No_Cap();
	return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(&Injected), NULL, 0, NULL);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

