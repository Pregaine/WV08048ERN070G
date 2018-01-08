#include <string.h>
#include "ht32.h"
#include "ht32_board.h"
#include "RA8875.h"
#include "RA8875_Touch.h"
#include "TimerManager.h"
#include "tsc2013.h"


#define NOTOUCH_TIMEOUT_uS 100	// 100000
#define TOUCH_TICKER_uS      	  1000

/// Touch Information data structure
typedef struct
{

	u8 			touchID;		///< Contains the touch ID, which is the "order" of touch, from 0 to n-1
	TouchCode_t touchCode;		///< Contains the touch code; no_touch, touch, held, release
	point_t 	coordinates;	///< Contains the X,Y coordinate of the touch

} touchInfo_T;

/// Touch panel parameters - common to both resistive and capacitive

/// Data type to indicate which TP, if any, is in use.
typedef enum
{
	TP_NONE,			///< no touch panel in use
	TP_RES, 			///< resistive touch panel using RA8875
	TP_CAP, 			///< capacitive touch panel using FT5206
} WhichTP_T;

/// boolean flag set true when using Capacitive touch panel, and false
/// for resistive.
WhichTP_T useTouchPanel;	///< Indicates which TP is selected for use.

/// Touch State used by TouchPanelReadable. See @ref TouchCode_t.
TouchCode_t touchState;

touchInfo_T touchInfo[5]; /// Contains the actual touch information in an array from 0 to n-1

/// Touch Panel calibration matrix.
tpMatrix_t tpMatrix;

/// keeps track of which sample we're collecting to filter out the noise.
int touchSample;

bool panelTouched;

u16 numberOfTouchPoints;
u8 gesture;            ///< Holds the reported gesture information.


RetCode_t( *idle_callback ) ( IdleReason_T reason,u16 param );

void _TouchTicker( void )
{
	#if 1
    if ( touchTimer_read( ) > NOTOUCH_TIMEOUT_uS )
    {
        // printf( "\r\n_TouchTicker" );

        touchSample = 0;

        if (touchState == held)
            touchState = release;
        else
            touchState = no_touch;

        touchTimer_reset();
    }
    #endif
}


RetCode_t TouchPanelInit( void )
{
    panelTouched = FALSE;

	/*
    if ( useTouchPanel == TP_CAP)
    {
        // Set to normal mode
        writeRegister8(FT5206_DEVICE_MODE, 0);
    }
    else
    {
        //TPCR0: Set enable bit, default sample time, wakeup, and ADC clock
        WriteCommand(TPCR0, TP_ENABLE | TP_ADC_SAMPLE_DEFAULT_CLKS | TP_ADC_CLKDIV_DEFAULT);
        // TPCR1: Set auto/manual, Ref voltage, debounce, manual mode params
        WriteCommand(TPCR1, TP_MODE_DEFAULT | TP_DEBOUNCE_DEFAULT);
        WriteCommand(INTC1, ReadCommand(INTC1) | RA8875_INT_TP);        // reg INTC1: Enable Touch Panel Interrupts (D2 = 1)
        WriteCommand(INTC2, RA8875_INT_TP);                            // reg INTC2: Clear any TP interrupt flag
        touchSample = 0;
        touchState = no_cal;
        //touchTicker.attach_us(callback(this, &RA8875::_TouchTicker), TOUCH_TICKER_uS);
        #if MBED_LIBRARY_VERSION > 122   // Is this the right version?
        touchTicker.attach_us(callback(this, &RA8875::_TouchTicker), TOUCH_TICKER_uS);
        #else
        touchTicker.attach_us(this, &RA8875::_TouchTicker, TOUCH_TICKER_uS);
        #endif

        touchTimer.start();
        touchTimer.reset();
    }
    */

	//TPCR0: Set enable bit, default sample time, wakeup, and ADC clock
	// WriteCommand( TPCR0, TP_ENABLE | TP_ADC_SAMPLE_DEFAULT_CLKS | TP_ADC_CLKDIV_DEFAULT );

	WriteCommand( TPCR0, TP_DISABLE | TP_ADC_SAMPLE_DEFAULT_CLKS | TP_ADC_CLKDIV_DEFAULT );

	// TPCR1: Set auto/manual, Ref voltage, debounce, manual mode params
	WriteCommand( TPCR1, TP_MODE_DEFAULT | TP_DEBOUNCE_DEFAULT );
	WriteCommand( INTC1, DataRead( INTC1 ) | RA8875_INT_TP );        // reg INTC1: Enable Touch Panel Interrupts (D2 = 1)
	WriteCommand( INTC2, RA8875_INT_TP );                            // reg INTC2: Clear any TP interrupt flag

	touchSample = 0;
	touchState  = no_cal;

	// touchTicker.attach_us(callback(this, &RA8875::_TouchTicker), TOUCH_TICKER_uS);

	// touchTicker.attach_us( this, &RA8875::_TouchTicker, TOUCH_TICKER_uS );

	// touchTimer.start();
	// touchTimer.reset();

    return noerror;
}



