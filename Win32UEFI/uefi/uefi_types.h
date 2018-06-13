#ifndef _UEFI_TYPES_H_
#define _UEFI_TYPES_H_

#include "types.h"

/* UEFI Specs v2.3.1 Section 3.2*/
#define EFI_GLOBAL_VARIABLE L"{8BE4DF61-93CA-11D2-AA0D-00E098032B8C}" //{0x8BE4DF61,0x93CA,0x11d2,0xAA,0x0D,0x00,0xE0,0x98,0x03,0x2B,0x8C}
#define EFI_DUMMY_GUID		L"{00000000-0000-0000-0000-000000000000}"

//*******************************************************
// Attributes (3.1.3 Load Options)
//*******************************************************
#define LOAD_OPTION_ACTIVE				0x00000001
#define LOAD_OPTION_FORCE_RECONNECT		0x00000002
#define LOAD_OPTION_HIDDEN				0x00000008
#define LOAD_OPTION_CATEGORY			0x00001F00
#define LOAD_OPTION_CATEGORY_BOOT		0x00000000
#define LOAD_OPTION_CATEGORY_APP		0x00000100
// All values 0x00000200-0x00001F00 are reserved


//*******************************************************
// Value reserved to indicate that No. of BootOptionIDs has reached the limit (not related to UEFI standard)
//*******************************************************
#define BOOT_OPTION_MAX_LIMIT			0xFFFF



#pragma pack(push, 1)
//#######################################################
// EFI Protocols
//#######################################################

//*******************************************************
// EFI_DEVICE_PATH_PROTOCOL (UEFI Specs: 9.2 EFI Device Path Protocol)
//*******************************************************
typedef struct {
	UINT8 Type;
	UINT8 SubType;
	UINT8 Length[2];
} EFI_DEVICE_PATH_PROTOCOL;


//#######################################################
// EFI Global Varibales
//#######################################################

//*******************************************************
// EFI_LOAD_OPTION (UEFI Specs: 3.1.3 Load Options)
//*******************************************************

typedef struct {
	UINT32 Attributes;
	UINT16 FilePathListLength;
	//WCHAR *Description;
	//EFI_DEVICE_PATH_PROTOCOL FilePathList;
	//UINT8 *OptionalData;
} EFI_LOAD_OPTION;


//*******************************************************
// BootOrder (UEFI Specs: 3.2 Globally Defined Variables)
//*******************************************************
typedef UINT16 EFI_BOOT_ORDER;

//*******************************************************
// Media Device (UEFI Specs: 9.3.6.1 Hard Drive)
//*******************************************************
typedef struct {
	EFI_DEVICE_PATH_PROTOCOL        Header;
	///
	/// Describes the entry in a partition table, starting with entry 1.
	/// Partition number zero represents the entire device. Valid
	/// partition numbers for a MBR partition are [1, 4]. Valid
	/// partition numbers for a GPT partition are [1, NumberOfPartitionEntries].
	///
	UINT32                          PartitionNumber;
	///
	/// Starting LBA of the partition on the hard drive.
	///
	UINT64                          PartitionStart;
	///
	/// Size of the partition in units of Logical Blocks.
	///
	UINT64                          PartitionSize;
	///
	/// Signature unique to this partition:
	/// If SignatureType is 0, this field has to be initialized with 16 zeros.
	/// If SignatureType is 1, the MBR signature is stored in the first 4 bytes of this field.
	/// The other 12 bytes are initialized with zeros.
	/// If SignatureType is 2, this field contains a 16 byte signature.
	///
	UINT8                           Signature[16];
	///
	/// Partition Format: (Unused values reserved).
	/// 0x01 - PC-AT compatible legacy MBR.
	/// 0x02 - GUID Partition Table.
	///
	UINT8                           PartitionFormat;
	///
	/// Type of Disk Signature: (Unused values reserved).
	/// 0x00 - No Disk Signature.
	/// 0x01 - 32-bit signature from address 0x1b8 of the type 0x01 MBR.
	/// 0x02 - GUID signature.
	///
	UINT8                           SignatureType;
} HARDDRIVE_DEVICE_PATH;

typedef struct {
	EFI_DEVICE_PATH_PROTOCOL        Header;
	///
	/// A NULL-terminated Path string including directory and file names, 
	/// its length doesn't matter as its already allocated in BDS_LOAD_OPTION.FilePathList and it includes '\0'
	/// We can't use pointer here, as the data after header is the real pathName not address of the pathName
	///
	WCHAR                          PathName[1]; 
} FILEPATH_DEVICE_PATH;
//*******************************************************
// Boot Device Selection BDS (UEFI Specs: N/A)
//*******************************************************
typedef struct {
	UINT16                      LoadOptionIndex;

	UINT32                      Attributes;
	UINT16                      FilePathListLength;
	WCHAR                      *Description;
	EFI_DEVICE_PATH_PROTOCOL    *FilePathList;

	void*                       OptionalData;
	UINT8                       OptionalDataSize;
} BDS_LOAD_OPTION;

#pragma pack(pop)



#endif