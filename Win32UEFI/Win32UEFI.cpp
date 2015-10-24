// Win32UEFI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#ifdef WIN32UEFIFUNC_EXPORTS
#define WIN32UEFIFUNC_API __declspec(dllexport) 
#else
#define WIN32UEFIFUNC_API __declspec(dllimport) 
#endif

EXTERN_C{
	WIN32UEFIFUNC_API BOOL UEFI_Init();
	WIN32UEFIFUNC_API void UEFI_MakeMediaBootOption(WCHAR* Description, WCHAR* DiskLetter, WCHAR* Path);
	WIN32UEFIFUNC_API void UEFI_DeleteBootOption();
	WIN32UEFIFUNC_API int UEFI_DeleteBootOptionByDescription(WCHAR* Description);
	WIN32UEFIFUNC_API EFI_BOOT_ORDER* UEFI_GetBootList();
	WIN32UEFIFUNC_API BDS_LOAD_OPTION** UEFI_GetBootDevices();
	WIN32UEFIFUNC_API int UEFI_GetBootCount();
	WIN32UEFIFUNC_API BOOL UEFI_isUEFIAvailable();
	
}

WIN32UEFIFUNC_API BOOL UEFI_Init()
{
	
	DBG_INIT();

	if (GetBootList() == NULL)
		return false;

	if (!efi_init())
		return false;

	if (GetBootDevices() == NULL)
		return false;

	return true;
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


