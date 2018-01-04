#include <string.h>
#include <math.h>
#include "ht32.h"
#include "ht32_board.h"
#include "sdio_sd.c"
#include "./FatFS/tff.h"
#include "RA8875.h"
#include "Keypad.h"
#include "DisplayDefs.h"
#include "TimerManager.h"
#include "common/MX25L512.h"
#include "RTC.h"

#define _FW 0x18010200

u8				tick = 0;
bool			click = FALSE;
u32 			uSpeedCount;

/* Global variables ----------------------------------------------------------------------------------------*/
FATFS			fs; /* File system object																	*/
FIL 			fsrc; /* File object																		*/
DIR 			dir; /* Directory object																	*/
FILINFO 		fno; /* File info object																	*/
UINT			dcnt = 0;
FRESULT 		result = FR_NOT_READY;
u8				SD_ReadBuf[51200]; //2017/09/25

void EX1( void );
void BTE_Ex3( void );
void BTE_Ex1( void );
void BTE_SMILE( void );


const unsigned char bw[2048] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x3c,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x0c,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0xe0,0x7a,0x00,0x00,0x3e,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0xfe,0x00,
	0x00,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0xa8,0xce,0x00,0x00,0x56,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xf9,
	0xde,0x00,0x00,0xde,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0xdd,0xda,0x00,0x00,0xee,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,
	0x01,0xdd,0x5c,0x00,0x00,0x7a,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3c,0x00,0x00,0x01,0x9d,0xfc,0x00,
	0x00,0xcc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,
	0x00,0x10,0x01,0xf7,0xb8,0x01,0xc1,0xbc,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7b,0x00,0x18,0x01,0xef,
	0xd0,0x01,0x50,0xb8,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x77,0x00,0x38,0x00,0xbc,0x60,0x01,0xf8,0x90,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x80,0x38,
	0x38,0xcc,0xcf,0xe1,0xbc,0xe0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x76,0x81,0xf8,0x2e,0x64,0x3b,0xe0,
	0xdc,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x75,
	0x87,0xef,0x3b,0x2d,0x7e,0xc1,0xd4,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7b,0x83,0xef,0xb3,0x98,
	0xb3,0x40,0xe6,0x80,0x00,0x07,0x80,0x00,0x00,0x00,
	0x00,0x6f,0x80,0x3f,0x3b,0x8b,0xbf,0x80,0xfe,0x80,
	0x00,0x18,0x20,0x00,0x00,0x00,0x00,0x3f,0x00,0x30,
	0x2b,0x47,0xcf,0x80,0x56,0x80,0x00,0x23,0x10,0x00,
	0x00,0x00,0x00,0x39,0x00,0x70,0x3d,0xc7,0x7f,0x00,
	0x3e,0x80,0x00,0x4f,0xc8,0x00,0x00,0x00,0x1f,0xdb,
	0x0e,0x32,0x1e,0xe6,0x7c,0x00,0x1a,0x03,0xc0,0x58,
	0x24,0x00,0x00,0x00,0x3f,0xea,0x1e,0x21,0x1e,0xeb,
	0xf0,0x40,0x07,0x07,0xfc,0xd7,0xb4,0x00,0x00,0x00,
	0xd7,0xf8,0x3e,0x01,0x0d,0xa8,0x00,0x40,0x01,0x0f,
	0xfc,0x96,0x94,0x00,0x00,0x00,0x7f,0xb8,0x76,0x01,
	0x07,0x38,0x78,0x40,0x01,0x0f,0xfe,0x92,0x94,0x00,
	0x00,0x00,0x17,0xe0,0xee,0x01,0x03,0xf1,0xf8,0x43,
	0xf2,0x0f,0xfe,0x4d,0x28,0x00,0x00,0x00,0x03,0x00,
	0xfe,0x00,0x80,0xf3,0xe0,0x4f,0xa6,0x0f,0xfc,0x60,
	0x28,0x00,0x00,0x00,0x00,0x02,0xcc,0x00,0x80,0x13,
	0xf0,0x5f,0x24,0x67,0xfc,0x30,0xd0,0x00,0x00,0x00,
	0x00,0x02,0xb8,0x3d,0x00,0x2b,0xf0,0x5d,0xec,0x07,
	0xfc,0x1f,0xe0,0x80,0x00,0x00,0x00,0x02,0xf8,0xfd,
	0x00,0x2b,0xe0,0xfb,0xc8,0x03,0xf8,0x00,0x0f,0x80,
	0x00,0x00,0x3f,0xc2,0x61,0xf9,0x00,0x2b,0xc0,0xe7,
	0x98,0x07,0xff,0x00,0x2f,0x80,0x00,0x00,0x7e,0xf3,
	0x83,0xf9,0x00,0x3f,0x80,0xbf,0x34,0x01,0xff,0x80,
	0x7f,0x80,0x00,0x01,0x87,0xe9,0x05,0xfb,0x00,0x3f,
	0x01,0xfc,0xea,0x00,0x7f,0x80,0xf8,0x00,0x00,0x00,
	0x7d,0x1d,0x05,0xf2,0x04,0x30,0x03,0x01,0xde,0x00,
	0x1e,0x01,0xff,0x00,0x03,0x00,0x37,0xbc,0x85,0xf2,
	0x0e,0x60,0x03,0x07,0xbb,0x0c,0x00,0x03,0xef,0x00,
	0x0f,0x00,0x1f,0xf0,0x8f,0xc2,0x1a,0x68,0x06,0x1e,
	0x2f,0x4c,0x80,0x03,0xfe,0x00,0x1f,0xc0,0x03,0x80,
	0x4b,0x82,0x1b,0x6c,0x0c,0x3c,0x77,0x6d,0x81,0x83,
	0x8e,0x00,0x0f,0xff,0xf1,0xfe,0x6c,0x06,0x3b,0x7e,
	0x18,0xf0,0x74,0x3f,0x03,0xc3,0xfc,0x00,0x01,0xff,
	0xf3,0x83,0x30,0x06,0x2b,0xfb,0x19,0xe0,0x56,0x1a,
	0x06,0xc6,0xf8,0x00,0x01,0xfc,0x64,0x00,0xd8,0x06,
	0x3b,0xff,0x37,0x98,0x6f,0xf7,0xee,0x45,0xe0,0x00,
	0x07,0xfc,0x18,0xfc,0x66,0x06,0x3e,0xf9,0x6f,0x3c,
	0x6c,0xd7,0x1b,0xc7,0x00,0x00,0x0f,0xe6,0x19,0xc6,
	0x23,0x06,0x3d,0xf3,0x7c,0x3e,0x29,0x3f,0x1d,0xc0,
	0x7d,0x80,0x3f,0xc0,0x33,0x01,0x11,0xc2,0x3b,0x6b,
	0xf8,0x36,0x3c,0x6d,0x8f,0xc9,0xbb,0xe0,0x7f,0x00,
	0x22,0x71,0x90,0x63,0x27,0x64,0xf0,0x36,0x08,0x4c,
	0xaa,0x83,0x77,0xb0,0xf0,0x00,0x26,0xc8,0x90,0x1b,
	0x37,0x63,0xe0,0x35,0x10,0x0c,0x1f,0x97,0xc2,0xe0,
	0x00,0x00,0x26,0xe4,0x90,0x0f,0xb2,0x61,0xe0,0x3b,
	0x23,0xf0,0x17,0x2f,0xdf,0xc0,0x00,0x00,0x26,0x6c,
	0x90,0x03,0xd4,0x61,0xc0,0x3f,0x4f,0xe0,0x17,0x6e,
	0xff,0x80,0x00,0x00,0x33,0x0a,0x90,0x01,0xec,0x21,
	0xc0,0x36,0xdf,0x60,0x1c,0xc3,0xfe,0x00,0x00,0x00,
	0x31,0xf9,0xa0,0x00,0xf8,0x23,0xb0,0x1c,0xbe,0x20,
	0x19,0x80,0x30,0x00,0x00,0x00,0x18,0x43,0xa4,0x00,
	0x38,0x33,0xbc,0x0b,0xfb,0xc0,0x0e,0x00,0x00,0x00,
	0x00,0x00,0x1c,0x03,0xc4,0x00,0x1c,0x33,0xaf,0x05,
	0x57,0xc0,0x3c,0x00,0x00,0x00,0x00,0x00,0x07,0x1c,
	0xc2,0x00,0x06,0x33,0x37,0x83,0xcc,0x87,0xe0,0x00,
	0x3f,0x00,0x00,0x00,0x03,0xff,0x62,0x0f,0xf3,0x13,
	0xbb,0xc2,0xbf,0x7d,0xe0,0x00,0x40,0xc0,0x00,0x00,
	0x00,0x7c,0x22,0x37,0xfd,0x9b,0xbb,0xc2,0x1f,0xef,
	0xdc,0x00,0x8e,0x60,0x00,0x00,0x06,0x00,0x31,0x6b,
	0xfa,0xdb,0x8f,0xa3,0xe7,0x98,0xff,0x01,0x31,0xa0,
	0x00,0x00,0x06,0x00,0x11,0xfc,0x1f,0x7b,0xb1,0xe6,
	0x0e,0x0f,0x7b,0x81,0x20,0x80,0x00,0x00,0x36,0x60,
	0x10,0x9d,0xf7,0xbf,0xbf,0x76,0x18,0x05,0xa7,0xc3,
	0x5a,0x50,0x00,0x00,0x3a,0xc0,0x10,0x4b,0xde,0x3f,
	0xde,0xf6,0x3b,0xc3,0xfd,0xc2,0x56,0x50,0x00,0x00,
	0x1b,0x80,0x10,0x23,0xb8,0x1f,0xdf,0xf6,0x67,0xe1,
	0xdd,0xe3,0x6a,0x50,0x00,0x00,0xf4,0xf0,0x18,0x18,
	0x00,0x0f,0xcd,0x74,0xe7,0x18,0x7f,0xc1,0x3c,0x80,
	0x00,0x00,0xfb,0xf8,0x18,0x4c,0x00,0x07,0xc7,0x14,
	0xc7,0x00,0x0e,0x01,0x91,0xa0,0x00,0x00,0x0f,0x80,
	0x08,0x63,0x80,0x07,0xe3,0xd5,0xc3,0x80,0x01,0xbc,
	0xf3,0x40,0x00,0x00,0x1a,0xc0,0x88,0xf0,0xe0,0x07,
	0xe1,0xd5,0x83,0xc0,0x03,0xf4,0x3f,0x80,0x00,0x00,
	0x36,0xe1,0xc8,0xf8,0x3c,0x03,0xf0,0x75,0x87,0xe0,
	0x07,0x5c,0x40,0x00,0x00,0x00,0x26,0x63,0x69,0xd8,
	0x07,0x81,0xf0,0x07,0x0f,0xf0,0x0b,0x78,0xc0,0x00,
	0x00,0x00,0x06,0x01,0x69,0xdc,0x01,0xc1,0xf8,0x07,
	0x1f,0xf0,0x1f,0xf9,0x9f,0xc0,0x00,0x00,0x00,0x82,
	0x6d,0xd4,0x1f,0x31,0xf8,0x0f,0x1f,0xf0,0x1b,0x33,
	0xfd,0xf0,0x00,0x00,0x00,0x87,0xed,0xdc,0x7d,0xcc,
	0xfc,0x0f,0x1f,0xf0,0x2f,0xee,0x47,0xf8,0x00,0x03,
	0xfc,0x47,0xa6,0x7d,0xdf,0xf6,0xfc,0x0e,0x0f,0xe0,
	0x3f,0xb8,0x2c,0xbc,0x00,0x07,0xef,0x45,0x66,0xbd,
	0x0e,0xfb,0xfe,0x0e,0x00,0x40,0x07,0xe0,0x3f,0x9e,
	0x00,0x18,0x76,0xa3,0xe6,0x50,0xfb,0x0d,0xff,0x0e,
	0x02,0x47,0xfe,0x00,0x0e,0xeb,0x00,0x07,0xd9,0xd3,
	0xc3,0x68,0x6f,0xf8,0xff,0x0e,0x01,0xfe,0x00,0x70,
	0x07,0xf7,0x00,0x03,0x7b,0xdb,0xc3,0xa8,0x1e,0xf0,
	0x7f,0x1c,0x0f,0xc0,0x00,0x7a,0x01,0xfc,0x00,0x01,
	0xf7,0x0c,0x81,0xd0,0x02,0x0e,0x7f,0x9c,0x38,0x00,
	0x00,0x7c,0x00,0x00,0x00,0x00,0x38,0x06,0x00,0xe0,
	0x00,0x0b,0x3f,0x9c,0xf0,0x00,0x00,0x7e,0x00,0x00,
	0x00,0x00,0x00,0x03,0x80,0x70,0x00,0x0f,0xbf,0xd9,
	0xc7,0xc0,0x00,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,
	0xe0,0x3c,0x00,0x1d,0xff,0xdb,0x9f,0xc0,0x00,0x3f,
	0xe0,0x00,0x00,0x00,0x00,0x00,0x3c,0x1f,0x00,0x05,
	0xdf,0xfe,0x6f,0x80,0x00,0x3f,0xf0,0x00,0x00,0x00,
	0x00,0x00,0x07,0xe7,0xc0,0x0d,0x5f,0xfc,0xef,0x80,
	0x00,0xff,0xf0,0x00,0x00,0x00,0x00,0x03,0xfe,0xff,
	0xf8,0x0f,0x7f,0xf9,0xf8,0x80,0x00,0xef,0xe0,0x00,
	0x00,0x00,0x00,0x0f,0xf7,0x83,0xfe,0x0e,0xff,0xf9,
	0xf7,0x80,0x00,0x1c,0x00,0x00,0x00,0x00,0x00,0x1e,
	0xf9,0x00,0x7f,0x85,0xff,0xf2,0xef,0x00,0x00,0x1c,
	0x00,0x00,0x00,0x00,0x00,0x7f,0x86,0x00,0x07,0xf6,
	0xbf,0xe2,0xdf,0x00,0x00,0x18,0x00,0x00,0x00,0x00,
	0x00,0x5f,0x6e,0x00,0x00,0xfb,0x5f,0xe6,0x82,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf2,0xfc,0x00,
	0x00,0x3f,0xdf,0xe6,0x7c,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0xbd,0xd8,0x00,0x00,0x0f,0x5f,0xe7,
	0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xff,
	0xe0,0x00,0x00,0x07,0x9f,0xe4,0x70,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x7f,0x80,0x00,0x00,0x01,
	0xdf,0xe7,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe6,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0xe0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3f,0xe0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x3f,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0xe0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x1f,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0xe0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3f,0xe0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x3f,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0xc0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x3f,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0xc0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3f,0xc0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x7f,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0xc0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x7f,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0xc0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x7f,0xc0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x7f,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xc0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xc0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0xc0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0xff,0xc0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xff,0xc0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0xff,0xc0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xc0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x03,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xff,0xe0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0xff,0xe0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,
	0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x07,0xff,0xe0,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x07,0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xff,0xe0,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x0f,0xff,0xe0,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,
	0xff,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
};


