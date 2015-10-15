#ifndef _HELPER_FUNCTIONS_H_
#define _HELPER_FUNCTIONS_H_

#include "efi_common.h"

extern BOOL isUEFIAvailable();
extern int wstrlen(const WCHAR* str);
extern BOOL FindItem(UINT16* arr, UINT16 val, UINT16 length);
#endif