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


	dim_t ( *fontheight )( void );

}_RA8875;


extern void DrawPictureFromSD( const char *filename, u8 *dst_ptr, u16 x, u16 y );

extern _RA8875 * RA8875_CreateObj( void );

#endif
