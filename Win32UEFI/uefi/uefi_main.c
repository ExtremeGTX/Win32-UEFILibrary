#include "uefi_main.h"
#include "DevicePath.h"
#include "uefi_parser.h"
#include "common.h"

#include <stdio.h>
#include <wchar.h>
#include <windows.h>

#define EFI_BOOT_LIST_LEN 20
BDS_LOAD_OPTION**	_BootOptions;
EFI_BOOT_ORDER		_BootOrder[EFI_BOOT_LIST_LEN];
static UINT16		BootCount = 0;

BOOL UpdateBootOrder();
UINT16 GetNewBootOptionID();

BOOL efi_init()
{
	_BootOptions = (BDS_LOAD_OPTION **)calloc(BootCount, sizeof(BDS_LOAD_OPTION **));

	if (_BootOptions == NULL)
	{
		DBG_INFO("Failed to Allocate Bootlist\n");
		return FALSE;
	}

	return TRUE;
}

EFI_BOOT_ORDER* GetBootList()
{
	UINT16 len = 0;
	len = (UINT16)GetFirmwareEnvironmentVariable(L"BootOrder", EFI_GLOBAL_VARIABLE, &_BootOrder, sizeof(EFI_BOOT_ORDER) * EFI_BOOT_LIST_LEN); //EFI Variables names are case sensitive

	if (!len)
	{
		DBG_INFO("Failed to Read Bootlist\n");
		return NULL;
	}

	BootCount = len / sizeof(UINT16);	/* Get Number of available boot options */
	DBG_INFO("BootCount: %u\n", BootCount);

	return _BootOrder;
}

BDS_LOAD_OPTION** GetBootDevices()
{

	wchar_t BootEntry[10] = L"Boot####";		/* change #### with 0001,0002, etc. 8 char + 1 terminator + 1 dummy (useless) */

	for (UINT16 i = 0; i < BootCount; i++)
	{
		swprintf_s(BootEntry, 10, L"Boot%04X", _BootOrder[i]);
		_BootOptions[i] = GetBootEntry(BootEntry, i);

		if (_BootOptions[i] == NULL)
		{
			DBG_INFO("Fail to Read BootOptions, %d\n",i);
			return NULL;
		}
	}
	return _BootOptions;
}

int GetBootCount()
{
	return BootCount;
}

int DeleteBootOptionByDescription(WCHAR* Description)
{
	BDS_LOAD_OPTION *p;

	for (int i = 0; i < BootCount; i++)
	{
		p = _BootOptions[i];

		if (wcscmp(Description, p->Description) == 0)
		{
			DBG_INFO("Delete BootOption ID:%d Entry:%d\n", _BootOrder[i],i);
			DeleteBootOption(_BootOrder[i]);
			return _BootOrder[i];
		}
	}
	return -1;
}

void DeleteBootOption(int id)
{
	UINT8 dummy;
	WCHAR BootEntry[10] = L"BootFFFF";		/* change #### with 0001,0002, etc. 8 char + 1 terminator + 1 dummy (useless) */
	swprintf_s(BootEntry, 10, L"Boot%04X", id);
	int ret;
	ret = SetFirmwareEnvironmentVariable(BootEntry, EFI_GLOBAL_VARIABLE, &dummy, 0);
	if (ret <= 0)
	{
		switch (GetLastError()){
		case ERROR_ENVVAR_NOT_FOUND:
			DBG_INFO("Error Removing param: ERROR_ENVVAR_NOT_FOUND\n");
			break;
		default:
			DBG_INFO("Error Removing param: %d\n", GetLastError());
			break;
		}
	}


	WCHAR* p = (WCHAR*)_BootOrder;
	for (UINT16 i = 0; i < BootCount; i++)
	{
		if (_BootOrder[i] == id)
		{
			DBG_INFO("Entry Found at: %d\n", i);
			wmemcpy(&p[i], &p[i + 1], (BootCount - 1) - i);
			BootCount--;
			wmemset(&p[BootCount], 0x00, EFI_BOOT_LIST_LEN - BootCount);
		}
	}


	UpdateBootOrder();
}

BOOL UpdateBootOrder()
{
	int ret = SetFirmwareEnvironmentVariable(L"BootOrder", EFI_GLOBAL_VARIABLE, _BootOrder, BootCount * 2);
	if (ret == 0)
	{
		return FALSE;
	}
	return TRUE;
}



