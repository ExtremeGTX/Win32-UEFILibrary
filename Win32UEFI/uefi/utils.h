#ifndef _UTILS_H_
#define _UTILS_H_

#include "types.h"

extern BOOL isUEFIAvailable();
extern UINT8 wstrlen(const WCHAR* str);
extern BOOL FindItem(UINT16* arr, UINT16 val, UINT16 length);
#endif