u8 mBuff2[256] = "test";

// Define a keyboard layout as a calculator-like keypad
// 789 /(
// 456 *)
// 123 -
// 00. +=
const char numberkeys[] =
{
    5,0x01, 10,'7',10,'8',10,'9', 5,0x01, 10,'/',10,'(', 0,0,
    5,0x01, 10,'4',10,'5',10,'6', 5,0x01, 10,'*',10,')', 0,0,
    5,0x01, 10,'1',10,'2',10,'3', 5,0x01, 10,'-',10,KYBD_SYM_BS, 0,0,
    5,0x01, 20,'0',       10,'.', 5,0x01, 10,'+',10,'=', 0,0,
    0,0
};

// Define the implementation of that keyboard
const keyboard_t altkeyboard =
{
    100,        // x=100; left edge
    0,          // y=0; computed from bottom up
    240,        // width=240
    0,          // height=0; bottom of screen justified
    4,          // rows
    6,          // columns
    numberkeys, // pointer to the keypad
    numberkeys
};


_KEYPAD *kp;
_RA8875 *lcd;


void CalibrateTS(void)
{
    // FIL *fh;
    tpMatrix_t matrix;
    RetCode_t r;

	printf( "\r\nCalibrateTS" );

    r = lcd->TouchPanelCalibrate( "\r\nCalibrate the touch panel", &matrix, 10 );

    if ( r == noerror )
    {
    	/*
        fh = fopen("/local/tpcal.cfg", "wb");

        if ( fh )
        {
            fwrite(&matrix, sizeof(tpMatrix_t), 1, fh);
            fclose(fh);
        }
        else
        {
            lcd.printf( "Cannot save calibration in /local/tpcal.cfg\r\n" );
            wait_ms(1000);
        }
        */
    }
    else
    {
        lcd->printf( "\r\nTouchPanelCalibrate returned error code %d", r );

       	wait_ms( 2000 );
    }

}