/**********************************************************************
 *
 *	   Function: TouchPanelReadable()
 *
 *	Description: Given a valid set of calibration factors and a point
 *				  value reported by the touch screen, this function
 *				  calculates and returns the true (or closest to true)
 *				  display point below the spot where the touch screen
 *				  was touched.
 *
 *
 *
 *	Argument(s): displayPtr (output) - Pointer to the calculated
 *										(true) display point.
 *				 screenPtr (input) - Pointer to the reported touch
 *									  screen point.
 *				 matrixPtr (input) - Pointer to calibration factors
 *									  matrix previously calculated
 *									  from a call to
 *									  setCalibrationMatrix()
 *
 *
 *	The function simply solves for Xd and Yd by implementing the
 *	 computations required by the translation matrix.
 *
 *												/-	   -\
 *				/-	  -\	 /- 		   -\	|		|
 *				|	   |	 |				|	|	Xs	|
 *				|  Xd  |	 | A	B	 C	|	|		|
 *				|	   |  =  |				| * |	Ys	|
 *				|  Yd  |	 | D	E	 F	|	|		|
 *				|	   |	 |				|	|	1	|
 *				\-	  -/	 \- 		   -/	|		|
 *												\-	   -/
 *
 *	It must be kept brief to avoid consuming CPU cycles.
 *
 *		 Return: OK - the display point was correctly calculated
 *					   and its value is in the output argument.
 *				 NOT_OK - an error was detected and the function
 *						   failed to return a valid point.
 *
 *				   NOTE!	NOTE!	 NOTE!
 *
 *	setCalibrationMatrix() and getDisplayPoint() will do fine
 *	for you as they are, provided that your digitizer
 *	resolution does not exceed 10 bits (1024 values).  Higher
 *	resolutions may cause the integer operations to overflow
 *	and return incorrect values.  If you wish to use these
 *	functions with digitizer resolutions of 12 bits (4096
 *	values) you will either have to a) use 64-bit signed
 *	integer variables and math, or b) judiciously modify the
 *	operations to scale results by a factor of 2 or even 4.
 *
 */
TouchCode_t TouchPanelReadable( point_t * TouchPoint )
{
	TouchCode_t ts = no_touch;

	useTouchPanel = TP_RES;

	if ( useTouchPanel == TP_RES )
	{
		int a2dX = 0;
		int a2dY = 0;

		touchInfo[0].touchID = 0;

		ts = TouchPanelA2DFiltered( &a2dX,&a2dY );

		// printf( "\r\nts = TouchPanelA2DFiltered( &a2dX,&a2dY );" );

		if ( ts != no_touch )
		{
			panelTouched = TRUE;

            numberOfTouchPoints = 1;

			if ( tpMatrix.Divider != 0 )
			{
				/* Operation order is important since we are doing integer */
				/*	math. Make sure you add all terms together before	   */
				/*	dividing, so that the remainder is not rounded off	   */
				/*	prematurely.										   */
				touchInfo[0].coordinates.x =
					 ( ( tpMatrix.An * a2dX ) + ( tpMatrix.Bn * a2dY ) +tpMatrix.Cn ) / tpMatrix.Divider;
				touchInfo[0].coordinates.y =
					 ( ( tpMatrix.Dn * a2dX ) + ( tpMatrix.En * a2dY ) +tpMatrix.Fn ) / tpMatrix.Divider;
			}
			else
			{
				ts	= no_cal;
			}
		}
		else
		{
			numberOfTouchPoints = 0;
		}

		touchInfo[0].touchCode = ts;
	}
	else /* (useTouchPanel == TP_CAP) */
	{
		;
	}

	if ( panelTouched == TRUE )
	{
		panelTouched = FALSE;

		if ( TouchPoint )
		{
			*TouchPoint = touchInfo[0].coordinates;
			ts	= touchInfo[0].touchCode;
		}
		else
		{
			ts	= touch;
		}
	}

	return ts;
}


