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

BOOL FindItem(UINT16* arr, UINT16 val, UINT16 length)
{
	for (int i = 0; i < length; i++)
	{
		if (val == arr[i])
		{
			return true;
		}
	}

	return false;
}