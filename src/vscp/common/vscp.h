// FILE: vscp.h 
//
// Copyright (C) 2000-2012 Ake Hedman akhe@grodansparadis.com 
//
// This software is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//

// HISTORY:
//		021107 - AKHE Started this file
//		120328 - AKHE All CAN references removed.
//

#ifndef _VSCP_H_
#define _VSCP_H_

#include "vscp_class.h"
#include "vscp_type.h"
#include "canal.h"
#include "../../common/crc.h"

#ifndef WIN32
#include <stdint.h>
#include <sys/types.h>
#endif

// TODO should be moved to platform file.  /AKHE
#define SIZEOF_CHAR         1
#define SIZEOF_SHORT        2
#define SIZEOF_INT          4
//#ifndef _MSC_EXTENSIONS
#define SIZEOF_LONG_LONG    8
//#endif


#ifndef ___int8_t_defined
#define ___int8_t_defined

//#if ( __VISUALC__ == 1200 || __VISUALC__ == 600 )


////#ifdef __VISUALC__
// VC6 does not support 'long long'.
// http://support.microsoft.com/kb/65472
//typedef __int64                           int64_t;
//#else
//typedef long long                         int64_t;
//#endif



#if SIZEOF_CHAR == 1
typedef unsigned char                       uint8_t;
typedef signed char                         int8_t;
#elif SIZEOF_INT == 1
typedef unsigned int                        uint8_t;
typedef signed int                          int8_t;
#else  /* XXX */
#error "there's no appropriate type for uint8_t"
#endif


#if SIZEOF_INT == 4
typedef signed int                          int32_t;
typedef unsigned int                        uint32_t;
#elif SIZEOF_LONG == 4
typedef unsigned long                       uint32_t;
typedef unsigned long                       uint32_t;
#elif SIZEOF_SHORT == 4
typedef unsigned short                      uint32_t;
typedef unsigned short                      uint32_t;
#else  /* XXX */
#error "there's no appropriate type for uint32_t"
#endif


#if SIZEOF_SHORT == 2
typedef unsigned short                      uint16_t;
typedef signed short                        int16_t;
#elif SIZEOF_INT == 2
typedef unsigned int                        uint16_t;
typedef signed int                          int16_t;
#elif SIZEOF_CHAR == 2
typedef unsigned char                       uint16_t;
typedef signed char                         int16_t;
#else  /* XXX */
#error "there's no appropriate type for uint16_t"
#endif


//#if ( __VISUALC__ == 1200 || __VISUALC__ == 600 )
//typedef unsigned __int64                  uint64_t;
// VC6 does not support 'long long'.
//#else
//typedef unsigned long long                uint64_t;
//#endif


#ifdef __LINUX__ 


#else

#if SIZEOF_LONG_LONG == 8
typedef unsigned long long                  uint64_t;
typedef long long                           int64_t;
// AKHE 2012-04-18 Changed due to problems on Linux 64-bit
//typedef unsigned long int                  uint64_t;
//typedef long int 			   int64_t;
#elif defined(_MSC_EXTENSIONS)
typedef unsigned _int64                     uint64_t;
typedef _int64                              int64_t;
#elif SIZEOF_INT == 8
typedef unsigned int                        uint64_t;
#elif SIZEOF_LONG == 8
typedef unsigned long                       uint64_t;
#elif SIZEOF_SHORT == 8
typedef unsigned short                      uint64_t;
#else  /* XXX */
#error "there's no appropriate type for     u_int64_t"
#endif

#endif


#endif  // __int8_t_defined



// VSCP daemon defines
#define VSCP_MAX_CLIENTS                    4096    // abs. max is 0xffff
#define VSCP_MAX_DEVICES                    4096    // abs. max is 0xffff

//
// daemon
//
// Base values for queue or channel id's
//
#define VSCP_CLIENT_ID_BASE                 0x00000000
#define VSCP_DEVICE_ID_BASE                 0x00010000


/// Names of mutex's
#define VSCPD_CLIENT_OBJ_MUTEX              _("____VSCPD_CLIENT_OBJ_MUTEX____")
#define VSCPD_UDP_OBJ_MUTEX                 _("____VSCPD_UDP_OBJ_MUTEX____")
#define VSCPD_SEND_OBJ_MUTEX                _("____VSCPD_SEND_OBJ_MUTEX____")
#define VSCPD_RECEIVE_OBJ_MUTEX             _("____VSCPD_RECEIVE_OBJ_MUTEX____")
#define VSCPD_CLIENT_MUTEX                  _("__VSCPD_CLIENT_MUTEX__")


#define	VSCP_LEVEL2_UDP_PORT                9598
#define	VSCP_LEVEL2_TCP_PORT                9598

#define VSCP_ADDRESS_SEGMENT_CONTROLLER	    0x00
#define VSCP_ADDRESS_NEW_NODE               0xff

#define VSCP_MAX_DATA                       (512-25)