static void InsertionSort( int * buf,int bufsize )
{
	int i, j;
	int temp;

	for ( i = 1; i < bufsize; i++ )
	{
		temp = buf[ i ];
		j = i;

		while ( j && ( buf[ j - 1 ] > temp ) )
		{
			buf[ j ] = buf[ j - 1 ];

			j = j - 1;
		}

		buf[ j ] = temp;
	} // End of sort
}


/*	 The following section is derived from Carlos E. Vidales.
 *
 *	 Copyright (c) 2001, Carlos E. Vidales. All rights reserved.
 *
 *	 This sample program was written and put in the public domain
 *	  by Carlos E. Vidales.  The program is provided "as is"
 *	  without warranty of any kind, either expressed or implied.
 *	 If you choose to use the program within your own products
 *	  you do so at your own risk, and assume the responsibility
 *	  for servicing, repairing or correcting the program should
 *	  it prove defective in any manner.
 *	 You may copy and distribute the program's source code in any
 *	  medium, provided that you also include in each copy an
 *	  appropriate copyright notice and disclaimer of warranty.
 *	 You may also modify this program and distribute copies of
 *	  it provided that you include prominent notices stating
 *	  that you changed the file(s) and the date of any change,
 *	  and that you do not charge any royalties or licenses for
 *	  its use.
 *
 *	 This file contains functions that implement calculations
 *	  necessary to obtain calibration factors for a touch screen
 *	  that suffers from multiple distortion effects: namely,
 *	  translation, scaling and rotation.
 *
 *	 The following set of equations represent a valid display
 *	  point given a corresponding set of touch screen points:
 *
 *												/-	   -\
 *				/-	  -\	 /- 		   -\	|		|
 *				|	   |	 |				|	|	Xs	|
 *				|  Xd  |	 | A	B	 C	|	|		|
 *				|	   |  =  |				| * |	Ys	|
 *				|  Yd  |	 | D	E	 F	|	|		|
 *				|	   |	 |				|	|	1	|
 *				\-	  -/	 \- 		   -/	|		|
 *												\-	   -/
 *	  where:
 *			 (Xd,Yd) represents the desired display point
 *					  coordinates,
 *			 (Xs,Ys) represents the available touch screen
 *					  coordinates, and the matrix
 *			 /-   -\
 *			 |A,B,C|
 *			 |D,E,F| represents the factors used to translate
 *			 \-   -/  the available touch screen point values
 *					  into the corresponding display
 *					  coordinates.
 *	  Note that for practical considerations, the utilities
 *	   within this file do not use the matrix coefficients as
 *	   defined above, but instead use the following
 *	   equivalents, since floating point math is not used:
 *			  A = An/Divider
 *			  B = Bn/Divider
 *			  C = Cn/Divider
 *			  D = Dn/Divider
 *			  E = En/Divider
 *			  F = Fn/Divider
 *	  The functions provided within this file are:
 *			setCalibrationMatrix() - calculates the set of factors
 *									  in the above equation, given
 *									  three sets of test points.
 *				 getDisplayPoint() - returns the actual display
 *									  coordinates, given a set of
 *									  touch screen coordinates.
 * translateRawScreenCoordinates() - helper function to transform
 *									  raw screen points into values
 *									  scaled to the desired display
 *									  resolution.
 */