void FloatingSmallQWERTYTest( loc_t x, loc_t y, dim_t w, dim_t h )
{
    keyboard_t tiny;

    char name1[ 20 ];

    kp->SetKeyboard( NULL, KYBD_SYM_ENTER, KYBD_SYM_ESCAPE );

    memcpy( &tiny, kp->GetKeyboard( ), sizeof( keyboard_t ) );

	tiny.x = x;
	tiny.y = y;
    tiny.width = w;
    tiny.height=h;

    printf( "\r\nFloatingSmallQWERTYTest" );

    // copy definition and then resize it
    // select the internal keyboard
    // now select this tiny keyboard
    kp->SetKeyboard( &tiny, KYBD_SYM_ENTER, KYBD_SYM_ESCAPE );

    kp->SetKeyboardFont( 0, 4 );

    if ( kp->GetString( name1, sizeof( name1 ), "", FALSE, 0, FALSE ) )
    {
    	/*
        LCD.foreground(BrightRed);
        lcd.background(Black);
        lcd.cls();
        lcd.SetTextCursor(0,40);
        lcd.printf("Compressed: %s\r\n", name1);
        */
    }
}

void CalculatorKeypadTest(void)
{
    char name1[20];

    kp->SetKeyboard( &altkeyboard, '=', 0 );
    kp->SetKeyboardFont( 0, 4 );

    if( kp->GetString( name1, sizeof( name1 ), "Calc:", FALSE, 0, FALSE ) )
    {
    	/*
        lcd.foreground(BrightRed);
        lcd.background(Black);
        lcd.cls();
        lcd.SetTextCursor(0,40);

        lcd.printf( "Calculator: %s\r\n", name1 );
        */
    }
}


