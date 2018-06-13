#ifndef _UEFI_MAIN_H_
#define _UEFI_MAIN_H_

#include "uefi_types.h"

extern BOOL efi_init();
extern EFI_BOOT_ORDER* GetBootList();
extern BDS_LOAD_OPTION** GetBootDevices();
extern int GetBootCount();

extern BOOL MakeMediaBootOption(UINT32 Attributes, WCHAR* Description, WCHAR* DiskLetter, WCHAR* Path);
extern void DeleteBootOption(int id);
extern int  DeleteBootOptionByDescription(WCHAR* Description);
#endif