/**********************************************************************
 *
 *	   Function: setCalibrationMatrix()
 *
 *	Description: Calling this function with valid input data
 *				  in the display and screen input arguments
 *				  causes the calibration factors between the
 *				  screen and display points to be calculated,
 *				  and the output argument - matrixPtr - to be
 *				  populated.
 *
 *				 This function needs to be called only when new
 *				  calibration factors are desired.
 *
 *
 *	Argument(s): displayPtr (input) - Pointer to an array of three
 *									   sample, reference points.
 *				 screenPtr (input) - Pointer to the array of touch
 *									  screen points corresponding
 *									  to the reference display points.
 *				 matrixPtr (output) - Pointer to the calibration
 *									   matrix computed for the set
 *									   of points being provided.
 *
 *
 *	From the article text, recall that the matrix coefficients are
 *	 resolved to be the following:
 *
 *
 *		Divider =  (Xs0 - Xs2)*(Ys1 - Ys2) - (Xs1 - Xs2)*(Ys0 - Ys2)
 *
 *
 *
 *				   (Xd0 - Xd2)*(Ys1 - Ys2) - (Xd1 - Xd2)*(Ys0 - Ys2)
 *			  A = ---------------------------------------------------
 *									 Divider
 *
 *
 *				   (Xs0 - Xs2)*(Xd1 - Xd2) - (Xd0 - Xd2)*(Xs1 - Xs2)
 *			  B = ---------------------------------------------------
 *									 Divider
 *
 *
 *				   Ys0*(Xs2*Xd1 - Xs1*Xd2) +
 *							   Ys1*(Xs0*Xd2 - Xs2*Xd0) +
 *											 Ys2*(Xs1*Xd0 - Xs0*Xd1)
 *			  C = ---------------------------------------------------
 *									 Divider
 *
 *
 *				   (Yd0 - Yd2)*(Ys1 - Ys2) - (Yd1 - Yd2)*(Ys0 - Ys2)
 *			  D = ---------------------------------------------------
 *									 Divider
 *
 *
 *				   (Xs0 - Xs2)*(Yd1 - Yd2) - (Yd0 - Yd2)*(Xs1 - Xs2)
 *			  E = ---------------------------------------------------
 *									 Divider
 *
 *
 *				   Ys0*(Xs2*Yd1 - Xs1*Yd2) +
 *							   Ys1*(Xs0*Yd2 - Xs2*Yd0) +
 *											 Ys2*(Xs1*Yd0 - Xs0*Yd1)
 *			  F = ---------------------------------------------------
 *									 Divider
 *
 *
 *		 Return: OK - the calibration matrix was correctly
 *					   calculated and its value is in the
 *					   output argument.
 *				 NOT_OK - an error was detected and the
 *						   function failed to return a valid
 *						   set of matrix values.
 *						  The only time this sample code returns
 *						  NOT_OK is when Divider == 0
 *
 *
 *
 *				   NOTE!	NOTE!	 NOTE!
 *
 *	setCalibrationMatrix() and getDisplayPoint() will do fine
 *	for you as they are, provided that your digitizer
 *	resolution does not exceed 10 bits (1024 values).  Higher
 *	resolutions may cause the integer operations to overflow
 *	and return incorrect values.  If you wish to use these
 *	functions with digitizer resolutions of 12 bits (4096
 *	values) you will either have to a) use 64-bit signed
 *	integer variables and math, or b) judiciously modify the
 *	operations to scale results by a factor of 2 or even 4.
 *
 */
RetCode_t TouchPanelComputeCalibration( point_t *displayPtr, point_t *screenPtr, tpMatrix_t *matrixPtr )
{
	RetCode_t retValue = noerror;

	tpMatrix.Divider =
		 ( ( screenPtr[0].x - screenPtr[2].x ) * ( screenPtr[1].y - screenPtr[2].y ) ) - ( ( screenPtr[1].x - screenPtr[2].x ) * ( screenPtr[0].y - screenPtr[2].y ) );

	if ( tpMatrix.Divider == 0 )
	{
		retValue = bad_parameter;
	}
	else
	{
		tpMatrix.An =
			 ( ( displayPtr[0].x - displayPtr[2].x ) * ( screenPtr[1].y - screenPtr[2].y ) ) - ( ( displayPtr[1].x - displayPtr[2].x ) * ( screenPtr[0].y - screenPtr[2].y ) );

		tpMatrix.Bn =
			 ( ( screenPtr[0].x - screenPtr[2].x ) * ( displayPtr[1].x - displayPtr[2].x ) ) - ( ( displayPtr[0].x - displayPtr[2].x ) * ( screenPtr[1].x - screenPtr[2].x ) );

		tpMatrix.Cn =
			 ( screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x ) *screenPtr[0].y + ( screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x ) *screenPtr[1].y + ( screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x ) *screenPtr[2].y;

		tpMatrix.Dn =
			 ( ( displayPtr[0].y - displayPtr[2].y ) * ( screenPtr[1].y - screenPtr[2].y ) ) - ( ( displayPtr[1].y - displayPtr[2].y ) * ( screenPtr[0].y - screenPtr[2].y ) );

		tpMatrix.En =
			 ( ( screenPtr[0].x - screenPtr[2].x ) * ( displayPtr[1].y - displayPtr[2].y ) ) - ( ( displayPtr[0].y - displayPtr[2].y ) * ( screenPtr[1].x - screenPtr[2].x ) );

		tpMatrix.Fn =
			 ( screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y ) *screenPtr[0].y + ( screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y ) *screenPtr[1].y + ( screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y ) *screenPtr[2].y;

		touchState = no_touch;

		if ( matrixPtr )
		{
			memcpy( matrixPtr, &tpMatrix, sizeof( tpMatrix_t ) );
		}
	}

	return ( retValue );
}