BOOL MakeMediaBootOption(UINT32 Attributes, WCHAR* Description, WCHAR* DiskLetter, WCHAR* Path)
{

	int byteCounter = 0;
	UINT8 EFIbuffer[512];

	EFI_LOAD_OPTION NewEntry;

	/* Build File Path list */
	HARDDRIVE_DEVICE_PATH* hd = BuildHardDrivePath(DiskLetter);
	FILEPATH_DEVICE_PATH*  fd = BuildFilePath(Path);
	EFI_DEVICE_PATH_PROTOCOL* ed = BuildDevicePathEnd();

	if (hd == NULL || fd == NULL || ed == NULL)
	{
		DBG_INFO("Building FilePathList failed\n");
		return FALSE;
	}
	/* Build Option Header */
	Attributes = LOAD_OPTION_ACTIVE;
	NewEntry.Attributes = Attributes;
	NewEntry.FilePathListLength = sizeof(HARDDRIVE_DEVICE_PATH)		/* HDD */
								+ sizeof(EFI_DEVICE_PATH_PROTOCOL)	/* FilePath */
								+ wstrlen(Path)						/* EFI File path */
								+ sizeof(EFI_DEVICE_PATH_PROTOCOL); /* EndNode */



	/*
	 * Build EFI Buffer
	 */


	/* Add Option Header */
	memcpy(&EFIbuffer[byteCounter], &NewEntry, sizeof(EFI_LOAD_OPTION));
	byteCounter += sizeof(EFI_LOAD_OPTION);

	/* Add Description */
	memcpy(&EFIbuffer[byteCounter], Description, wstrlen(Description));
	byteCounter += wstrlen(Description);
	/*#### Add FileList ####*/

	/* Add HD Path */
	memcpy(&EFIbuffer[byteCounter], hd, sizeof(HARDDRIVE_DEVICE_PATH));
	byteCounter += sizeof(HARDDRIVE_DEVICE_PATH);

	/* Add File Path*/
	memcpy(&EFIbuffer[byteCounter], fd, sizeof(EFI_DEVICE_PATH_PROTOCOL));
	byteCounter += sizeof(EFI_DEVICE_PATH_PROTOCOL);

	/* Add EFI Path */
	memcpy(&EFIbuffer[byteCounter], Path, wstrlen(Path));
	byteCounter += wstrlen(Path);

	/* Add Device End node */
	memcpy(&EFIbuffer[byteCounter], ed, sizeof(EFI_DEVICE_PATH_PROTOCOL));
	byteCounter += sizeof(EFI_DEVICE_PATH_PROTOCOL);


	if (!ValidateBootEntry(EFIbuffer))
	{
		DBG_INFO("Validation Error\n");
		return FALSE;
	}

	UINT16 BootID = GetNewBootOptionID();
	if (BootID == -1)
	{
		DBG_INFO("BOOT_ID out of bounds\n");
		return FALSE;
	}
	WCHAR BootEntry[10] = L"BootFFFF";		/* change #### with 0001,0002, etc. 8 char + 1 terminator + 1 dummy (useless) */
	swprintf_s(BootEntry, 10, L"Boot%04X", BootID);


	int ret;
	ret = SetFirmwareEnvironmentVariable(BootEntry, EFI_GLOBAL_VARIABLE, EFIbuffer, byteCounter);
	if (ret == 0)
	{
		DBG_INFO("Write Boot Entry Fail\n");
		return FALSE;
	}

	_BootOrder[BootCount] = BootID;
	BootCount++;
	if (!UpdateBootOrder())
	{
		DBG_INFO("Update Fail\n");
		return FALSE;
	}

	DBG_INFO("NewEntry:%s,%u %s\n", BootEntry, (BootCount-1), Description);
	return TRUE;
}

UINT16 GetNewBootOptionID()
{
	UINT16 BootOptionID = 0;
	while (FindItem(_BootOrder, BootOptionID, BootCount))
	{
		BootOptionID++;
	}
	

	if (BootOptionID >= EFI_BOOT_LIST_LEN)
		return BOOT_OPTION_MAX_LIMIT;
	else
		return BootOptionID;
}