#ifndef _RA8875_H_

#define _RA8875_H_

#include "ht32f12365_66.H"
#include "DisplayDefs.h"

#define RA8875_DEFAULT_SPI_FREQ 5000000

// Define this to enable code that monitors the performance of various
// graphics commands.
//#define PERF_METRICS

// What better place for some test code than in here and the companion
// .cpp file. See also the bottom of this file.
//#define TESTENABLE

/// DOS colors - slightly color enhanced
#define Black       (color_t)(RGB(0,0,0))
#define Blue        (color_t)(RGB(0,0,187))
#define Green       (color_t)(RGB(0,187,0))
#define Cyan        (color_t)(RGB(0,187,187))
#define Red         (color_t)(RGB(187,0,0))
#define Magenta     (color_t)(RGB(187,0,187))
#define Brown       (color_t)(RGB(63,63,0))
#define Gray        (color_t)(RGB(187,187,187))
#define Charcoal    (color_t)(RGB(85,85,85))
#define BrightBlue  (color_t)(RGB(0,0,255))
#define BrightGreen (color_t)(RGB(0,255,0))
#define BrightCyan  (color_t)(RGB(0,255,255))
#define BrightRed   (color_t)(RGB(255,0,0))
#define Orange      (color_t)(RGB(255,85,85))
#define Pink        (color_t)(RGB(255,85,255))
#define Yellow      (color_t)(RGB(187,187,0))
#define White       (color_t)(RGB(255,255,255))

#define DarkBlue    (color_t)(RGB(0,0,63))
#define DarkGreen   (color_t)(RGB(0,63,0))
#define DarkCyan    (color_t)(RGB(0,63,63))
#define DarkRed     (color_t)(RGB(63,0,0))
#define DarkMagenta (color_t)(RGB(63,0,63))
#define DarkBrown   (color_t)(RGB(63,63,0))
#define DarkGray    (color_t)(RGB(63,63,63))

#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a>b)?a:b)


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

	u16 orig_x;
	u16 orig_y;

	u16 move_x;
	u16 move_y;

}_PicturePack;

/// cursor type to be shown as the text cursor.
typedef enum
{
    NOCURSOR,   ///< cursor is hidden
    IBEAM,      ///< | cursor
    UNDER,      ///< _ cursor
    BLOCK       ///< Block cursor

} cursor_t;

/// font type selection.
typedef enum
{
    ISO8859_1,      ///< ISO8859-1 font
    ISO8859_2,      ///< ISO8859-2 font
    ISO8859_3,      ///< ISO8859-3 font
    ISO8859_4       ///< ISO8859-4 font
} font_t;

/// display orientation
typedef enum
{
    normal,         ///< normal (landscape) orientation
    rotate_0 = normal,  ///< alternate to 'normal'
    rotate_90,      ///< rotated clockwise 90 degree
    rotate_180,     ///< rotated (clockwise) 180 degree
    rotate_270,     ///< rotated clockwise 270 degree
} orientation_t;

/// alignment
typedef enum
{
    align_none,     ///< align - none
    align_full      ///< align - full
} alignment_t;

/// Font Horizontal Scale factor - 1, 2, 3 4
typedef int HorizontalScale;

/// Font Vertical Scale factor - 1, 2, 3, 4
typedef int VerticalScale;

/// Clear screen region
typedef enum
{
    FULLWINDOW,     ///< Full screen
    ACTIVEWINDOW    ///< active window/region
} Region_t;

/// Set the Layer Display Mode. @ref SetLayerMode
typedef enum
{
    ShowLayer0,         ///< Only layer 0 is visible, layer 1 is hidden (default)
    ShowLayer1,         ///< Only layer 1 is visible, layer 0 is hidden
    LightenOverlay,     ///< Lighten-overlay mode
    TransparentMode,    ///< Transparent mode
    BooleanOR,          ///< Boolean OR mode
    BooleanAND,         ///< Boolean AND mode
    FloatingWindow      ///< Floating Window mode
} LayerMode_T;

/// Touch Panel modes
typedef enum
{
    TP_Auto,               ///< Auto touch detection mode
    TP_Manual,             ///< Manual touch detection mode
} tpmode_t;