TouchCode_t TouchPanelA2DFiltered( int * x, int * y )
{
	static int xbuf[TPBUFSIZE], ybuf[TPBUFSIZE];
	static int lastX, lastY;
	int i, j;

	TouchCode_t ret = touchState;

	// printf( "\r\nTouchPanelA2DFiltered" );

	tsc2013_read_touch_status( );

	// Test for TP Interrupt pending in register INTC2
	// if ( ( DataRead( INTC2 ) & RA8875_INT_TP ) )
	if( g_touch_detected == 1 && ( ( tsc2013_touch_status & 0xf000 ) == 0xf000 ) )
	{
		// touchTimer.reset();
		// touchTimer
		g_touch_detected = 0;

		touchTimer_reset( );

		/*

		// Get the next data samples
		ybuf[touchSample] = DataRead( TPYH ) << 2 | ( ( DataRead( TPXYL ) & 0xC ) >> 2 ); 	// D[9:2] from reg TPYH, D[1:0] from reg TPXYL[3:2]
		xbuf[touchSample] = DataRead( TPXH ) << 2 | ( ( DataRead( TPXYL ) & 0x3 ) ); 		// D[9:2] from reg TPXH, D[1:0] from reg TPXYL[1:0]

		*/
		tsc2013_read_touch_data( );

		ybuf[touchSample] = data_set.values.Y1;
		xbuf[touchSample] = data_set.values.X1;

		#if 0
		printf( "\r\ndata_set.values.X1 %d ", data_set.values.X1 );

		printf( "\r\ndata_set.values.Y1 %d ", data_set.values.Y1 );
		#endif

		// Check for a complete set
		if ( ++touchSample == TPBUFSIZE )
		{
			// Buffers are full, so process them using Finn's method described in Analog Dialogue No. 44, Feb 2010
			// This requires sorting the samples in order of size, then discarding the top 25% and
			// bottom 25% as noise spikes. Finally, the middle 50% of the values are averaged to
			// reduce Gaussian noise.
#if 1
			InsertionSort( ybuf,TPBUFSIZE );
			InsertionSort( xbuf,TPBUFSIZE );
#else

			// Sort the Y buffer using an Insertion Sort
			for ( i = 1; i <= TPBUFSIZE; i++ )
			{
				temp = ybuf[i];
				j	= i;

				while ( j && ( ybuf[j - 1] > temp ) )
				{
					ybuf[j] = ybuf[j - 1];
					j	= j - 1;
				}

				ybuf[j] = temp;
			} // End of Y sort

			// Sort the X buffer the same way

			for ( i = 1; i <= TPBUFSIZE; i++ )
			{
				temp = xbuf[i];
				j	= i;

				while ( j && ( xbuf[j - 1] > temp ) )
				{
					xbuf[j] = xbuf[j - 1];
					j	= j - 1;
				}

				xbuf[j] = temp;
			} // End of X sort

#endif

			// Average the middle half of the  Y values and report them
			j = 0;

			for ( i = ( TPBUFSIZE / 4 ) - 1; i < TPBUFSIZE - TPBUFSIZE / 4; i++ )
			{
				j	+= ybuf[i];
			}

			*y	= lastY = j * ( float ) 2 / TPBUFSIZE; // This is the average

			// Average the middle half of the  X values and report them
			j	= 0;

			for ( i = ( TPBUFSIZE / 4 ) - 1; i < TPBUFSIZE - TPBUFSIZE / 4; i++ )
			{
				j	+= xbuf[i];
			}

			*x	= lastX = j * ( float ) 2 / TPBUFSIZE; // This is the average

			// Tidy up and return
			if ( touchState == touch || touchState == held )
			{
				touchState = held;
			}
			else
			{
				touchState = touch;
			}

			ret = touchState;

			// Ready to start on the next set of data samples
			touchSample = 0;
		}
		else
		{
			// Buffer not yet full, so do not return any results yet
			if ( touchState == touch || touchState == held )
			{
				*x	= lastX;
				*y	= lastY;
				ret = touchState = held;
			}
		}

        // reg INTC2: Clear that TP interrupt flag
		WriteCommand( INTC2, RA8875_INT_TP );

		// printf( "\r\ntouchState %d", touchState );

	} // End of initial if -- data has been read and processed
	else
	{
		if( touchState == touch || touchState == held )
		{
			*x	= lastX;
			*y	= lastY;

			ret = touchState = held;

			// printf( "\r\nret = touchState = held;" );
		}
		else if ( touchState == release )
		{
			*x	= lastX;
			*y	= lastY;

			ret = release;

			touchState = no_touch;

			// printf( "\r\ntouchState = no_touch;" );
		}
	}

	return ret;
}


