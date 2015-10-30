#include <stdafx.h>
#include <stdio.h>
#include "efiparser.h"
#include "EFIMacros.h"
#include "DevicePath.h"

BDS_LOAD_OPTION* GetBootEntry(LPCWSTR BootEntry, int id)
{
	int len;
	UINT8 buffer[BUFFER_SIZE];					//Block of bytes
	BDS_LOAD_OPTION* pBootEntry;

	len = GetFirmwareEnvironmentVariable(BootEntry, EFI_GLOBAL_VARIABLE, buffer, sizeof(char) * BUFFER_SIZE); //EFI Variables names are case sensitive

	if (!len)
		return NULL;

	//Alocate New BootDevice
	pBootEntry = (BDS_LOAD_OPTION*)calloc(1, sizeof(BDS_LOAD_OPTION));
	if (!pBootEntry)
		return NULL;

	//Update BootOption ID
	pBootEntry->LoadOptionIndex = id;

	//Get Attributes
	pBootEntry->Attributes = GET_ATTRIBUTES(buffer);

	//Get FilePathListLength
	pBootEntry->FilePathListLength = GET_FILELISTLENGTH(buffer);
	if (pBootEntry->FilePathListLength > len)
		return NULL;

	//Get Description
	int descSize = wstrlen((WCHAR*)DESCRIPTION_OFFSET(buffer));
	if (descSize == 0)
		return NULL;

	pBootEntry->Description = ALLOCATE_WCHAR_STRING(descSize);
	memcpy(pBootEntry->Description, DESCRIPTION_OFFSET(buffer), descSize);
	
	DBG_INFO("%s,%d : %s\n",BootEntry, id, pBootEntry->Description);
	//Get FilePathList
	pBootEntry->FilePathList = (EFI_DEVICE_PATH_PROTOCOL*)calloc(1, pBootEntry->FilePathListLength);
	if (!pBootEntry->FilePathList)
		return NULL;
	memcpy(pBootEntry->FilePathList, (EFI_DEVICE_PATH_PROTOCOL*)(DESCRIPTION_OFFSET(buffer) + descSize), pBootEntry->FilePathListLength);

	//UINT8 OptionalData = (UINT8)((UINT8*)pBootEntry - ((UINT8*)pBootEntry->FilePathList) + pBootEntry->FilePathListLength);
	//Read FilePathList
	//GetFilePathList(pBootEntry, buffer, descSize);

	return pBootEntry;
}


/* Currently this function is useless */
void GetFilePathList(BDS_LOAD_OPTION* BdsLoadOption, char* buffer, int descSize)
{
	if (BdsLoadOption->FilePathListLength <= 0)
		return;

	EFI_DEVICE_PATH_PROTOCOL* DevicePathNode = BdsLoadOption->FilePathList;

	// File path fields
	DevicePathNode = BdsLoadOption->FilePathList;
	if (DevicePathType(DevicePathNode) != MEDIA_DEVICE_PATH_TYPE)
		return;

	while (!IsDevicePathEndType(DevicePathNode))
	{
		switch (DevicePathSubType(DevicePathNode))
		{
		case HARDDRIVE_SUBTYPE:
			printf("HDD");
			break;

		case FILE_PATH_SUBTYPE:
			printf("FILE");
			break;
		}


		DevicePathNode = NextDevicePathNode(DevicePathNode);
	}
}



#define VALIDATE_ERR_NOTSUPPORTED_ATTRIBUTES	0xF1
#define VALIDATE_ERR_FILELISTLENGTH				0xF2
#define VALIDATE_ERR_DESCRIPTION				0xF3
#define VALIDATE_ERR_CORRUBTED_DEVPATHNODE		0xF4



BOOL ValidateBootEntry(UINT8* buffer)
{

	//char buffer[512];					//Block of bytes
	BDS_LOAD_OPTION* pBootEntry;

	if (!buffer)
		return FALSE;

	//Alocate New BootDevice
	pBootEntry = (BDS_LOAD_OPTION*)calloc(1, sizeof(BDS_LOAD_OPTION));
	if (!pBootEntry)
		return FALSE;

	//Get Attributes
	pBootEntry->Attributes = GET_ATTRIBUTES(buffer);

	//Get FilePathListLength
	pBootEntry->FilePathListLength = GET_FILELISTLENGTH(buffer);

	//Get Description
	int descSize = wstrlen((WCHAR*)DESCRIPTION_OFFSET(buffer));// GET_DESCRIPTION_LENGTH(buffer);
	if (descSize == 0)
		return FALSE;

	pBootEntry->Description = ALLOCATE_WCHAR_STRING(descSize);
	if (pBootEntry->Description == NULL)
		return FALSE;

	memcpy(pBootEntry->Description, DESCRIPTION_OFFSET(buffer), descSize);

	//Get FilePathList
	pBootEntry->FilePathList = (EFI_DEVICE_PATH_PROTOCOL*)calloc(1, pBootEntry->FilePathListLength);
	if (pBootEntry->FilePathList == NULL)
		return FALSE;

	memcpy(pBootEntry->FilePathList, (EFI_DEVICE_PATH_PROTOCOL*)(DESCRIPTION_OFFSET(buffer) + descSize), pBootEntry->FilePathListLength);

	//Read FilePathList
	//GetFilePathList(pBootEntry, buffer, descSize);

	if (pBootEntry->FilePathListLength <= 0)
		return FALSE;

	EFI_DEVICE_PATH_PROTOCOL* DevicePathNode = pBootEntry->FilePathList;

	// File path fields
	DevicePathNode = pBootEntry->FilePathList;
	if (DevicePathType(DevicePathNode) != MEDIA_DEVICE_PATH_TYPE)
		return FALSE;


	while (!IsDevicePathEndType(DevicePathNode)) {

		DevicePathNode = NextDevicePathNode(DevicePathNode);
	}

	GUID* pG;
	while (!IsDevicePathEndType(DevicePathNode))
	{
		if (DevicePathNode->Type != 0x7F)
			if (DevicePathNode->Type < 0x01 || DevicePathNode->Type > 0x05)
			{
				return FALSE;
			}
		switch (DevicePathSubType(DevicePathNode))
		{
		case HARDDRIVE_SUBTYPE:
			HARDDRIVE_DEVICE_PATH* hd;
			hd = (HARDDRIVE_DEVICE_PATH *)DevicePathNode;
#if 0
			wprintf(L"\t PartitionFormat:%d\n", hd->PartitionFormat);
			wprintf(L"\t Partition No:%d\n", hd->PartitionNumber);
			wprintf(L"\t Partition Size:%d\n", hd->PartitionSize);
			wprintf(L"\t Partition Start:%d\n", hd->PartitionStart);
			wprintf(L"\t Signature type:%d\n", hd->SignatureType);
			pG = (GUID*)&hd->Signature;
			LPOLESTR xds;
			_GUID y;
			StringFromCLSID(*pG, &xds);
			wprintf(L"\t Partition Signature:%s\n", xds);
#endif
			break;

		case FILE_PATH_SUBTYPE:
			FILEPATH_DEVICE_PATH* f;
			f = (FILEPATH_DEVICE_PATH*)DevicePathNode;
#if 0
			wprintf(L"\t PathName:%s\n", f->PathName);
#endif
			break;
		}


		DevicePathNode = NextDevicePathNode(DevicePathNode);
	}

	return TRUE;
}