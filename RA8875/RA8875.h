#ifndef _RA8875_H_

#define _RA8875_H_

#include "ht32f12365_66.H"
#include "DisplayDefs.h"
#include "RA8875_Regs.h"

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

////////////////// Start of Resistive Touch Panel parameters

/// Resistive Touch Panel register name definitions
#define TPCR0   0x70
#define TPCR1   0x71
#define TPXH    0x72
#define TPYH    0x73
#define TPXYL   0x74
#define INTC1   0xF0
#define INTC2   0xF1

/// Specify the default settings for the Touch Panel, where different from the chip defaults
#define TP_MODE_DEFAULT             TP_MODE_AUTO
#define TP_DEBOUNCE_DEFAULT         TP_DEBOUNCE_ON
#define TP_ADC_CLKDIV_DEFAULT       TP_ADC_CLKDIV_8

#define TP_ADC_SAMPLE_DEFAULT_CLKS  TP_ADC_SAMPLE_8192_CLKS

/// Other Touch Panel params
#define TPBUFSIZE   16       // Depth of the averaging buffers for x and y data

// Needs both a ticker and a timer. (could have created a timer from the ticker, but this is easier).
// on a touch, the timer is reset.
// the ticker monitors the timer to see if it has been a long time since
// a touch, and if so, it then clears the sample counter so it doesn't get partial old
// and partial new.

/// Touch Panel ticker
// Ticker touchTicker;

/// Touch Panel timer
// Timer touchTimer;



/// Private function for touch ticker callback.
void _TouchTicker(void);
////////////////// End of Touch Panel parameters



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

