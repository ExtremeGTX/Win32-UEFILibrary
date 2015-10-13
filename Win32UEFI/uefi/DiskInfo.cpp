#include "stdafx.h"
#include "DiskInfo.h"

GPT_PARTITION* GetDiskInfo(const WCHAR* DiskLetter)
{
	//HANDLE hDevice = CreateFile(L"\\\\.\\E:", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	HANDLE hDevice = CreateFile(DiskLetter, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	BOOL ok;
	if (hDevice == INVALID_HANDLE_VALUE)
		return NULL;

	PARTITION_INFORMATION_EX partInfo;
	DWORD BytesReturned;
	ok = DeviceIoControl(
		(HANDLE)hDevice,					 // handle to a partition
		IOCTL_DISK_GET_PARTITION_INFO_EX,	 // dwIoControlCode
		NULL,								 // lpInBuffer
		0,									 // nInBufferSize
		(LPVOID)&partInfo,					 // output buffer
		(DWORD)sizeof(partInfo),			 // size of output buffer
		(LPDWORD)&BytesReturned,			 // number of bytes returned
		(LPOVERLAPPED)NULL					 // OVERLAPPED structure
		);

	if (partInfo.PartitionStyle != PARTITION_STYLE_GPT)
		return NULL;

	GPT_PARTITION* gptp = (GPT_PARTITION*)malloc(sizeof(GPT_PARTITION));
	gptp->PartitionNumber = partInfo.PartitionNumber;
	gptp->PartitionId = partInfo.Gpt.PartitionId;
	gptp->PartitionLength = (partInfo.PartitionLength.QuadPart / 512);
	gptp->StartingOffset = (partInfo.StartingOffset.QuadPart / 512);

	//MakeNewEntry(gptp);
	return gptp;

}