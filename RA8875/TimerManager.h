#ifndef _TIMER_MANAGER_H_

#define _TIMER_MANAGER_H_

#include "ht32f12365_66.h"

extern void TimingDelay( void );
extern void wait_ms( u32 nTime );
extern void timeout_increase( void );
extern u16 timeout_read( void );
extern void timeout_start( void );

extern void touchTimer_reset( void );
extern void touchTimer_start( void );
extern void touchTimer_increase( void );
extern u16 touchTimer_read( void );


#endif