RetCode_t TouchPanelCalibrate( const char *msg, tpMatrix_t *matrix, int maxwait_s )
{
	point_t pTest[3];
	point_t pSample[3];

    int i, t;
	int x, y;

	// timeout guards for not-installed, stuck, user not present...
	timeout_start( );
	touchTimer_start( );

	#if 1
	while ( TouchPanelA2DFiltered( &x, &y ) && ( timeout_read( ) < maxwait_s ) )
	{
		wait_ms( 20 );
	}
	#endif

	cls( 0 );

	if ( msg )
	{
		puts( msg );
		ra_puts( msg );
	}

	SetTextCursor( 0, height( ) / 2 );

	pTest[0].x = 50;
	pTest[0].y = 50;
	pTest[1].x = width() - 50;
	pTest[1].y = height() / 2;
	pTest[2].x = width() / 2;
	pTest[2].y = height() - 50;

	printf( "\r\nwidth():%d", width( ) );
	printf( "\r\nheight():%d", height( ) );

	#if 0
	while( 1 )
	{
		// wait_ms( 20 );

		TouchPanelA2DFiltered( &x, &y );
	}
	#endif

	for( i = 0; i < 3; i++ )
	{
		foreground( Blue );

		printf( "\r\n( %3d, %3d ) => ", pTest[i].x, pTest[i].y );

		foreground( White );
		line( pTest[i].x - 10, pTest[i].y, pTest[i].x + 10, pTest[i].y /* White */ );

		foreground( White );
		line( pTest[i].x, pTest[i].y - 10, pTest[i].x, pTest[i].y + 10 /* White */ );


		while ( !TouchPanelA2DFiltered( &x, &y ) )
		{
			wait_ms( 20 );

		}

		// printf( "\r\n!TouchPanelA2DFiltered" );

		wait_ms( 100 );

		pSample[ i ].x = x;
		pSample[ i ].y = y;

		foreground( Black );
		line( pTest[i].x - 10, pTest[i].y, pTest[i].x + 10, pTest[i].y /* Black */ );

		foreground( Black );
		line( pTest[i].x, pTest[i].y - 10, pTest[i].x, pTest[i].y + 10 /* Black */ );

		foreground( Blue );
		printf( "\r\n( %4d, %4d )", x, y );

		while( TouchPanelA2DFiltered( &x, &y ) )
		{
			wait_ms( 20 );
		}

		// printf( "\r\nTouchPanelA2DFiltered" );

	}

	if ( timeout_read( ) >= maxwait_s )
	{
		printf( "\r\ntouch_cal_timeout;" );
		return touch_cal_timeout;
	}
	else
	{
		printf( "\r\nTouchPanelComputeCalibration( pTest,pSample,matrix );" );
		return TouchPanelComputeCalibration( pTest, pSample, matrix );
	}


}


