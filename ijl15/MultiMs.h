#include "stdafx.h"
#include <winternl.h>
#include <cstdlib>
bool MultiClient() {

	struct SYSTEM_HANDLE_INFORMATION {
		DWORD pid;
		BYTE type;
		BYTE flags;
		WORD handle;
		DWORD object;
		DWORD access;
	};

	typedef struct _OBJECT_NAME_INFORMATION {
		UNICODE_STRING Name;
	}OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

	typedef NTSTATUS(NTAPI *NtQuerySystemInformation_t) (SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
	typedef NTSTATUS(NTAPI *NtQueryObject_t) (HANDLE Handle, OBJECT_INFORMATION_CLASS ObjectInformationClass, PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength);


	int handleCount = 1024;

	HMODULE ntdll = GetModuleHandleA("ntdll");

	if (ntdll == NULL) return false;

	NtQueryObject_t ntQO = (NtQueryObject_t)GetProcAddress(ntdll, "NtQueryObject");
	NtQuerySystemInformation_t ntQSI = (NtQuerySystemInformation_t)GetProcAddress(ntdll, "NtQuerySystemInformation");

	if (ntQO == NULL || ntQSI == NULL) return false;

	struct HandleInfoSet {
		unsigned int count;
		SYSTEM_HANDLE_INFORMATION handles[1];
	};

	HandleInfoSet *infoSet = (HandleInfoSet *)malloc(4 + sizeof(SYSTEM_HANDLE_INFORMATION) * handleCount);
	DWORD length;

	NTSTATUS status = ntQSI((SYSTEM_INFORMATION_CLASS)16, infoSet, 4 + sizeof(SYSTEM_HANDLE_INFORMATION) * handleCount, &length);

	while (status == 0xc0000004 /*STATUS_INFO_LENGTH_MISMATCH*/) {
		free(infoSet);
		handleCount *= 2;
		infoSet = (HandleInfoSet *)malloc(4 + sizeof(SYSTEM_HANDLE_INFORMATION) * handleCount);
		status = ntQSI((SYSTEM_INFORMATION_CLASS)16, infoSet, 4 + sizeof(SYSTEM_HANDLE_INFORMATION) * handleCount, &length);
	}


	if (status != 0 /*STATUS_SUCCESS*/) {
		free(infoSet);
		return false;
	}


	for (unsigned int i = 0; i < infoSet->count; i++) {
		wchar_t nameBuffer[1024];
		status = ntQO((HANDLE)infoSet->handles[i].handle, (OBJECT_INFORMATION_CLASS)1, nameBuffer, 1024, &length);
		if (status != 0 /*STATUS_SUCCESS*/) continue;

		if (wcsstr(nameBuffer + 4, L"WvsClientMtx") != NULL) {
			HANDLE handle;
			DuplicateHandle(GetCurrentProcess(), (HANDLE)infoSet->handles[i].handle, 0, &handle, 0, FALSE, DUPLICATE_CLOSE_SOURCE);
			CloseHandle(handle);
			break;
		}
	}

	free(infoSet);
	return true;
}