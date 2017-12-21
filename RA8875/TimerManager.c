#include "ht32.h"
#include "ht32_board.h"
#include "TimerManager.h"

/* Private variables ---------------------------------------------------------------------------------------*/
static vu32 DelayTime;
static vu32 timeout;

static vu32 touchTimer;
static u8 touchTimer_enable = 0;

/*********************************************************************************************************//**
  * @brief  Decrements the TimingDelay variable.
  * @retval None
  ***********************************************************************************************************/
void TimingDelay( void )
{
	if( DelayTime != 0 )
	{
		DelayTime--;
	}
}

/*********************************************************************************************************//**
  * @brief  Insert Delay time.
  * @param  nTime: Insert Delay time.
  * @retval None
  ***********************************************************************************************************/
void wait_ms( u32 nTime )
{
	/* Enable the SysTick Counter */
	// SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
	// SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

	DelayTime = nTime;

	while( DelayTime != 0 );

	/* Disable SysTick Counter */
	// SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
	/* Clear SysTick Counter */
	// SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}


void timeout_increase( void )
{
	if( timeout )
		timeout --;
}

u16 timeout_read( void )
{
	// return second
	return ( timeout / 1000 );
}


void timeout_start( void )
{
	timeout = 1;
}


void touchTimer_reset( void )
{
	touchTimer = 0;
}


void touchTimer_start( void )
{
	touchTimer_enable = 1;
}

void touchTimer_increase( void )
{
	//if( touchTimer_enable )
	touchTimer ++;
}

u16 touchTimer_read( void )
{
	return touchTimer;
}




