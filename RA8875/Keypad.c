#include "ht32.h"
#include "ht32_board.h"
#include <string.h>
#include "Keypad.h"
#include "RA8875.h"

#define true					1
#define false					0

// byte pairs: size, symbol [,size, symbol [, size, symbol [...]]]
// size is a scale factor where 10 = "normal calculated size" based on the keyboard width and the # of keys, 5 is 1/2 width, etc.
// symbol is either the ASCII character to show, when leveraging the RA8875 internal fonts, or it is a special
//    character [normally non-printable] that is intercepted to perform an operation (e.g. <bs>, <shift>, etc.)
//    \x01: special no-key value, to leave whitespace before the next key.
const char kpd_shiftkeys[] = {
    5,0x01, 8,0x15,10,'~',10,'!',10,'@',10,'#',10,'$',10,'%',10,'^',10,'&',10,'*',10,'(',10,')',10,'_',   10,'+',10,'\x1B',0,0,
    5,0x01,12,0x1A,10,'Q',10,'W',10,'E',10,'R',10,'T',10,'Y',10,'U',10,'I',10,'O',10,'P',10,'{',   10,'}',10,'|',0,0,
    5,0x01,15,' ',10,'A',10,'S',10,'D',10,'F',10,'G',10,'H',10,'J',10,'K',10,'L',10,':',10,'"',   17,'\x1C',0,0,
    5,0x01,21,0x18,10,'Z',10,'X',10,'C',10,'V',10,'B',10,'N',10,'M',10,'<',10,'>',10,'?',21,0x18,0,0,
    5,0x01,40,0x01,55,' ',0,0,
    0,0
};
const char kpd_unshiftkeys[] = {
    5,0x01, 8,0x15,10,'`',10,'1',10,'2',10,'3',10,'4',10,'5',10,'6',10,'7',10,'8',10,'9',10,'0',10,'-',   10,'=',10,'\x1B',0,0,
    5,0x01,12,0x1A,10,'q',10,'w',10,'e',10,'r',10,'t',10,'y',10,'u',10,'i',10,'o',10,'p',10,'[',   10,']',10,'\\',0,0,
    5,0x01,15,' ',10,'a',10,'s',10,'d',10,'f',10,'g',10,'h',10,'j',10,'k',10,'l',10,';',10,'\'',  17,'\x1C',0,0,
    5,0x01,21,0x18,10,'z',10,'x',10,'c',10,'v',10,'b',10,'n',10,'m',10,',',10,'.',10,'/',21,0x18,0,0,
    5,0x01,40,0x01,55,' ',0,0,
    0,0
};




const keyboard_t internalkbd =
{
	0,-1,											// x,y: 0; start at left edge, -1; top is calculated from # rows

	//	   -1 is size calculated
	0,												// wi,h: width is calculated
	0,												//		bottom of screen justified
	5,												// 5 rows
	15, 											// columns in the <esc>`1234...-=<bs> row
	kpd_unshiftkeys,
	kpd_shiftkeys
};


_RA8875 *ra;
_KEYPAD KEYPAD_Obj;

const unsigned char * restore_font; // save user font
const unsigned char * user_font; // keyboard restore user font

int restore_hScale; // save scale
int restore_vScale;
int user_font_scale; // keyboard font scale

const keyboard_t *kbd;

char enter_key;
char esc_key;
bool shift;

char * pStart;
char * pNext;
int bufSize;

point_t userText;
color_t fore;
color_t back;

/// Draw a key in the specified rectangle. Invert the colors if desired.
///
/// @param[in] r is the rectangle defining the key.
/// @param[in] c is the character to draw.
/// @param[in] invert is the optional parameter to cause the presentation to be inverted.
///
/// void DrawKey( rect_t r,char c,bool invert = false );
void DrawKey( rect_t r, char c, bool invert );

/// Draw the entire keypad
///
void DrawKeypad( void );

/// Draw the entire input panel which includes the prompt, edit area and keyboard.
///
/// @param[in] prompt is the text string to print to hint to the user what you
/// 		   want them to enter the text for. It is good to keep this very brief.
///
void DrawInputPanel( const char * prompt );

