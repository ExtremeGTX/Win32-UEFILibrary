#ifndef _UEFI_PARSER_H_
#define _UEFI_PARSER_H_

#include "uefi_types.h"

#define GET_ATTRIBUTES(buf)			 (UINT32) (buf[3]<<24 | buf[2]<<16 | buf[1]<<8 | buf[0])//(UINT32)buf[0]
#define GET_FILELISTLENGTH(buf)		 (UINT16) (buf[5]<<8 | buf[4])						  //(UINT16)buf[4]

#define DESCRIPTION_OFFSET(buf)			&buf[6]
#define GET_DESCRIPTION_LENGTH(buf)		wcslen((WCHAR*)DESCRIPTION_OFFSET(buf)) + 1
#define ALLOCATE_WCHAR_STRING(count)	(WCHAR*)calloc(count,sizeof(WCHAR));


#define FILEPATHLIST_OFFSET(buf,offset)		(UINT8*)(DESCRIPTION_OFFSET_PTR(buffer) + offset);


#define BUFFER_SIZE	4096

extern BDS_LOAD_OPTION* GetBootEntry(LPCWSTR BootEntry, UINT16 id);
extern void GetFilePathList(BDS_LOAD_OPTION* BdsLoadOption, char* buffer, int descSize);
extern BOOL ValidateBootEntry(UINT8* buffer);

#endif