int main( void )
{
	// int i = 0;
	// u8	tStr[ ] = "";
	// u8	tStr2[ 9 ] = "";
	// u16 temp;
	// u16 temp_result;
	u8	lsd,msd;
	rtc_t rtc;
	// int second;
	rect_t r;
	char buffer [50];
  	int n, a=5, b=3;

	RETARGET_Configuration( );						//Retarget Related configuration
	printf( "\r\n========== Initial ==========" );
	printf( "\r\nFW %x", _FW );

	HT32F_DVB_LEDInit( HT_LED1 );

	NVIC_EnableIRQ( SDIO_IRQn );
	NVIC_EnableIRQ( PDMACH6_IRQn ); 					  // SDIO_RX
	NVIC_EnableIRQ( PDMACH7_IRQn ); 					  // SDIO_TX

	/* SYSTICK configuration */
  	SYSTICK_ClockSourceConfig( SYSTICK_SRC_STCLK );       // Default : CK_SYS/8
  	SYSTICK_SetReloadValue( SystemCoreClock / 8 / 1000 ); // (CK_SYS/8/1000) = 1ms on chip
  	SYSTICK_IntConfig( ENABLE );                          // Enable SYSTICK Interrupt
  	SYSTICK_CounterCmd( SYSTICK_COUNTER_CLEAR );
	SYSTICK_CounterCmd( SYSTICK_COUNTER_ENABLE );
	// ---------------------------------------------

    #if 0
	AFIO_GPxConfig( GPIO_PC, AFIO_PIN_12, AFIO_FUN_GPIO );
	GPIO_DirectionConfig( HT_GPIOC, AFIO_PIN_12, GPIO_DIR_OUT );

	while( 1 )
	{
		wait_ms( 1000 );

		GPIO_WriteOutBits( HT_GPIOC, AFIO_PIN_12,SET );

		// HT32F_DVB_LEDToggle( HT_LED1 );

		wait_ms( 1000 );

		GPIO_WriteOutBits( HT_GPIOC, AFIO_PIN_12,0 );
	}
    #endif

	wait_ms( 500 );

    LCD_Init( );
    LCD_Config( );

    EBI_SRAM_Init( );
    EBI_SRAM_Test( );
   	EBI_SRAM_Test_1( );

	kp = Keypad_CreateObj( Black, Gray );
	lcd = RA8875_CreateObj( );

	I2C_EEPROM_Init( );
	RTC_Init();

	/* Initialize the SPI_FLASH driver */
  	result = SPI_FLASH_Init();

  	printf( "\r\n%d = SPI_FLASH_Init( )", result );

  	MX25L512_Test( );

	f_mount( 0, &fs );

	// DrawPictureFromSD( "file1.dat", SD_ReadBuf, 100, 0 );

    // lcd->foreground( Yellow );
    // lcd->background( Black );
	// ---------------------------------------

	// CalibrateTS();
	// CalculatorKeypadTest( );

	/*
	Graphic_Mode( );
	lcd->clsw( FULLWINDOW );
	r.p1.x = 0;
	r.p1.y = 0;
	r.p2.x = 800;
	r.p2.y = 480;
	lcd->fillrect( r, Gray, FILL );
	*/

	Graphic_Mode( );
	Write_Dir( 0x8E, 0x80 ); 	//Clean
	CmdWrite( 0x02 );
	openBMP3( "bg.dat" );

	// FloatingSmallQWERTYTest( 30, 50, 725, 0 );

	// FloatingSmallQWERTYTest( 30, 50, 725, 0 );

	/*
	DrawString(180,20,tStr2,0,0,FALSE,FALSE,Red,Black);
	DrawRect(350,200,450,300,Red,Blue,TRUE);
	DrawLine(250,450,250,50,Red,Green);
	DrawTri(100,200,300,400,450,400,Red,Purple,FALSE);
	DrawEllipse(150,300,30,50,White,Red,TRUE);
	DrawCircle(550,240,60,White,Red,TRUE);
	DrawRoundRect(450,500,50,100,0,0,15,15,White,Red,FALSE);
	DrawCurve(600,60,30,30,1,White,Yellow,FALSE);
	Chk_Busy();

		//Write_Dir(0x10,0x0F);//16Bit

		Write_Dir(0x10,0x0c);//8Bit
		//Chk_Busy();

		//Write_Dir(0x10,0x0F);//16Bit
		Foreground_Color(Red);
		Background_color(Green);
		BTE_Source_Destination(0,250,0,250);
		BTE_Size_setting(120,100);
		BTE_ROP_Code(0x79);	//	set BTE color expansion with transparency
		//BTE_ROP_Code(0xF9);
		BTE_enable();
		CmdWrite(0x02);//MRWC

		//for(i=0;i<750;i++){
		//	temp = rabit[i*2];
		//	temp<<=8;
		//	temp&=0xff00;
		//	temp= temp | rabit[i*2+1];
		//	DataWrite(temp);
		//	Chk_Busy();
		//}
		for(i=0;i<1500;i++){
			DataWrite(rabit[i]);
			Chk_Busy();
		}
		//Chk_Busy();

		Chk_Busy_BTE();

		Write_Dir(0x10,0x0F);//16Bit
		Foreground_Color(Yellow);
		Background_color(Purple);
		BTE_Source_Destination(0,650,0,350);
		BTE_Size_setting(128,128);
		BTE_ROP_Code(0xF9); // set BTE color expansion
		BTE_enable();
		CmdWrite(0x02);//MRWC

	for(i=0;i<1024;i++)
	{
		temp = bw[i*2];
		temp<<=8;
		temp&=0xff00;
		temp= temp | bw[i*2+1];
		DataWrite(temp);
		Chk_Busy();
	}
	*/

	// Write_Dir(0x10,0x0F);//16Bit
	// while(1){
	// Scroll(10,80,10,80);
	// }

	/*
	CmdWrite(0x02);
	Chk_Busy();
	*/

	// Write_Dir(0x8E,0x80);	// Clean
	// CmdWrite(0x02);			// MRWC
	// KeyBoard_Int( );

	RTC_SetDateTime( &rtc );


	while ( 1 )
	{
		Graphic_Mode( );
		// lcd->clsw( FULLWINDOW );
		r.p1.x = 0;
		r.p1.y = 0;
		r.p2.x = 100;
		r.p2.y = 100;
		lcd->fillrect( r, Red, FILL );

		wait_ms( 1000 );

        Graphic_Mode( );
        Write_Dir( 0x8E, 0x80 ); 	//Clean
        CmdWrite( 0x02 );
        openBMP3( "bg.dat" );

        Text_Mode( );
		lcd->SetTextFontSize( 1, 1 );
		lcd->SetTextCursor( 400, 200 );
  		sprintf( buffer, "%d %d %d", rtc.hour, rtc.min, rtc.sec );

        lcd->puts( buffer );

        wait_ms( 1000 );

		RTC_GetDateTime( &rtc );

		printf( "\r\nTime %d/%d/%d %d %d:%d:%d", rtc.year, rtc.month, rtc.date, rtc.weekDay, rtc.hour, rtc.min, rtc.sec );
	}
}


void openBMP3( const char * filename )
{
	UINT			t	= 0,j = 0;
	UINT			dcnt = 0;

	result	= f_open( &fsrc,( const char * ) filename,FA_OPEN_EXISTING | FA_READ );

	if ( result == FR_OK )
	{
		// printf("[Read File OK]\r\n");
	}
	else
	{
		printf( "\r\nSD Card Read File Failed" );
	}

	for ( t = 0; t < 15; t++ )
	{
		result				= f_read( &fsrc,SD_ReadBuf,51200,&dcnt );

		for ( j = 0; j < 25600; j++ )
		{
			EBI_LCD->EBI_LCD_RAM = SD_ReadBuf[j * 2 + 0] << 8 | SD_ReadBuf[j * 2 + 1];
		}
	}

	f_close( &fsrc );
}


u16 ToColor( u8 data1,u8 data2 )
{
	return data1 << 8 | data2;
}
