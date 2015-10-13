#include "stdafx.h"
#include "HelperFunctions.h"


BOOL isUEFIAvailable()
{
	UINT32 buffer;
	GetFirmwareEnvironmentVariable(L"", EFI_DUMMY_GUID, &buffer, sizeof(UINT32));
	if (GetLastError() == ERROR_INVALID_FUNCTION)
	{
		return FALSE;
	}

	return TRUE;
}

int wstrlen(const WCHAR* str)
{
	//return (WCHAR String Length) + (null-terminating char) in bytes 
	return (wcslen(str) + 1) * 2;
}

