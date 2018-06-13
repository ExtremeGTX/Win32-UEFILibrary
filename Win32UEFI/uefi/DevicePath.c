#include "DevicePath.h"
#include "DiskInfo.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

HARDDRIVE_DEVICE_PATH* BuildHardDrivePath(const WCHAR* DiskLetter)
{
	HARDDRIVE_DEVICE_PATH* hd = NULL;
	GPT_PARTITION* gpt = NULL;

	hd = (HARDDRIVE_DEVICE_PATH*)calloc(1, sizeof(HARDDRIVE_DEVICE_PATH));
	if (hd == NULL)
		return NULL;

	gpt = GetDiskInfo(DiskLetter);
	if (gpt == NULL)
		return NULL;

	/* Fill Header Info */
	hd->Header.Type = MEDIA_DEVICE_PATH_TYPE;
	hd->Header.SubType = HARDDRIVE_SUBTYPE;
	hd->Header.Length[0] = 42;
	hd->Header.Length[1] = 0;

	/* Fill Partition Specific info */
	hd->PartitionNumber = gpt->PartitionNumber;
	hd->PartitionStart = gpt->StartingOffset;
	hd->PartitionSize = gpt->PartitionLength;
	hd->PartitionFormat = 0x02;
	hd->SignatureType = 0x02;
	memcpy(&hd->Signature, &gpt->PartitionId, 16);

	return hd;
}

FILEPATH_DEVICE_PATH* BuildFilePath(const WCHAR* Path)
{
	FILEPATH_DEVICE_PATH* fd;
	UINT8 PathLen;


	fd = (FILEPATH_DEVICE_PATH*)calloc(1, sizeof(FILEPATH_DEVICE_PATH));
	if (fd == NULL)
		return NULL;

	PathLen = wstrlen(Path);
	fd->Header.Type = MEDIA_DEVICE_PATH_TYPE;
	fd->Header.SubType = FILE_PATH_SUBTYPE;
	fd->Header.Length[0] = sizeof(EFI_DEVICE_PATH_PROTOCOL) + PathLen;
	fd->Header.Length[1] = 0;

	return fd;

}
EFI_DEVICE_PATH_PROTOCOL* BuildDevicePathEnd()
{
	EFI_DEVICE_PATH_PROTOCOL* ed;
	ed = (EFI_DEVICE_PATH_PROTOCOL*)calloc(1, sizeof(EFI_DEVICE_PATH_PROTOCOL));
	if (ed == NULL)
		return NULL;

	ed->Length[0] = 4;
	ed->Length[1] = 0;
	ed->Type = 0x7f;
	ed->SubType = 0xff;

	return ed;
}