#ifdef __cplusplus
extern "C" {
#endif

//			* * * General structure for VSCP * * *

// This structure is for VSCP Level II
// 

typedef struct  {	
	uint16_t crc;           // crc checksum - currently only used for UDP and RF
	uint8_t  *pdata;        // Pointer to data. Max 487 (512- 25) bytes
    // Following two are for daemon internal use
	uint32_t obid;          // Used by driver for channel info etc.
	uint32_t timestamp;     // Relative time stamp for package in microseconds
    // CRC should be calculated from
    // here to end + datablock
	uint16_t head;          // Bit 16   GUID is IP v.6 address.
							// bit 765  prioriy, Priority 0-7 where 0 is highest.
                            // bit 4 = hardcoded, true for a hardcoded device.
                            // bit 3 = Dont calculate CRC, false for CRC usage.
                            // bit 2 = Reserved.
                            // bit 1 = Reserved.
                            // bit 0 = Reserved.
	uint16_t vscp_class;    // VSCP class
	uint16_t vscp_type;     // VSCP type
	uint8_t  GUID[ 16 ];    // Node address MSB(0) -> LSB(15)
	uint16_t sizeData;      // Number of valid data bytes		
} vscpEvent;


typedef vscpEvent *PVSCPEVENT;



// This structure is for VSCP Level II with data embedded which is used
// for the TCP interface.// 

typedef struct {	
	uint16_t crc;                   // crc checksum
	
	// Following two are for daemon internal use
	uint32_t obid;                  // Used by driver for channel info etc.
	uint32_t timestamp;             // Relative time stamp for package in microseconds.
// CRC should be calculated from
// here to end + datablock
	uint8_t head;                   // bit 7,6,5 prioriy => Priority 0-7 where 0 is highest.
                                    // bit 4 = hardcoded, true for a hardcoded device.
                                    // bit 3 = Dont calculate CRC, Set to zero to use CRC.
                                    // bit 2 = Set means this is CAN message.
                                    // bit 1 = If bit 2 is set; Extended CAN message if set
                                    // bit 0 = If bit 2 is set: Remote frame if set
	uint16_t vscp_class;            // VSCP class
	uint16_t vscp_type;             // VSCP type
	uint8_t  GUID[ 16 ];            // Node address MSB(0) -> LSB(15)
	uint16_t sizeData;              // Number of valid data bytes		

    uint8_t  data[VSCP_MAX_DATA];   // Pointer to data. Max 487 (512- 25) bytes

} vscpEventEx;


typedef vscpEventEx *PVSCPEVENTEX;

#define VSCP_PRIORITY_0             0x00
#define VSCP_PRIORITY_1             0x20
#define VSCP_PRIORITY_2             0x40
#define VSCP_PRIORITY_3             0x60
#define VSCP_PRIORITY_4             0x80
#define VSCP_PRIORITY_5             0xA0
#define VSCP_PRIORITY_6             0xC0
#define VSCP_PRIORITY_7             0xE0

#define VSCP_PRIORITY_HIGH          0x00
#define VSCP_PRIORITY_LOW           0xE0
#define VSCP_PRIORITY_NORMAL        0x60

#define VSCP_HEADER_PRIORITY_MASK   0xE0

#define VSCP_HEADER_HARD_CODED      0x10    // If set node nickname is hardcoded
#define VSCP_HEADER_NO_CRC          0x08    // Don't calculate CRC

#define VSCP_NO_CRC_CALC            0x08    // If set no CRC is calculated

#define VSCP_MASK_PRIORITY          0xE0
#define VSCP_MASK_HARDCODED         0x10
#define VSCP_MASK_NOCRCCALC         0x08

#define VSCP_LEVEL1_MAXDATA         8
#define VSCP_LEVEL2_MAXDATA         (512 - 25)

#define VSCP_CAN_ID_HARD_CODED	    0x02000000  // Hard coded bit in CAN frame id

//
// Filter structure for VSCP Level 2
//

typedef struct  {

	uint8_t filter_priority;        // Priority 
	uint8_t mask_priority;
									
	uint16_t filter_class;          // VSCP class
	uint16_t mask_class;

	uint16_t filter_type;           // VSCP type
	uint16_t mask_type;

	uint8_t filter_GUID[ 16 ];      // Node address MSB -> LSB, LSB is node nickname id
	uint8_t mask_GUID[ 16 ];        //		when interfacing the VSCP daemon.
	
} vscpEventFilter;


typedef vscpEventFilter *PVSCPEVENTFILTER;


// VSCP LEVEL II UDP datagram offsets
#define VSCP_UDP_POS_HEAD             0
#define VSCP_UDP_POS_CLASS            1
#define VSCP_UDP_POS_TYPE             3
#define VSCP_UDP_POS_GUID             5
#define VSCP_UDP_POS_SIZE             21

#define VSCP_UDP_POS_DATA             23  // Holder for beginning of data
#define VSCP_UDP_POS_CRC              25  // dummy: actual is len - 2


/*!
	VSCP Channel Info

	This is the channel VSCP Info structure
*/

typedef struct structVSCPChannelInfo {
	unsigned char channelType;      // Leve I, Level II etc from canal.h 
	unsigned short channel;         // daemon channel number
	char GUID[ 16 ];                // Channel GUID id
	
} VSCPChannelInfo;

typedef  VSCPChannelInfo	*PVSCPCHANNELINFO;

// Bootloaders
#define VSCP_BOOTLOADER_VSCP						0x00	// VSCP bootloader algorithm
#define VSCP_BOOTLOADER_PIC1						0x01	// PIC algorithm 0
#define VSCP_BOOTLOADER_AVR1						0x10	// AVR algorithm 0
#define VSCP_BOOTLOADER_LPC1						0x20	// NXP/Philips LPC algorithm 0
#define VSCP_BOOTLOADER_ST						    0x30	// ST STR algorithm 0
#define VSCP_BOOTLOADER_NONE						0xff


//			* * * Data Coding for VSCP packets * * *

// Data format masks
#define VSCP_MASK_DATACODING_TYPE       0xE0  // Bits 5,6,7
#define VSCP_MASK_DATACODING_UNIT       0x18  // Bits 3,4
#define VSCP_MASK_DATACODING_INDEX      0x03  // Bits 0,1,2

// Theese bits are coded in the three MSB bytes of the first data byte
// in a paket and tells the type of the data that follows.
#define VSCP_DATACODING_BIT             0x00
#define VSCP_DATACODING_BYTE            0x20
#define VSCP_DATACODING_STRING          0x40
#define VSCP_DATACODING_INTEGER         0x60
#define VSCP_DATACODING_NORMALIZED      0x80
#define VSCP_DATACODING_SINGLE          0xA0
#define VSCP_DATACODING_RESERVED1       0xC0
#define VSCP_DATACODING_RESERVED2       0xE0

// These bits are coded in the four least significant bits of the first data byte
// in a paket and tells how the following data should be interpreted. For a flow sensor
// the default format can be liters/minute. Other formats such as m3/second can be defined 
// by the node if it which. However it must always be able to report in the defaultformat.
#define VSCP_DATACODING_INTERPRETION_DEFAULT  0

/// Get data coding type
#define VSCP_DATACODING_TYPE( b ) ( VSCP_MASK_DATACODING_TYPE & b )

/// Get data coding unit
#define VSCP_DATACODING_UNIT( b ) ( ( VSCP_MASK_DATACODING_UNIT & b ) >> 3 )

/// Get data coding sensor index
#define VSCP_DATACODING_INDEX( b ) ( VSCP_MASK_DATACODING_INDEX & b )

// CRC8 Constants
#define VSCP_CRC8_POLYNOMIAL            0x18
#define VSCP_CRC8_REMINDER              0x00

// CRC16 Constants
#define VSCP_CRC16_POLYNOMIAL           0x1021
#define VSCP_CRC16_REMINDER             0xFFFF

// CRC32 Constants
#define VSCP_CRC32_POLYNOMIAL           0x04C11DB7
#define VSCP_CRC32_REMINDER             0xFFFFFFFF


// Node data - the required registers are fetched from this 
//	structure
struct myNode {
	unsigned char GUID[ 16 ];
	unsigned char nicknameID;
};

// * * * Standard VSCP registers * * *

// Register defines above 0x7f
#define VSCP_STD_REGISTER_ALARM_STATUS          0x80

#define VSCP_STD_REGISTER_MAJOR_VERSION         0x81
#define VSCP_STD_REGISTER_MINOR_VERSION         0x82
#define VSCP_STD_REGISTER_SUB_VERSION           0x83

// 0x84 - 0x88
#define VSCP_STD_REGISTER_USER_ID               0x84

// 0x89 - 0x8C
#define VSCP_STD_REGISTER_USER_MANDEV_ID        0x89

// 0x8D -0x90
#define VSCP_STD_REGISTER_USER_MANSUBDEV_ID     0x8D

#define VSCP_STD_REGISTER_NICKNAME_ID           0x91

#define VSCP_STD_REGISTER_PAGE_SELECT_MSB       0x92
#define VSCP_STD_REGISTER_PAGE_SELECT_LSB       0x93

#define VSCP_STD_REGISTER_FIRMWARE_MAJOR        0x94
#define VSCP_STD_REGISTER_FIRMWARE_MINOR        0x95
#define VSCP_STD_REGISTER_FIRMWARE_SUBMINOR     0x96

#define VSCP_STD_REGISTER_BOOT_LOADER           0x97
#define VSCP_STD_REGISTER_BUFFER_SIZE           0x98
#define VSCP_STD_REGISTER_PAGES_COUNT           0x99

// 0xc0 - 0xcf
#define VSCP_STD_REGISTER_GUID                  0xC0

// 0xe0 - 0xff
#define VSCP_STD_REGISTER_DEVICE_URL            0xE0


#ifdef __cplusplus
}
#endif



#endif // _VSCP_H_

