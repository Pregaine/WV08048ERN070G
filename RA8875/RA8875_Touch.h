#ifndef _RA8875_TOUCH_H_
#define _RA8875_TOUCH_H_

#include "RA8875.h"

extern RetCode_t TouchPanelCalibrate( const char * msg, tpMatrix_t * matrix, int maxwait_s );

extern TouchCode_t TouchPanelReadable( point_t * TouchPoint );

extern TouchCode_t TouchPanelA2DFiltered( int * x,int * y );

extern RetCode_t TouchPanelComputeCalibration( point_t *displayPtr, point_t *screenPtr, tpMatrix_t *matrixPtr );

#endif
