#include "ht32.h"
#include "ht32_board.h"
#include "common/ebi_lcd.h"
#include "./FatFS/tff.h"
#include "RA8875.h"
#include "RA8875_Touch.h"
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


dim_t fontheight( void )
{
    if( font == NULL )
    {
        // return ( ( ( ReadCommand( 0x22 ) >> 0 ) & 0x3 ) + 1 ) * 16;
		return ( ( ( DataRead( 0x22 ) >> 0 ) & 0x3 ) + 1 ) << 4;
	}
    else
    {
        return extFontHeight;
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


RetCode_t WriteCommand( u8 command, u16 data )
{
	/*
    _select(true);

    _spiwrite( 0x80 );            // RS:1 (Cmd/Status), RW:0 (Write)

    _spiwrite( command );

    if( data <= 0xFF )
    {
    	// only if in the valid range
        _spiwrite(0x00);
        _spiwrite(data);
    }

	 _select(false);
    */

    CmdWrite( command );

    DataWrite( data );

    return noerror;
}


RetCode_t WriteCommandW( uint8_t command, uint16_t data )
{
    WriteCommand( command, data & 0xFF );

    WriteCommand( command+1, data >> 8 );

    return noerror;
}



unsigned char ReadStatus( void )
{
    unsigned char data;

    // _spiwrite(0xC0);            // RS:1 (Cmd/Status), RW:1 (Read) (Read STSR)

    // data = _spiread();

	data = LCD_StatusRead( );

    return data;
}


RetCode_t _writeColorTrio( u8 regAddr, color_t color )
{
    RetCode_t rt = noerror;

	/*
    if (screenbpp == 16)
    {
        WriteCommand(regAddr+0, (color>>11));                  // BGCR0
        WriteCommand(regAddr+1, (unsigned char)(color>>5));    // BGCR1
        rt = WriteCommand(regAddr+2, (unsigned char)(color));  // BGCR2

    }
    else
    {
        uint8_t r, g, b;

        // RRRR RGGG GGGB BBBB      RGB
        // RRR   GGG    B B
        r = (uint8_t)((color) >> 13);
        g = (uint8_t)((color) >> 8);
        b = (uint8_t)((color) >> 3);
        WriteCommand(regAddr+0, r);  // BGCR0
        WriteCommand(regAddr+1, g);  // BGCR1
        rt = WriteCommand(regAddr+2, b);  // BGCR2
    }
    */

    WriteCommand( regAddr + 0, ( color >>11 ) );        // BGCR0

    WriteCommand( regAddr + 1, ( u8 )( color >>5 ) );   // BGCR1

    rt = WriteCommand( regAddr + 2, ( u8 )( color ) );  // BGCR2

    return rt;
}

color_t _readColorTrio( u8 regAddr )
{
    color_t color;

    u8 r, g, b;

    r = DataRead( regAddr + 0 );
	g = DataRead( regAddr + 1 );
	b = DataRead( regAddr + 2 );

	// 000R RRRR 00GG GGGG 000B BBBB
	// RRRR RGGG GGGB BBBB
	color  = (r & 0x1F) << 11;
	color |= (g & 0x3F) << 5;
	color |= (b & 0x1F);

	/*

	r = ReadCommand( regAddr + 0  );
    g = ReadCommand( regAddr + 1 );
    b = ReadCommand( regAddr + 2 );

    if (screenbpp == 16)
    {
        // 000R RRRR 00GG GGGG 000B BBBB
        // RRRR RGGG GGGB BBBB
        color  = (r & 0x1F) << 11;
        color |= (g & 0x3F) << 5;
        color |= (b & 0x1F);
    } else {
        // RRRG GGBB
        // RRRR RGGG GGGB BBBB
        color  = (r & 0x07) << 13;
        color |= (g & 0x07) << 8;
        color |= (b & 0x03) << 3;
    }

	*/

    return color;
}


color_t GetForeColor( void )
{
    return _readColorTrio( 0x63 );

}


RetCode_t SetGraphicsCursor( loc_t x, loc_t y )
{

    WriteCommand(0x46, x);
    WriteCommand(0x48, y);

    return noerror;
}

RetCode_t _StartGraphicsStream( void )
{
    WriteCommand( 0x40, 0x00 );    	// Graphics write mode

    CmdWrite( 0x02 );         		// Prepare for streaming data

    return noerror;
}

RetCode_t _EndGraphicsStream( void )
{
    return noerror;
}

/// @todo add a timeout and return false, but how long
/// to wait since some operations can be very long.
bool _WaitWhileReg( uint8_t reg, uint8_t mask )
{
	#if 1

    // int i = 20000 / POLLWAITuSec; // 20 msec max

    int i = 20000;

    while ( i-- && DataRead( reg ) & mask )
    {
        // wait_us( POLLWAITuSec );
		wait_ms( 1 );

		/*
        COUNTIDLETIME(POLLWAITuSec);

        if ( idle_callback )
        {
            if ( external_abort == (*idle_callback)( command_wait, 0 ) )
            {
                return false;
            }
        }
        */

    }

    if( i )
        return TRUE;
    else
        return FALSE;

    #endif
}


/// @todo add a timeout and return false, but how long
/// to wait since some operations can be very long.
bool _WaitWhileBusy( uint8_t mask )
{
    // int i = 20000/POLLWAITuSec; // 20 msec max
	int i = 0xFFFFF;

    while ( i && ( ReadStatus( ) & mask ) )
    {
		i --;

        __NOP;

		wait_ms( 1 );
        // wait_us(POLLWAITuSec);

        // COUNTIDLETIME(POLLWAITuSec);

        /*
        if (idle_callback)
        {
            if (external_abort == (*idle_callback)(status_wait, 0))
            {
                return false;
            }
        }
        */
    }

    if( i )
        return TRUE;
    else
        return FALSE;

}


RetCode_t background( color_t color )
{
    // GraphicsDisplay::background(color);

    return _writeColorTrio( 0x60, color );
}

RetCode_t foreground( color_t color )
{
    // GraphicsDisplay::foreground(color);

    return _writeColorTrio( 0x63, color );
}


RetCode_t pixelStream( color_t * p, uint32_t count, loc_t x, loc_t y )
{

    SetGraphicsCursor( x, y );

    _StartGraphicsStream( );

    /*
    _select( true );

    _spiwrite(0x00);         // Cmd: write data

    while (count--)
    {

		_spiwrite(*p >> 8);
		_spiwrite(*p & 0xFF);

        p++;
    }

    _select(false);
    */

   	while (count--)
    {
		DataWrite( *p );
        p++;
    }

    _EndGraphicsStream();

    return( noerror );
}


RetCode_t pixel( loc_t x, loc_t y )
{
    RetCode_t ret;

    color_t color = GetForeColor( );

    ret = pixelStream( &color, 1, x, y );

    return ret;
}

RetCode_t line( loc_t x1, loc_t y1, loc_t x2, loc_t y2 )
{
	unsigned char drawCmd = 0x00;       // Line

    if ( x1 == x2 && y1 == y2 )
    {
        pixel( x1, y1 );
    }
    else
    {
        WriteCommandW( 0x91, x1 );
        WriteCommandW( 0x93, y1 );
        WriteCommandW( 0x95, x2 );
        WriteCommandW( 0x97, y2 );

        WriteCommand( 0x90, drawCmd );
        WriteCommand( 0x90, 0x80 + drawCmd ); // Start drawing.

        if ( !_WaitWhileReg(0x90, 0x80) )
        {
            return external_abort;
        }
    }

    return noerror;
}




RetCode_t rect( loc_t x1, loc_t y1, loc_t x2, loc_t y2, fill_t fillit )
{
    RetCode_t ret = noerror;
    unsigned char drawCmd = 0x10;   // Rectangle

    // printf( "\r\nDraw rect top left %d %d", x1, y1 );

    // printf( "\r\nDraw rect bot right %d %d", x2, y2 );

    // printf( "\r\nDraw rect fill %d", fillit );

    // check for bad_parameter
    if( x1 < 0 || x1 >= screenwidth || x2 < 0 || x2 >= screenwidth
     || y1 < 0 || y1 >= screenheight || y2 < 0 || y2 >= screenheight )
    {
        ret = bad_parameter;

        // printf( "\r\nDraw rect bad_parameter" );
    }
    else
    {
        if( x1 == x2 && y1 == y2 )
        {
            pixel( x1, y1 );

        }
        else if( x1 == x2 )
        {
            line( x1, y1, x2, y2 );
        }
        else if ( y1 == y2 )
        {
            line( x1, y1, x2, y2 );

        }
        else
        {
            WriteCommandW( 0x91, x1 );
            WriteCommandW( 0x93, y1 );
            WriteCommandW( 0x95, x2 );
            WriteCommandW( 0x97, y2 );

            if ( fillit == FILL ) drawCmd |= 0x20;

            WriteCommand( 0x90, drawCmd );

            ret = WriteCommand( 0x90, 0x80 + drawCmd ); // Start drawing.

            if( !_WaitWhileReg( 0x90, 0x80 ) )
            {
                return external_abort;
            }
        }
    }

    return ret;
}


//
// Rectangle functions all mostly helpers to the basic rectangle function
//

RetCode_t fillrect( rect_t r, color_t color, fill_t fillit )
{
	foreground( color );

    return rect( r.p1.x, r.p1.y, r.p2.x, r.p2.y, fillit );
}

dim_t fontwidth( void )
{
	// printf( "\r\nfontwidth" );

    if ( font == NULL )
        return ( ( ( DataRead( 0x22 ) >> 2 ) & 0x3 ) + 1 ) * 8;
    else
        return extFontWidth;
}


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

RetCode_t SetTextCursor(     loc_t x, loc_t y )
{
    // INFO("SetTextCursor(%d, %d)", x, y);

	// set these values for non-internal fonts
    cursor_x = x;
    cursor_y = y;

    WriteCommandW( 0x2A, x );
    WriteCommandW( 0x2C, y );

    return noerror;
}

RetCode_t SetTextCursorControl( cursor_t cursor, bool blink )
{
    unsigned char mwcr0 = DataRead( 0x40 ) & 0x0F; // retain direction, auto-increase
    unsigned char mwcr1 = DataRead( 0x41 ) & 0x01; // retain currently selected layer

    unsigned char horz = 0;
    unsigned char vert = 0;

    mwcr0 |= 0x80;                  // text mode

    if( cursor != NOCURSOR ) mwcr0 |= 0x40;              // visible

    if( blink ) mwcr0 |= 0x20;              // blink


    WriteCommand(0x40, mwcr0);      // configure the cursor
    WriteCommand(0x41, mwcr1);      // close the graphics cursor
    WriteCommand(0x44, 0x1f);       // The cursor flashing cycle

    switch ( cursor )
    {
        case IBEAM:
            horz = 0x01;
            vert = 0x1F;
            break;
        case UNDER:
            horz = 0x07;
            vert = 0x01;
            break;
        case BLOCK:
            horz = 0x07;
            vert = 0x1F;
            break;
        case NOCURSOR:
        default:
            break;
    }

    WriteCommand(0x4e, horz);       // The cursor size horz
    WriteCommand(0x4f, vert);       // The cursor size vert

    return noerror;
}


RetCode_t SetTextFont( font_t font)
{
    if ( /*font >= RA8875::ISO8859_1 && */ font <= ISO8859_4 )
    {
        WriteCommand(0x21, (unsigned int)(font) );

        return noerror;
    }
    else
    {
        return bad_parameter;
    }
}

const uint8_t* GetUserFont( void )
{
	// printf( "\r\nGetUserFont" );

	return font;
}

RetCode_t GetTextFontSize( HorizontalScale *hScale, VerticalScale *vScale )
{

	// Font Control Register1 22h
    unsigned char reg = DataRead( 0x22 );


    if (hScale)
        *hScale = 1 + (reg >> 2) & 0x03;
    if (vScale)
        *vScale = 1 + reg & 0x03;

    return noerror;
}




RetCode_t SetTextFontSize( HorizontalScale hScale, VerticalScale vScale )
{
    unsigned char reg = DataRead( 0x22 );

    // printf( "\r\nRetCode_t SetTextFontSize( HorizontalScale hScale, VerticalScale vScale )" );

    if (vScale == -1)
        vScale = hScale;

    if ( hScale >= 1 && hScale <= 4 && vScale >= 1 && vScale <= 4)
    {
        reg &= 0xF0;    // keep the high nibble as is.
        reg |= ((hScale - 1) << 2);
        reg |= ((vScale - 1) << 0);

        WriteCommand( 0x22, reg );

        // printf( "\r\nSetTextFontSize return noerror" );

        return noerror;
    }
    else
    {
		// printf( "\r\nSetTextFontSize return bad_parameter" );

        return bad_parameter;
    }

}

loc_t GetTextCursor_Y(void)
{
    loc_t y;

    if (font == NULL)
        y = DataRead( 0x2C ) | (DataRead(0x2D) << 8);
    else
        y = cursor_y;

    return y;
}


loc_t GetTextCursor_X(void)
{
    loc_t x;

    if ( font == NULL )
        x = DataRead( 0x2A ) | ( DataRead( 0x2B ) << 8 );
    else
        x = cursor_x;

    return x;
}

point_t GetTextCursor( void )
{
    point_t p;

    p.x = GetTextCursor_X( );
    p.y = GetTextCursor_Y( );

    return p;
}

// Questions to ponder -
// - if we choose to wrap to the next line, because the character won't fit on the current line,
//      should it erase the space to the width of the screen (in case there is leftover junk there)?
// - it currently wraps from the bottom of the screen back to the top. I have pondered what
//      it might take to scroll the screen - but haven't thought hard enough about it.
//
int _external_putc(int c)
{
	/*
    if (c)
    {
        if (c == '\r') {
            cursor_x = windowrect.p1.x;
        } else if (c == '\n') {
            cursor_y += extFontHeight;
        } else {
            dim_t charWidth, charHeight;
            const uint8_t * charRecord;

            charRecord = getCharMetrics( c, &charWidth, &charHeight );
            // int advance = charwidth(c);
            // INFO("(%d,%d) - (%d,%d):(%d,%d), charWidth: %d '%c", cursor_x, cursor_y,
                windowrect.p1.x, windowrect.p1.y, windowrect.p2.x, windowrect.p2.y,
                charWidth, c);
            if (charRecord) {
                //cursor_x += advance;
                if (cursor_x + charWidth >= windowrect.p2.x) {
                    cursor_x = windowrect.p1.x;
                    cursor_y += charHeight;
                }
                if (cursor_y + charHeight >= windowrect.p2.y) {
                    cursor_y = windowrect.p1.y;               // @todo Should it scroll?
                }
                (void)character(cursor_x, cursor_y, c);
                cursor_x += charWidth;
            }
        }
    }
    */

    return c;
}


int _internal_putc(int c)
{
	u8 state = 0;

    if( c )
    {
        unsigned char mwcr0;

        mwcr0 = DataRead( 0x40 );

        if ( ( mwcr0 & 0x80 ) == 0x00 )
        {
            WriteCommand( 0x40, 0x80 | mwcr0 );    // Put in Text mode if not already
        }

        if( c == '\r' )
        {
            loc_t x;

            x = DataRead( 0x30 ) | ( DataRead( 0x31 ) << 8 );   // Left edge of active window

            WriteCommandW( 0x2A, x );
        }
        else if( c == '\n' )
        {
            loc_t y;

            y = DataRead( 0x2C ) | ( DataRead( 0x2D ) << 8 );   // current y location

            y += fontheight( );

            if ( y >= height( ) )               // @TODO after bottom of active window, then scroll window?
                y = 0;

            WriteCommandW( 0x2C, y );
        }
        else
        {
            CmdWrite( 0x02 );                // RA8875 Internal Fonts

            DataWrite( c );

            state = _WaitWhileBusy( 0x80 );

			// Delay Problem
            // printf( "\r\n_WaitWhileBusy state : %d", state );

        }
    }

    return c;
}


int _putc( int c )
{
    if( font == NULL )
    {
        return _internal_putc( c );
    }
    else
    {
        return _external_putc( c );
    }
}

void ra_puts( const char * string )
{
    if ( font == NULL )
    {
        WriteCommand( 0x40,0x80 );    // Put in Text mode if internal font
    }

    if ( *string != '\0' )
    {
        while( *string != '\0' )
        {
        	// @TODO calling individual _putc is slower... optimizations?
            _putc( *string++ );

            // printf( "\r\n*string %s", "A" );

            // printf( "\r\n*string %s", *string );
        }
    }
}



RetCode_t roundrect( loc_t x1, loc_t y1, loc_t x2, loc_t y2, dim_t radius1, dim_t radius2, color_t color, fill_t fillit )
{
    RetCode_t ret = noerror;
    u8 drawCmd = 0x20;       // Rounded Rectangle

    foreground( color );


    if( x1 < 0 || x1 >= screenwidth || x2 < 0 || x2 >= screenwidth
     || y1 < 0 || y1 >= screenheight || y2 < 0 || y2 >= screenheight )
    {
        ret = bad_parameter;
    }
    else if (x1 > x2 || y1 > y2 || (radius1 > (x2-x1)/2) || (radius2 > (y2-y1)/2) )
    {
        ret = bad_parameter;
    }
    else if ( x1 == x2 && y1 == y2 )
    {
        pixel( x1, y1 );
    }
    else if ( x1 == x2 )
    {
        line( x1, y1, x2, y2 );
    }
    else if ( y1 == y2 )
    {
        line( x1, y1, x2, y2 );
    }
    else
    {
        WriteCommandW( 0x91, x1 );
        WriteCommandW( 0x93, y1 );
        WriteCommandW( 0x95, x2 );
        WriteCommandW( 0x97, y2 );

        WriteCommandW( 0xA1, radius1 );
        WriteCommandW( 0xA3, radius2 );

        // Should not need this...
        WriteCommandW(0xA5, 0);
        WriteCommandW(0xA7, 0);

        if (fillit == FILL) drawCmd |= 0x40;

        WriteCommand(0xA0, drawCmd);
        WriteCommand(0xA0, 0x80 + drawCmd); // Start drawing.

        if( !_WaitWhileReg(0xA0, 0x80) )
        {
            return external_abort;
        }
    }

    return ret;
}

//
// rounded rectangle functions are mostly helpers to the base round rect
//
RetCode_t fillroundrect( rect_t r, dim_t radius1, dim_t radius2, color_t color, fill_t fillit )
{
    return roundrect( r.p1.x, r.p1.y, r.p2.x, r.p2.y , radius1, radius2, color, fillit );
}

RetCode_t SelectDrawingLayer( u16 layer /* , u16 *prevLayer */ )
{
	u16 *prevLayer = NULL;

    unsigned char mwcr1 = DataRead( 0x41 ); // retain all but the currently selected layer

    if( prevLayer )
    	*prevLayer = mwcr1 & 1;

    mwcr1 &= ~0x01; // remove the current layer

    if( screenwidth >= 800 && screenheight >= 480 && screenbpp > 8 )
    {
        layer = 0;
    }
    else if( layer > 1 )
    {
        layer = 0;
    }

    return WriteCommand( 0x41, mwcr1 | layer );

}

u16 GetDrawingLayer( void )
{
    return ( DataRead( 0x41 ) & 0x01 );
}

RetCode_t locate( textloc_t column, textloc_t row )
{
    return SetTextCursor( column * fontwidth(), row * fontheight() );
}

RetCode_t clsw( Region_t region )
{

    WriteCommand( 0x8E, ( region == ACTIVEWINDOW ) ? 0xC0 : 0x80 );

    if ( !_WaitWhileReg( 0x8E, 0x80 ) )
    {
        return external_abort;
    }

    return noerror;
}

RetCode_t cls( uint16_t layers )
{
    RetCode_t ret;

    uint16_t prevLayer;


    if ( layers == 0 )
    {
        ret = clsw( FULLWINDOW );
    }
    else if (layers > 3)
    {
        ret = bad_parameter;
    }
    else
    {
        prevLayer = GetDrawingLayer();

        if ( layers & 1 )
        {
            SelectDrawingLayer( 0 );

            clsw( FULLWINDOW );
        }

        if (layers & 2)
        {
            SelectDrawingLayer(1);

            clsw(FULLWINDOW);
        }
        SelectDrawingLayer(prevLayer);
    }

    ret = SetTextCursor(0,0);
    ret = locate(0,0);

    return ret;
}

bool Intersect( rect_t rect, point_t p )
{
    if (p.x >= min(rect.p1.x, rect.p2.x) && p.x <= max(rect.p1.x, rect.p2.x)
    && p.y >= min(rect.p1.y, rect.p2.y) && p.y <= max(rect.p1.y, rect.p2.y))
        return TRUE;
    else
        return FALSE;
}



void LoadFile( const char *filename, u8 *dst_ptr, DWORD file_index, u16 size )
{
    FIL fsrc; 	/* File object		  */
    UINT dcnt = 0;

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

		// CmdWrite( 0x02 );

		_StartGraphicsStream( );

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


_RA8875 * RA8875_CreateObj( void )
{
	_RA8875 *obj = &RA8875_Obj;

	screenwidth = 800;
	screenheight = 480;
	portraitmode = FALSE;

	TouchPanelInit( );

	obj->height = height;

	obj->width = width;

	obj->fillrect = fillrect;

	obj->roundrect = roundrect;

	obj->fillroundrect = fillroundrect;

	obj->SetTextCursor = SetTextCursor;

	obj->SetTextCursorControl = SetTextCursorControl;

	obj->SetTextFontSize = SetTextFontSize;

	obj->GetUserFont = GetUserFont;

	obj->GetTextFontSize = GetTextFontSize;

	obj->GetTextCursor = GetTextCursor;

	obj->fontwidth = fontwidth;

	obj->foreground = foreground;

	obj->background = background;

	obj->fontheight = fontheight;

	obj->putc = _putc;

	obj->puts = ra_puts;

	obj->printf = printf;

	obj->TouchPanelCalibrate = TouchPanelCalibrate;

	obj->TouchPanelReadable = TouchPanelReadable;

	obj->Intersect = Intersect;

	obj->cls = cls;

	obj->clsw = clsw;

	return obj;
}







