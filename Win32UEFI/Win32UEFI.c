// Win32UEFI.cpp : Defines the exported functions for the DLL application.
//

#include "Win32UEFI.h"

#include <Windows.h>

// Defines the entry point for the DLL application.
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


WIN32UEFIFUNC_API BOOL UEFI_Init()
{
	
	DBG_INIT();

	if (GetBootList() == NULL)
		return FALSE;

	if (!efi_init())
		return FALSE;

	if (GetBootDevices() == NULL)
		return FALSE;

	return TRUE;
}

WIN32UEFIFUNC_API void UEFI_MakeMediaBootOption(WCHAR* Description, WCHAR* DiskLetter, WCHAR* Path)
{
	DBG_INFO("===New BootOption===\n");
	MakeMediaBootOption(LOAD_OPTION_ACTIVE, Description, DiskLetter, Path);
}

WIN32UEFIFUNC_API void UEFI_DeleteBootOption()
{

}

WIN32UEFIFUNC_API int UEFI_DeleteBootOptionByDescription(WCHAR* Description)
{
	DBG_INFO("===Delete BootOptionBD===\n");
	return DeleteBootOptionByDescription(Description);
}


WIN32UEFIFUNC_API EFI_BOOT_ORDER* UEFI_GetBootList()
{
	return GetBootList();
}

WIN32UEFIFUNC_API BDS_LOAD_OPTION** UEFI_GetBootDevices()
{
	return GetBootDevices();
}

WIN32UEFIFUNC_API int UEFI_GetBootCount()
{
	return GetBootCount();
}

WIN32UEFIFUNC_API BOOL UEFI_isUEFIAvailable()
{
	return isUEFIAvailable();
}


