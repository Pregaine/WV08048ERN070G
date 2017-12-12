#ifndef _RA8875_H_

#define _RA8875_H_

typedef union
{
	struct
	{
        uint16_t hi:8;		// bit 8 ~ 15
		uint16_t lo:8;		// bit 0 ~ 7

	}packet;

	uint16_t word_data;

}_SD_WORD;


typedef struct
{
	_SD_WORD width;

	_SD_WORD lenght;

	_SD_WORD *data_ptr;

	u32 size;

}_PicturePack;


extern void DrawPictureFromSD( const char *filename, u8 *dst_ptr, u16 x, u16 y );

#endif
