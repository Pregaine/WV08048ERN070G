#include "ht32.h"
#include "ht32_board.h"
#include "TimerManager.h"

/* Private variables ---------------------------------------------------------------------------------------*/
static vu32 DelayTime;

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
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
	SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);

	DelayTime = nTime;

	while( DelayTime != 0 );

	/* Disable SysTick Counter */
	SYSTICK_CounterCmd(SYSTICK_COUNTER_DISABLE);
	/* Clear SysTick Counter */
	SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
}