/// isKeyTouched scans the keyboard for the point of touch. If the touch is
/// within the constraints of a key, that key is identifed.
///
/// This API optionally provides the rectangle from which the touch occurred.
///
/// @param[in] point is a pointer to a point_t, containing the touch coordinates.
/// @param[inout] rectTouched is a pointer to a rect_t. If this pointer is not NULL,
/// 			  it will be filled in with the rectangle within which the touch
/// 			  was detected.
/// returns the character at that location on the keyboard, or zero, if no key
/// 		was touched.
///
// char isKeyTouched( point_t *point, rect_t *rectTouched = NULL );
char isKeyTouched( point_t * point,rect_t * rectTouched );

/// Show the current edit buffer metrics - number of characters and the limit.
///
void ShowBufferMetrics( void );

/// Compute the coordinates for the keypad
///
/// @returns rect_t for the keypad.
///
rect_t ComputeKeypadRect( void );


bool SetKeyboard( const keyboard_t * _kbd,char _enter_key,char _esc_key )
{
	if ( _kbd )
	{
		kbd = _kbd;
	}
	else
	{
		kbd = &internalkbd;
	}

	enter_key = _enter_key;

	esc_key = _esc_key;

	return TRUE;
}


bool SetKeyboardFont( const unsigned char *_font, int _fontscale )
{
	if ( _font )
	{
		user_font = _font;
	}
	else
	{
		user_font_scale = _fontscale;
	}

	return TRUE;
}


void DrawKey( rect_t r, char c, bool invert )
{

	printf( "\r\nvoid DrawKey( rect_t r, char c, bool invert )" );

	ra->fillroundrect( r, 2, 2, invert ?fore:back, NOFILL );

	ra->roundrect( r, 2, 2, invert ? back: fore, NOFILL );


	printf( "\r\ny:%d", r.p1.y + 2 );
	printf( "\r\nx:%d", r.p1.x + ( r.p2.x - r.p1.x ) / 2 - ra->fontwidth() / 2 );

	ra->SetTextCursor( r.p1.x + ( r.p2.x - r.p1.x ) / 2 - ra->fontwidth() / 2, r.p1.y + 2 );

	if ( invert )
	{
		ra->foreground( back );
		ra->background( fore );

		ra->SetTextCursorControl( NOCURSOR, FALSE );
	}

	ra->putc( c );

	if ( invert )
	{
		ra->foreground( fore );
		ra->background( back );
	}

}


void Erase( color_t c )
{
	/*

	dim_t			kH	= ra.fontheight() + 4;
	rect_t			r	= ComputeKeypadRect();

	ra.fillrect( r,c );

	if ( user_font )
	{
		ra.SelectUserFont( restore_font ); // restore
	}
	else if ( restore_hScale | restore_vScale )
	{
		ra.SetTextFontSize( restore_hScale,restore_vScale );
	}

	//printf("Restore: font: %p, scale: %d,%d\r\n", user_font, restore_hScale, restore_vScale);

	*/
}


void DrawInputPanel( const char * prompt )
{

	rect_t r;

	if ( user_font || user_font_scale )
	{
		// save to later restore
		restore_font = ra->GetUserFont();

		ra->GetTextFontSize( &restore_hScale, &restore_vScale );

		printf( "\r\nSave : font: %p, scale: %d,%d", user_font, restore_hScale, restore_vScale );
	}

	if ( !user_font && user_font_scale )
	{
		ra->SetTextFontSize( user_font_scale, -1 );
	}

	r = ComputeKeypadRect();

	ra->fillrect( r, back, NOFILL );

	ra->foreground( fore );
	ra->background( back );

	ra->SetTextCursor( r.p1.x, r.p1.y + 2 );

	ra->puts( prompt );
	// ra->puts( "12345" );

	userText = ra->GetTextCursor();

	DrawKeypad( );

}


