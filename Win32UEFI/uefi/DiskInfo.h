#ifndef _DISK_INFO_H_
#define _DISK_INFO_H_

#include "types.h"
#include <guiddef.h>

typedef struct {
	UINT64	StartingOffset;
	UINT64	PartitionLength;
	DWORD	PartitionNumber;
	GUID	PartitionType;                 // Partition type. See table 16-3.
	GUID	PartitionId;                   // Unique GUID for this partition.
} GPT_PARTITION;



extern GPT_PARTITION* GetDiskInfo(const WCHAR* DiskLetter);


#endif