/// Idle reason provided in the Idle Callback
typedef enum
{
	unknown,            ///< reason has not been assigned (this should not happen)
	status_wait,        ///< driver is polling the status register while busy
	command_wait,       ///< driver is polling the command register while busy
	getc_wait,          ///< user has called the getc function
	touch_wait,         ///< user has called the touch function
	touchcal_wait,      ///< driver is performing a touch calibration
	progress,           ///< communicates progress

} IdleReason_T;



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
    int ( *putc )( int c );


    /// Write string of text to the display
    ///
    /// @code
    ///     lcd.puts("Test STring");
    /// @endcode
    ///
    /// @param[in] string is the null terminated string to send to the display.
    ///
    void ( *puts )( const char *string );


	/************************
	 * @brief Print function.
	 * @param f: Format string.
	 * @retval String length.
	 ************************/
    signed int ( *printf )( const char *f, ... );

    /// Poll the TouchPanel and on a touch event return the a to d filtered x, y coordinates.
    ///
    /// This method reads the touch controller, which has a 10-bit range for each the
    /// x and the y axis.
    ///
    /// @note The returned values are not in display (pixel) units but are in analog to
    ///     digital converter units.
    ///
    /// @note This API is usually not needed and is likely to be deprecated.
    ///     See @ref TouchPanelComputeCalibration.
    ///     See @ref TouchPanelReadable.
    ///
    /// @param[out] x is the x scale a/d value.
    /// @param[out] y is the y scale a/d value.
    /// @returns a value indicating the state of the touch,
    ///         - no_cal:   no calibration matrix is available, touch coordinates are not returned.
    ///         - no_touch: no touch is detected, touch coordinates are not returned.
    ///         - touch:    touch is detected, touch coordinates are returned.
    ///         - held:     held after touch, touch coordinates are returned.
    ///         - release:  indicates a release, touch coordinates are returned.
    ///
    TouchCode_t ( *TouchPanelA2DFiltered )( int *x, int *y );


    /// Poll the TouchPanel and on a touch event return the a to d raw x, y coordinates.
    ///
    /// This method reads the touch controller, which has a 10-bit range for each the
    /// x and the y axis. A number of samples of the raw data are taken, filtered,
    /// and the results are returned.
    ///
    /// @note The returned values are not in display (pixel) units but are in analog to
    ///     digital converter units.
    ///
    /// @note This API is usually not needed and is likely to be deprecated.
    ///     See @ref TouchPanelComputeCalibration.
    ///     See @ref TouchPanelReadable.
    ///
    /// @param[out] x is the x scale a/d value.
    /// @param[out] y is the y scale a/d value.
    /// @returns a value indicating the state of the touch,
    ///         - no_cal:   no calibration matrix is available, touch coordinates are not returned.
    ///         - no_touch: no touch is detected, touch coordinates are not returned.
    ///         - touch:    touch is detected, touch coordinates are returned.
    ///         - held:     held after touch, touch coordinates are returned.
    ///         - release:  indicates a release, touch coordinates are returned.
    ///
    TouchCode_t ( *TouchPanelA2DRaw )( int *x, int *y );


    /// Wait for a touch panel touch and return it.
    ///
    /// This method is similar to Serial.getc() in that it will wait for a touch
    /// and then return. In order to extract the coordinates of the touch, a
    /// valid pointer to a point_t must be provided.
    ///
    /// @note There is no timeout on this function, so its use is not recommended.
    ///
    /// @code
    ///     Timer t;
    ///     t.start();
    ///     do {
    ///        point_t point = {0, 0};
    ///        display.TouchPanelGet(&point);   // hangs here until touch
    ///        display.pixel(point, Red);
    ///    } while (t.read_ms() < 30000);
    /// @endcode
    ///
    /// @param[out] TouchPoint is the touch point, if a touch is registered.
    /// @returns a value indicating the state of the touch,
    ///         - no_cal:   no calibration matrix is available, touch coordinates are not returned.
    ///         - no_touch: no touch is detected, touch coordinates are not returned.
    ///         - touch:    touch is detected, touch coordinates are returned.
    ///         - held:     held after touch, touch coordinates are returned.
    ///         - release:  indicates a release, touch coordinates are returned.
    ///
    TouchCode_t ( *TouchPanelGet )( point_t *TouchPoint );


    /// Calibrate the touch panel.
    ///
    /// This method accepts two lists - one list is target points in ,
    /// display coordinates and the other is a lit of raw touch coordinate
    /// values. It generates a calibration matrix for later use. This
    /// matrix is also accessible to the calling API, which may store
    /// the matrix in persistent memory and then install the calibration
    /// matrix on the next power cycle. By doing so, it can avoid the
    /// need to calibrate on every power cycle.
    ///
    /// @note The methods "TouchPanelComputeCalibration", "TouchPanelReadable", and
    ///     indirectly the "TouchPanelSetMatrix" methods are all derived
    ///     from a program by Carlos E. Vidales. See the copyright note
    ///     for further details. See also the article
    ///     http://www.embedded.com/design/system-integration/4023968/How-To-Calibrate-Touch-Screens
    ///
    /// @copyright Copyright (c) 2001, Carlos E. Vidales. All rights reserved.
    ///     This sample program was written and put in the public domain
    ///      by Carlos E. Vidales.  The program is provided "as is"
    ///      without warranty of any kind, either expressed or implied.
    ///     If you choose to use the program within your own products
    ///      you do so at your own risk, and assume the responsibility
    ///      for servicing, repairing or correcting the program should
    ///      it prove defective in any manner.
    ///     You may copy and distribute the program's source code in any
    ///      medium, provided that you also include in each copy an
    ///      appropriate copyright notice and disclaimer of warranty.
    ///     You may also modify this program and distribute copies of
    ///      it provided that you include prominent notices stating
    ///      that you changed the file(s) and the date of any change,
    ///      and that you do not charge any royalties or licenses for
    ///      its use.
    ///
    /// @param[in] display is a pointer to a set of 3 points, which
    ///             are in display units of measure. These are the targets
    ///             the calibration was aiming for.
    /// @param[in] screen is a pointer to a set of 3 points, which
    ///             are in touchscreen units of measure. These are the
    ///             registered touches.
    /// @param[out] matrix is an optional parameter to hold the calibration matrix
    ///             as a result of the calibration. This can be saved in
    ///             non-volatile memory to recover the calibration after a power fail.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    /// RetCode_t ( *TouchPanelComputeCalibration )( point_t display[3], point_t screen[3], tpMatrix_t * matrix );
	RetCode_t ( *TouchPanelComputeCalibration )( point_t *display, point_t *screen, tpMatrix_t *matrix );

    /// Perform the touch panel calibration process.
    ///
    /// This method provides the easy "shortcut" to calibrating the touch panel.
    /// The process will automatically generate the calibration points, present
    /// the targets on-screen, detect the touches, compute the calibration
    /// matrix, and optionally provide the calibration matrix to the calling code
    /// for persistence in non-volatile memory.
    ///
    /// @param[in] msg is a text message to present on the screen during the
    ///             calibration process.
    /// @param[out] matrix is an optional parameter to hold the calibration matrix
    ///             as a result of the calibration. This can be saved in
    ///             non-volatile memory to recover the calibration after a power fail.
    /// @param[in] maxwait_s is the maximum number of seconds to wait for a touch
    ///             calibration. If no touch panel installed, it then reports
    ///             touch_cal_timeout.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
	/// RetCode_t TouchPanelCalibrate(const char * msg, tpMatrix_t * matrix = NULL, int maxwait_s = 15);
	RetCode_t ( *TouchPanelCalibrate )( const char *msg, tpMatrix_t *matrix, int maxwait_s );

    /// Set the calibration matrix for the touch panel.
    ///
    /// This method is used to set the calibration matrix for the touch panel. After
    /// performing the calibration (See @ref TouchPanelComputeCalibration), the matrix can be stored.
    /// On a subsequence power cycle, the matrix may be restored from non-volatile and
    /// passed in to this method. It will then be held to perform the corrections when
    /// reading the touch panel point.
    ///
    /// @code
    /// FILE * fh = fopen("/local/tpmatrix.cfg", "r");
    /// if (fh) {
    ///     tpMatrix_t matrix;
    ///     if (fread(fh, &matrix, sizeof(tpMatrix_t))) {
    ///         lcd.TouchPanelSetMatrix(&matrix);
    ///     }
    ///     fclose(fh);
    /// }
    /// @endcode
    ///
    /// @param[in] matrix is a pointer to the touch panel calibration matrix.
    /// @returns success/failure code. See @ref RetCode_t.
    ///
    /// RetCode_t TouchPanelSetMatrix(tpMatrix_t * matrix);
	RetCode_t ( *TouchPanelSetMatrix )( tpMatrix_t *matrix );

	/// Get the screen calibrated point of touch.
    ///
    /// This method determines if there is a touch and if so it will provide
    /// the screen-relative touch coordinates. This method can be used in
    /// a manner similar to Serial.readable(), to determine if there was a
    /// touch and indicate that - but not care about the coordinates. Alternately,
    /// if a valid pointer to a point_t is provided, then if a touch is detected
    /// the point_t will be populated with data.
    ///
    /// @code
    ///     Timer t;
    ///     t.start();
    ///     do {
    ///        point_t point = {0, 0};
    ///        if (display.TouchPanelReadable(&point)) {
    ///            display.pixel(point, Red);
    ///        }
    ///    } while (t.read_ms() < 30000);
    /// @endcode
    ///
    /// @param[out] TouchPoint is a pointer to a point_t, which is set as the touch point, if a touch is registered.
    /// @returns a value indicating the state of the touch,
    ///         - no_cal:   no calibration matrix is available, touch coordinates are not returned.
    ///         - no_touch: no touch is detected, touch coordinates are not returned.
    ///         - touch:    touch is detected, touch coordinates are returned.
    ///         - held:     held after touch, touch coordinates are returned.
    ///         - release:  indicates a release, touch coordinates are returned.
    ///
    /// TouchCode_t TouchPanelReadable(point_t * TouchPoint = NULL);
    TouchCode_t ( *TouchPanelReadable )( point_t * TouchPoint );

    /// Determine if a point is within a rectangle.
    ///
    /// @param[in] rect is a rectangular region to use.
    /// @param[in] p is a point to analyze to see if it is within the rect.
    /// @returns true if p is within rect.
    ///
    bool ( *Intersect )( rect_t rect, point_t p );


}_RA8875;

extern RetCode_t cls( uint16_t layers );

extern dim_t height( void );

extern dim_t width( void );

extern RetCode_t _writeColorTrio( u8 regAddr, color_t color );

extern void ra_puts( const char * string );

extern RetCode_t line( loc_t x1, loc_t y1, loc_t x2, loc_t y2 );

extern RetCode_t foreground( color_t color );

extern RetCode_t WriteCommand( u8 command, u16 data );

extern RetCode_t SetTextCursor(     loc_t x, loc_t y );

extern void DrawPictureFromSD( const char *filename, u8 *dst_ptr, u16 x, u16 y );

extern _RA8875 *RA8875_CreateObj( void );

#endif