rect_t ComputeKeypadRect( void )
{
	dim_t kH = ra->fontheight() + 4;
	rect_t r;


	printf( "\r\nComputeKeypadRect kbd->x %d", kbd->x  );

	printf( "\r\nComputeKeypadRect kbd->y %d", kbd->y  );

	if ( kbd->x <= 0 )
	{
		r.p1.x = 0;
	}
	else
	{
		r.p1.x = kbd->x;
	}

	if ( kbd->y <= 0 )
	{
		r.p1.y = ra->height() - ( kH * ( kbd->rows + 1 ) ) - 1;
	}
	else
	{
		r.p1.y = kbd->y;
	}

	if ( kbd->width == 0 )
	{
		r.p2.x = ra->width() - 1;
	}
	else
	{
		r.p2.x = r.p1.x + kbd->width - 1;
	}

	if ( kbd->height == 0 )
	{
		r.p2.y = ra->height() - 1;
	}
	else
	{
		r.p2.y = r.p1.y + kbd->height - 1;
	}

	// r.p1.x = 10;
	// r.p1.y = 100;
	// r.p2.x = 300;
	// r.p2.y = 300;

	printf( "\r\nra->height() %d", ra->height() );
	printf( "\r\nra->width() %d", ra->width() );
	printf( "\r\nkbd->height %d", kbd->height );
	printf( "\r\nkbd->width %d", kbd->width );

	printf( "\r\nKeypadRect (%d,%d) - (%d,%d)", r.p1.x, r.p1.y, r.p2.x, r.p2.y );

	return r;


}


void DrawKeypad( void )
{
	// dim_t fW;
	dim_t fH;
	dim_t kW;										// key width
	dim_t kH;										// key Height

    const char *p = kbd->keydef1;
    rect_t ref, r;
    const char *symbol;

	// fW = ra.fontwidth();
	fH	= ra->fontheight();

	kH	= fH + 4;

	r = ComputeKeypadRect();

    ref = r;

	kW	= ( ref.p2.x - ref.p1.x ) / ( kbd->cols + 1 );

	printf( "\r\nDrawKeypad kW=%d from (%d,%d) %d", kW, ref.p2.x, ref.p1.x, kbd->cols + 1 );

	r.p1.y += kH;

	if ( shift )
	{
		p = kbd->keydef2;
	}

	while ( *p || *( p + 2 ) )
	{
		if ( *p == 0 )
		{
			r.p1.x = ref.p1.x;
			r.p1.y += kH;
		}
		else
		{
			symbol = p + 1;

			if ( *symbol >= 0x10 )
			{
				r.p2.x = r.p1.x + ( kW * *p ) / 10;
				r.p2.y = r.p1.y + kH;

				DrawKey( r, *symbol, TRUE );
			}

			r.p1.x += ( kW * *p ) / 10;
		}

		p += 2;

	}

}


char isKeyTouched( point_t * point,rect_t * rectTouched )
{

#if 1
	dim_t kW;
	dim_t kH = ra->fontheight( ) + 4;

	const char * p = kbd->keydef1;
	rect_t r = ComputeKeypadRect();
	rect_t ref = r;

	kW	= ( ref.p2.x - ref.p1.x ) / ( kbd->cols + 1 );
	r.p1.y = r.p1.y + kH;

	if ( shift )
	{
		p	= kbd->keydef2;
	}

	while ( *p || * ( p + 2 ) )
	{
		if ( *p == 0 )
		{
			r.p1.x = ref.p1.x;
			r.p1.y += kH;
		}
		else
		{
			const char * symbol = p + 1;

			if ( *symbol >= 0x10 )
			{
				r.p2.x = r.p1.x + ( kW * *p ) / 10;
				r.p2.y = r.p1.y + kH;

				if ( ra->Intersect( r, *point ) )
				{
					if ( rectTouched )
					{
						*rectTouched = r;
					}

					return ( *symbol );
				}
			}

			r.p1.x += ( kW * *p ) / 10;
		}

		p	+= 2;
	}

	return 0;

#endif
}


void ShowBufferMetrics( void )
{

	rect_t r = ComputeKeypadRect();

	ra->SetTextCursor( r.p2.x - 1 - ra->fontwidth() * 5,r.p1.y + 2 );

	ra->printf( "%02d/%02d", pNext - pStart,bufSize ); // "001/100"

	ra->SetTextCursor( ra->width( ) - 1 - ra->fontwidth() * 7, ra->height() - 1 - ra->fontheight( ) );

	ra->SetTextCursor( userText.x,userText.y );

	ra->SetTextCursorControl( UNDER, FALSE );
}


