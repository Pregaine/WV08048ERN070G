#include "ht32.h"
#include "ht32_board.h"
#include "./FatFS/tff.h"
#include "RA8875.h"
#include "Converter.h"

_PicturePack PicturePack;

_PicturePack * LoadSD_DatFile( const char * filename, u8 *dst_ptr )
{
    FIL 			fsrc; 	/* File object		  */
    UINT			dcnt = 0;

	FRESULT result = FR_NOT_READY;

	result = f_open( &fsrc,( const char * ) filename, FA_OPEN_EXISTING | FA_READ );

	if ( result == FR_OK )
	{
		printf( "[Read File OK]\r\n" );
	}
	else
	{
		printf( "[Read File Failed]\r\n" );
	}

	result = f_read( &fsrc, dst_ptr, 4, &dcnt );

	PicturePack.width.packet.hi = *dst_ptr;

	PicturePack.width.packet.lo = *( dst_ptr + 1 );

	PicturePack.lenght.packet.hi = *( dst_ptr + 2 );

	PicturePack.lenght.packet.lo = *( dst_ptr + 3 );

	SWAP_U16( &( PicturePack.width.word_data )  );

	SWAP_U16( &( PicturePack.lenght.word_data )  );

	PicturePack.size = ( PicturePack.width.word_data ) * ( PicturePack.lenght.word_data );

	printf( "\r\npicture width : %d, lenght : %d, size : %d", PicturePack.width.word_data,
	                                                          PicturePack.lenght.word_data,
	                                                          PicturePack.size );

	result = f_read( &fsrc, dst_ptr, PicturePack.size << 1, &dcnt );

	f_close( &fsrc );

	PicturePack.data_ptr = ( _SD_WORD * )( dst_ptr );

	printf( "\r\nPicturePack.data_ptr[ 8 / 9 ] 0x%x 0x%x", PicturePack.data_ptr[ 0 ].word_data, PicturePack.data_ptr[ 8 ].word_data   );

	return &PicturePack;

}

void DrawPictuure( _PicturePack *pic, u16 x, u16 y  )
{
	u16 index = 0;

	u8 pos_x = 0;

	do
	{
		XY_Coordinate( x, y );

		SWAP_U16( &( pic->data_ptr[ index ].word_data )  );

		CmdWrite( 0x02 );

		EBI_LCD->EBI_LCD_RAM = ( u16 )( pic->data_ptr[ index ].word_data );

		// EBI_LCD->EBI_LCD_RAM = Red;

		index ++;
		x ++;
		pos_x ++;

		if( pos_x >= pic->width.word_data )
		{
			y ++;
			pos_x = 0;
			x -= pic->width.word_data;
		}

	}while( pic->size -- );

}


void DrawPictureFromSD( const char * filename, u8 *dst_ptr, u16 x, u16 y )
{
	_PicturePack *obj;

	obj = LoadSD_DatFile( filename, dst_ptr );

	DrawPictuure( obj, x, y );
}




