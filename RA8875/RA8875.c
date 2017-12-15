#include "ht32.h"
#include "ht32_board.h"
#include "./FatFS/tff.h"
#include "RA8875.h"
#include "Converter.h"

_PicturePack PicturePack;

_RA8875 RA8875_Obj;

uint8_t screenbpp;              ///< configured bits per pixel
dim_t screenwidth;              ///< configured screen width
dim_t screenheight;             ///< configured screen height
rect_t windowrect;              ///< window commands are held here for speed of access
bool portraitmode;              ///< set true when in portrait mode (w,h are reversed)

const unsigned char * font;     ///< reference to an external font somewhere in memory
uint8_t extFontHeight;          ///< computed from the font table when the user sets the font
uint8_t extFontWidth;           ///< computed from the font table when the user sets the font

loc_t cursor_x, cursor_y;       ///< used for external fonts only


_PicturePack * LoadSD_DataDescription( const char * filename, u8 *dst_ptr )
{
    FIL 			fsrc; 	/* File object		  */
    UINT			dcnt = 0;

	FRESULT result = FR_NOT_READY;

	result = f_open( &fsrc,( const char * ) filename, FA_OPEN_EXISTING | FA_READ );

	if ( result == FR_OK )
	{
		printf( "\r\n%s Read File OK", filename );
	}
	else
	{
		printf( "\r\n%s Read File Failed", filename );
	}

	#if 1
	result = f_read( &fsrc, dst_ptr, 4, &dcnt );

	PicturePack.width.packet.hi = *dst_ptr;

	PicturePack.width.packet.lo = *( dst_ptr + 1 );

	PicturePack.lenght.packet.hi = *( dst_ptr + 2 );

	PicturePack.lenght.packet.lo = *( dst_ptr + 3 );

	f_close( &fsrc );


	SWAP_U16( &( PicturePack.width.word_data )  );

	SWAP_U16( &( PicturePack.lenght.word_data )  );
	#endif

    #if 0
    PicturePack.width.word_data  = 168;
    PicturePack.lenght.word_data = 168;
    #endif

	PicturePack.size = ( PicturePack.width.word_data ) * ( PicturePack.lenght.word_data ) << 1;

	printf( "\r\npicture width : %d, lenght : %d, size : %d", PicturePack.width.word_data,
	                                                          PicturePack.lenght.word_data,
	                                                          PicturePack.size );
	return &PicturePack;

}

void LoadFile( const char *filename, u8 *dst_ptr, DWORD file_index, u16 size )
{
    FIL 			fsrc; 	/* File object		  */
    UINT			dcnt = 0;

	FRESULT result = FR_NOT_READY;

	/*
	if( file_index > 0xFFFF )
	{
		printf( "Picture Overflow" );

		return;
	}
	*/

	result = f_open( &fsrc,( const char * ) filename, FA_OPEN_EXISTING | FA_READ );

	if ( result == FR_OK )
	{
		printf( "\r\n%s Read File OK", filename );
	}
	else
	{
		printf( "\r\n%s Read File Failed", filename );
	}

	f_lseek( &fsrc, file_index );

	result = f_read( &fsrc, dst_ptr, size, &dcnt );

	f_close( &fsrc );

	PicturePack.data_ptr = ( _SD_WORD * )( dst_ptr );

	printf( "\r\nPicturePack.data_ptr[ 0 ] 0x%x ", PicturePack.data_ptr[ 0 ].word_data );
	printf( "\r\nPicturePack.data_ptr[ 1 ] 0x%x ", PicturePack.data_ptr[ 1 ].word_data );

}


void DrawPictuure( _PicturePack *pic, u16 size  )
{
	u32 index = 0;

    printf( "\r\npicture width : %d, lenght : %d, size : %d", pic->width.word_data,
	                                                          pic->lenght.word_data,
	                                                          pic->size );

	printf( "\r\npixcel 10 11 12: %x, %x %x",   pic->data_ptr[ 10 ].word_data,
	                                      		pic->data_ptr[ 11 ].word_data,
	                                      		pic->data_ptr[ 12 ].word_data );

	do
	{

		SWAP_U16( &( pic->data_ptr[ index ].word_data )  );

		// Chk_Busy();

		XY_Coordinate( pic->move_x, pic->move_y );

		CmdWrite( 0x02 );

		EBI_LCD->EBI_LCD_RAM = ( u16 )( pic->data_ptr[ index ].word_data );

		// EBI_LCD->EBI_LCD_RAM = Red;

		index ++;
		pic->move_x += 1;
		size --;

		if( pic->move_x >= ( pic->width.word_data + pic->orig_x ) )
		{
			pic->move_y += 1;
			pic->move_x = pic->orig_x;
		}

	}while( size );

}







void DrawPictureFromSD( const char * filename, u8 *dst_ptr, u16 x, u16 y )
{
	_PicturePack *obj;

	u32 len;

	u32 file_index = 4;

	// u32 ram_size = 51200;
	u32 ram_size = 25600;

	obj = LoadSD_DataDescription( filename, dst_ptr );

	obj->orig_x = x;
	obj->orig_y = y;

	obj->move_x = x;
	obj->move_y = y;

	do
	{
		len = ( ( obj->size ) >= ram_size )?( ram_size ) :( obj->size );

		LoadFile( filename, dst_ptr, file_index, len );

		file_index += ( len );

		obj->size -= ( len );
        // ------------------------------

		printf( "\r\nLoad SD len : %d %d %d ", len, obj->size, file_index );

		DrawPictuure( obj, ( len >> 1 ) );

	}while( obj->size  );
}


dim_t fontheight( void )
{
    if( font == NULL )
    {
        // return ( ( ( ReadCommand( 0x22 ) >> 0 ) & 0x3 ) + 1 ) * 16;
		return ( ( ( DataRead( 0x22 ) >> 0 ) & 0x3 ) + 1 ) << 4;
	}
    else
    {
        // return extFontHeight;
    }
}

dim_t width( void )
{
    if( portraitmode )
        return screenheight;
    else
        return screenwidth;
}


dim_t height( void )
{
    if( portraitmode )
        return screenwidth;
    else
        return screenheight;
}


_RA8875 * RA8875_CreateObj( void )
{
	_RA8875 *obj = &RA8875_Obj;

	obj->fontheight = fontheight;

	obj->height = height;

	obj->width = width;

	return obj;
}







