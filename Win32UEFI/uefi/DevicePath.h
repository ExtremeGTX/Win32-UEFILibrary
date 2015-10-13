#ifndef _DEVICE_PATH_H_
#define _DEVICE_PATH_H_

#include "efi_common.h"
#include "DiskInfo.h"

/* functions prototypes */
extern HARDDRIVE_DEVICE_PATH* BuildHardDrivePath(const WCHAR* DiskLetter);
extern FILEPATH_DEVICE_PATH* BuildFilePath(const WCHAR* Path);
extern EFI_DEVICE_PATH_PROTOCOL* BuildDevicePathEnd();





/* Device Node Types */
#define MEDIA_DEVICE_PATH_TYPE				0x4
#define END_DEVICE_PATH_TYPE				0x7F

/* Predefined lengths of Some Device Nodes*/
#define HDD_PATH_LENGTH						42


/* Device Node Sub-Types */
#define HARDDRIVE_SUBTYPE					0x1
#define FILE_PATH_SUBTYPE					0x4
#define END_ENTIRE_DEVICE_PATH_SUBTYPE      0xFF


// Some helper macros from EfiDevicePath.h
#define DevicePathType(a)           ( ((a)->Type)  )
#define DevicePathSubType(a)        ( (a)->SubType )
#define DevicePathNodeLength(a)     ( ((a)->Length[0]) | ((a)->Length[1] << 8) )
#define NextDevicePathNode(a)       ( (EFI_DEVICE_PATH_PROTOCOL *) ( ((UINT8 *) (a)) + DevicePathNodeLength(a)))
#define IsDevicePathEndType(a)      ( DevicePathType(a) == END_DEVICE_PATH_TYPE )
#define IsDevicePathEndSubType(a)   ( (a)->SubType == END_ENTIRE_DEVICE_PATH_SUBTYPE )
#define IsDevicePathEnd(a)          ( IsDevicePathEndType(a) && IsDevicePathEndSubType(a) )
#define IsDevicePathUnpacked(a)     ( (a)->Type & EFI_DP_TYPE_UNPACKED )

#endif