#include "stdafx.h"

#include "EFIFunctions.h"
#include "DevicePath.h"
#include "efiparser.h"

#define EFI_BOOT_LIST_LEN 20
BDS_LOAD_OPTION** _BootOptions;
EFI_BOOT_ORDER	 _BootOrder[EFI_BOOT_LIST_LEN];
static int BootCount = 0;

BOOL UpdateBootOrder();
int GetNewBootOptionID();

BOOL efi_init()
{
	_BootOptions = (BDS_LOAD_OPTION **)calloc(BootCount, sizeof(BDS_LOAD_OPTION **));

	if (_BootOptions == NULL)
	{
		return false;
	}

	return true;
}

EFI_BOOT_ORDER* GetBootList()
{
	int len = 0;
	len = GetFirmwareEnvironmentVariable(L"BootOrder", EFI_GLOBAL_VARIABLE, &_BootOrder, sizeof(EFI_BOOT_ORDER) * EFI_BOOT_LIST_LEN); //EFI Variables names are case sensitive

	if (!len)
	{
		return NULL;
	}

	BootCount = len / sizeof(UINT16);	/* Get Number of available boot options */

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
			return NULL;
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
			printf("Error Removing param: ERROR_ENVVAR_NOT_FOUND\n");
			break;
		default:
			printf("Error Removing param: %d\n", GetLastError());
		}
	}


	WCHAR* p = (WCHAR*)_BootOrder;
	for (int i = 0; i < BootCount; i++)
	{
		if (_BootOrder[i] == id)
		{
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
		return false;
	}
	return true;
}



BOOL MakeMediaBootOption(UINT32 Attributes, WCHAR* Description, WCHAR* DiskLetter, WCHAR* Path)
{

	int byteCounter = 0;
	UINT8 EFIbuffer[512];

	EFI_LOAD_OPTION NewEntry;

	/* Init structs */
	//memset(EFIbuffer, 0x00, 512);
	//memset(&NewEntry, 0x00, sizeof(BDS_LOAD_OPTION));




	/* Build File Path list */
	HARDDRIVE_DEVICE_PATH* hd = BuildHardDrivePath(DiskLetter);
	FILEPATH_DEVICE_PATH*  fd = BuildFilePath(Path);
	EFI_DEVICE_PATH_PROTOCOL* ed = BuildDevicePathEnd();

	if (hd == NULL || fd == NULL || ed == NULL)
		return false;

	/* Build Option Header */
	NewEntry.Attributes = LOAD_OPTION_ACTIVE;
	NewEntry.FilePathListLength = sizeof(HARDDRIVE_DEVICE_PATH) /* HDD */
		+ sizeof(EFI_DEVICE_PATH_PROTOCOL)	/* FilePath */
		+ wstrlen(Path)	/* EFI File path */
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
		return false;
	}

	int BootID = GetNewBootOptionID();
	if (BootID == -1)
		return false;

	WCHAR BootEntry[10] = L"BootFFFF";		/* change #### with 0001,0002, etc. 8 char + 1 terminator + 1 dummy (useless) */
	swprintf_s(BootEntry, 10, L"Boot%04X", BootID);


	int ret;
	ret = SetFirmwareEnvironmentVariable(BootEntry, EFI_GLOBAL_VARIABLE, EFIbuffer, byteCounter);
	if (ret == 0)
	{
		return false;
	}

	_BootOrder[BootCount] = BootID;
	BootCount++;
	if (!UpdateBootOrder())
		return false;



	return true;
}

int GetNewBootOptionID()
{
	int BootOptionID = 0;
	while (FindItem(_BootOrder, BootOptionID, BootCount))
	{
		BootOptionID++;
	}
	

	if (BootOptionID >= EFI_BOOT_LIST_LEN)
		return -1;
	else
		return BootOptionID;
}