bool GetString( char *buffer, unsigned char size, const char *prompt, bool initFromBuffer, char mask, bool auto_close )
{
	point_t point =
	{
		0,0
	};

    TouchCode_t is;

    rect_t touchRect;

	bool success = FALSE;

	pStart = pNext = buffer;
	bufSize = size;

	DrawInputPanel( prompt );

	if ( initFromBuffer && ( strlen( buffer ) < size ) )
	{
		ra->SetTextCursor( userText.x, userText.y );

		ra->puts( buffer );

		userText = ra->GetTextCursor();
		pNext += strlen( buffer );
	}

	ShowBufferMetrics( );

#if 1
	while ( 1 )
	{
		if ( touch == ra->TouchPanelReadable( &point ) )
		{
			// find out if they touched a key;
			char key = isKeyTouched( &point, &touchRect );

			if ( key )
			{
				DrawKey( touchRect, key, TRUE );

				// wait_ms( 50 );

				do
				{
					is	= ra->TouchPanelReadable( NULL );

					printf( "is: %d\r\n", is );
				}
				while( is != no_touch );

				DrawKey( touchRect, key, FALSE );
			}

			printf( "\r\nTouch %02X at (%d,%d)", key, point.x, point.y );
			printf( "\r\nTouch %02X at (%d,%d)", key, point.x, point.y );

			if ( key == enter_key )
			{
				*pNext = '\0';

				ra->SetTextCursorControl( NOCURSOR, FALSE );

				success = TRUE;

				break;
			}
			else if ( key == esc_key )
			{
				*buffer = '\0';

				ra->SetTextCursorControl( NOCURSOR, FALSE );

				success = FALSE;

				break;
			}
			else if ( key == KYBD_SYM_BS )
			{
				if ( pNext > buffer )
				{
					pNext--;

					// blank the last char
					userText.x -= ra->fontwidth();

					ra->SetTextCursor( userText.x, userText.y );

					ra->putc( ' ' );
				}
			}
			else if ( key == KYBD_SYM_TAB )
			{
				*pNext++ = ' ';

				ra->SetTextCursor( userText.x,userText.y );

				ra->putc( ' ' );

				userText.x += ra->fontwidth();
			}
			else if ( key == KYBD_SYM_SHIFT )
			{
				shift = !shift;

				DrawKeypad();
			}
			else if ( key )
			{
				*pNext++ = key;

				ra->SetTextCursor( userText.x,userText.y );

				if ( mask )
				{
					ra->putc( mask );
				}
				else
				{
					ra->putc( key );
				}

				userText.x += ra->fontwidth();
			}

			if ( ( pNext - buffer ) >= ( size - 1 ) )
			{
				*pNext = '\0';

				ra->SetTextCursorControl( NOCURSOR, FALSE );

				success = TRUE;

				break;
			}

			ShowBufferMetrics();
		}

	}

	if ( auto_close )
	{
		Erase( back );
	}

#endif

	return success;
}


/// Get a handle to the keyboard currently in use.
///
/// This returns a handle to the currently active keyboard. If you first SelectKeyboard(NULL),
/// and then call GetKeyboard(), it will return a handle to the built-in keyboard.
///
/// This API might be used if you want to use the built-in keyboard definition, but
/// resize it meet your needs.
///
/// @returns a handle to the currently active keyboard.
///
const keyboard_t * GetKeyboard( void )
{
	return kbd;
}


_KEYPAD *Keypad_CreateObj( color_t _fore, color_t _back )
{
	_KEYPAD *obj = &KEYPAD_Obj;

	ra = RA8875_CreateObj( );

	shift = FALSE;
    enter_key = KYBD_SYM_ENTER;
    esc_key   = KYBD_SYM_ESCAPE;
    user_font = NULL;
    user_font_scale = 0;

	fore = _fore;
	back = _back;

	obj->shift = FALSE;

	obj->enter_key = KYBD_SYM_ENTER;

	obj->esc_key = KYBD_SYM_ESCAPE;

	obj->user_font = NULL;

	obj->user_font_scale = 0;

	obj->SetKeyboard = SetKeyboard;

	obj->SetKeyboardFont = SetKeyboardFont;

	obj->GetKeyboard = GetKeyboard;

	obj->GetString = GetString;


	return obj;
}



