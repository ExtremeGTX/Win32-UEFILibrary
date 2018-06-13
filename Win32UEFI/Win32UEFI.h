#pragma once

#include "uefi\uefi.h"
#include "dbg.h"


#define WIN32UEFIFUNC_EXPORTS
#define _DEBUG_INFO

#ifdef WIN32UEFIFUNC_EXPORTS
#define WIN32UEFIFUNC_API __declspec(dllexport) 
#else
#define WIN32UEFIFUNC_API __declspec(dllimport) 
#endif



WIN32UEFIFUNC_API BOOL UEFI_Init();
WIN32UEFIFUNC_API void UEFI_MakeMediaBootOption(WCHAR* Description, WCHAR* DiskLetter, WCHAR* Path);
WIN32UEFIFUNC_API void UEFI_DeleteBootOption();
WIN32UEFIFUNC_API int UEFI_DeleteBootOptionByDescription(WCHAR* Description);
WIN32UEFIFUNC_API EFI_BOOT_ORDER* UEFI_GetBootList();
WIN32UEFIFUNC_API BDS_LOAD_OPTION** UEFI_GetBootDevices();
WIN32UEFIFUNC_API int UEFI_GetBootCount();
WIN32UEFIFUNC_API BOOL UEFI_isUEFIAvailable();
