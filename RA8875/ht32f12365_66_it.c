/*********************************************************************************************************//**
 * @file    EBI/LCD/ht32f12365_66_it.c
 * @version $Rev:: 155          $
 * @date    $Date:: 2016-11-03 #$
 * @brief   This file provides all interrupt service routine.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32_board.h"
#include "sdio_sd.h"
#include "TimerManager.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup SDIO_Examples SDIO
  * @{
  */

/** @addtogroup SDIO_SDCard
  * @{
  */


/* Global variables ----------------------------------------------------------------------------------------*/
extern __IO SD_Error TransferStatus;
extern u8 tick;
extern bool click;
/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Memory Manage exception.
 * @retval  None
 ************************************************************************************************************/
void MemManage_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Bus Fault exception.
 * @retval  None
 ************************************************************************************************************/
void BusFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles Usage Fault exception.
 * @retval  None
 ************************************************************************************************************/
void UsageFault_Handler(void)
{
  while (1);
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Debug Monitor exception.
 * @retval  None
 ************************************************************************************************************/
void DebugMon_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA_CH6 interrupt.
 * @retval  None
 ************************************************************************************************************/
void PDMA_CH6_IRQHandler(void)
{
  if (HT_PDMA->ISR1 & (PDMA_FLAG_TE << ((PDMA_CH6-6)*5)))
  {
    printf(" TE6");
    while (1);
  }

  HT_PDMA->ISCR1 = PDMA_FLAG_TC << ((PDMA_CH6-6)*5);
}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA_CH7 interrupt.
 * @retval  None
 ************************************************************************************************************/
void PDMA_CH7_IRQHandler(void)
{
  if (HT_PDMA->ISR1 & (PDMA_FLAG_TE << ((PDMA_CH7-6)*5)))
  {
    printf(" TE7");
    while (1);
  }

  HT_PDMA->ISCR1 = PDMA_FLAG_TC << ((PDMA_CH7-6)*5);
}

/*********************************************************************************************************//**
 * @brief   This function handles SDIO interrupt.
 * @retval  None
 ************************************************************************************************************/
void SDIO_IRQHandler(void)
{
  TransferStatus = SD_InterruptService();
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
  	extern u32 uSpeedCount;

  	uSpeedCount++;

  	TimingDelay();

  	timeout_increase();

  	touchTimer_increase();

  	_TouchTicker( );
}
u8 mBuff[256];
void EXTI13_IRQHandler(void)
{
	// u8 i=0;
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_13);
	printf("13\r\n");

}

void EXTI14_IRQHandler(void)
{
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
	printf("14\r\n");
}

void EXTI2_IRQHandler(void)
{
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
	printf("2\r\n");
}

void EXTI1_IRQHandler(void)
{
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
	printf("1\r\n");

	if(click == FALSE){
		click = TRUE;
	}
	tick++;
	if(tick==4)
		tick=0;
}

void EXTI9_IRQHandler(void)
{
	u8 mBuff2[32];
	int i=0;
	EXTI_ClearEdgeFlag(EXTI_CHANNEL_9);
	//printf("9\r\n");
	///*
	I2C_EEPROM_BufferRead(mBuff2,0, 32);
	printf("========= Start =========\r\n");
	for(i=0;i<32;i++){
		if( i%8==7)
			printf("[0x%x]\r\n",mBuff2[i]);
		else
			printf("[0x%x],",mBuff2[i]);
	}
	printf("========= End =========\r\n");
	//*/
}