/// printscreen callback commands
typedef enum
{
    OPEN,       ///< command to open the file. cast uint32_t * to the buffer to get the total size to be written.
    WRITE,      ///< command to write some data, buffer points to the data and the size is in bytes.
    CLOSE,      ///< command to close the file
} filecmd_t;


typedef struct
{

	/// get the screen width in pixels
    ///
    /// @returns screen width in pixels.
    ///
	dim_t ( *width )( void );

    /// get the screen height in pixels
    ///
    /// @returns screen height in pixels.
    ///
	dim_t ( *height )(void);


	/// Draw a rectangle in the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param[in] rect defines the rectangle.
    /// @param[in] color defines the foreground color.
    /// @param[in] fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    /// RetCode_t ( *rect )( rect_t rect, color_t color, fill_t fillit = NOFILL );
    RetCode_t ( *rect )( rect_t rect, color_t color, fill_t fillit );

	/// Draw a filled rectangle in the specified color
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param[in] rect defines the rectangle.
    /// @param[in] color defines the foreground color.
    /// @param[in] fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
	RetCode_t ( *fillrect )( rect_t r, color_t color, fill_t fillit );


    /// Draw a rectangle with rounded corners using the specified color.
    ///
    /// This draws a rounded rectangle. A numbers of checks are made on the values,
    /// and it could reduce this to drawing a line (if either x1 == x2, or y1 == y2),
    /// or a single point (x1 == x2 && y1 == y2). If the radius parameters are
    /// > 1/2 the length of that side (width or height), an error value is returned.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param[in] r is the rectangle to draw.
    /// @param[in] radius1 defines the horizontal radius of the curved corner. Take care
    ///         that this value < 1/2 the width of the rectangle, or bad_parameter
    ///         is returned.
    /// @param[in] radius2 defines the vertical radius of the curved corner. Take care
    ///         that this value < 1/2 the height of the rectangle, or bad_parameter
    ///         is returned.
    /// @param[in] color defines the foreground color.
    /// @param[in] fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    /// RetCode_t roundrect(rect_t r,
    ///    dim_t radius1, dim_t radius2, color_t color, fill_t fillit = NOFILL);
    RetCode_t ( *roundrect )( rect_t r, dim_t radius1, dim_t radius2, color_t color, fill_t fillit );


    /// Draw a filled rectangle with rounded corners using the specified color.
    ///
    /// This draws a rounded rectangle. A numbers of checks are made on the values,
    /// and it could reduce this to drawing a line (if either x1 == x2, or y1 == y2),
    /// or a single point (x1 == x2 && y1 == y2). If the radius parameters are
    /// > 1/2 the length of that side (width or height), an error value is returned.
    ///
    /// @note As a side effect, this changes the current
    ///     foreground color for subsequent operations.
    ///
    /// @param[in] x1 is the horizontal start of the line and must be <= x2.
    /// @param[in] y1 is the vertical start of the line and must be <= y2.
    /// @param[in] x2 is the horizontal end of the line and must be >= x1.
    /// @param[in] y2 is the vertical end of the line and must be >= y1.
    /// @param[in] radius1 defines the horizontal radius of the curved corner. Take care
    ///         that this value < 1/2 the width of the rectangle, or bad_parameter
    ///         is returned.
    /// @param[in] radius2 defines the vertical radius of the curved corner. Take care
    ///         that this value < 1/2 the height of the rectangle, or bad_parameter
    ///         is returned.
    /// @param[in] color defines the foreground color.
    /// @param[in] fillit is optional to FILL the rectangle. default is NOFILL.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    /// RetCode_t *fillroundrect( loc_t x1, loc_t y1, loc_t x2, loc_t y2, dim_t radius1, dim_t radius2, color_t color, fill_t fillit = FILL);
    RetCode_t ( *fillroundrect )( rect_t r, dim_t radius1, dim_t radius2, color_t color, fill_t fillit );

	/// Prepare the controller to write text to the screen by positioning
    /// the cursor.
    ///
    /// @code
    ///     lcd.SetTextCursor(100, 25);
    ///     lcd.puts("Hello");
    /// @endcode
    ///
    /// @param[in] x is the horizontal position in pixels (from the left edge)
    /// @param[in] y is the vertical position in pixels (from the top edge)
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    RetCode_t ( *SetTextCursor )( loc_t x, loc_t y );


    /// Set the foreground color.
    ///
    /// @param[in] color is expressed in 16-bit format.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    RetCode_t ( *foreground )( color_t color );

    /// Set the background color.
    ///
    /// @param[in] color is expressed in 16-bit format.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    RetCode_t ( *background )( color_t color );

    /// get the width in pixels of the currently active font
    ///
    /// @returns font width in pixels.
    ///
    dim_t ( *fontwidth )( void );


    /// get the height in pixels of the currently active font
    ///
    /// @returns font height in pixels.
    ///
    dim_t ( *fontheight )( void );


	/// Get the currently selected user font.
    ///
    /// @returns a pointer to the font, or null, if no user font is selected.
    ///
    const uint8_t* ( *GetUserFont )( void );

    /// Configure additional Cursor Control settings.
    ///
    /// This API lets you modify other cursor control settings;
    /// Cursor visible/hidden, Cursor blink/normal,
    /// Cursor I-Beam/underscore/box.
    ///
    /// @param[in] cursor can be set to NOCURSOR (default), IBEAM,
    ///         UNDER, or BLOCK.
    /// @param[in] blink can be set to true or false (default false)
    /// @returns success/failure code. See @ref RetCode_t
    ///
    /// RetCode_t SetTextCursorControl( cursor_t cursor = NOCURSOR, bool blink = false );
    RetCode_t ( *SetTextCursorControl )( cursor_t cursor, bool blink );

    /// Get the text font size of the RA8875 internal fonts.
    ///
    /// This command lets you retrieve the current settings for the font
    /// horizontal and vertical scale factors. The return value is
    /// one of the scale factors 1, 2, 3, or 4.
    ///
    /// @param[out] hScale is a pointer to memory where the horizontal scale factor
    ///     will be written. If the pointer is null, that item will be ignored.
    /// @param[out] vScale is a pointer to memory where the vertical scale factor
    ///     will be written. If the pointer is null, that item will be ignored.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    RetCode_t ( *GetTextFontSize )( HorizontalScale * hScale, VerticalScale * vScale );


    /// Control the font size of the RA8875 internal fonts.
    ///
    /// This command lets you set the font enlargement for both horizontal
    /// and vertical, independent of the rotation, background, and
    /// alignment. See @ref SetTextFontControl.
    ///
    /// @note This command only operates on the RA8875 internal fonts.
    ///
    /// @param[in] hScale defaults to 1, but can be 1, 2, 3, or 4,
    ///     and scales the font size by this amount.
    /// @param[in] vScale is an optional parameter that defaults to the hScale value,
    ///     but can be 1, 2, 3, or 4, and scales the font size by this amount.
    ///
    /// @code
    ///     lcd.SetTextFontSize(2);     // Set the font to 2x normal size
    ///     lcd.puts("Two times");
    ///     lcd.SetTextFontSize(2,3);   // Set the font to 2x Width and 3x Height
    ///     lcd.puts("2*2 3*h");
    ///     lcd.SetTextFontSize();      // Restore to normal size in both dimensions
    ///     lcd.puts("normal");
    /// @endcode
    ///
    /// @note if either hScale or vScale is outside of its permitted range,
    ///     the command is not executed.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    /// RetCode_t ( *SetTextFontSize )( HorizontalScale hScale = 1, VerticalScale vScale = -1 );
    RetCode_t ( *SetTextFontSize )( HorizontalScale hScale, VerticalScale vScale );

    /// Get the current cursor position in pixels.
	///
	/// @code
	/// 	point_t point = GetTextCursor();
	/// 	if (point.x > 100 && point.y > 150)
	/// 		//...
	/// @endcode
	///
	/// @returns cursor position.
	///
	point_t ( *GetTextCursor )( void );


    /// put a character on the screen.
    ///
    /// @param[in] c is the character.
    /// @returns the character, or EOF if there is an error.
    ///
    int ( *_putc )( int c );



}_RA8875;


extern void DrawPictureFromSD( const char *filename, u8 *dst_ptr, u16 x, u16 y );

extern _RA8875 * RA8875_CreateObj( void );

#endif
