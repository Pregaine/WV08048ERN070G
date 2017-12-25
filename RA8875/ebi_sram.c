/*********************************************************************************************************//**
 * @file    ebi_sram.c
 * @version V1.0
 * @date    03/12/2012
 * @brief   This file provides a set of functions needed to manage the
 *          communication between EBI peripheral and IS61WVP2416 SRAM.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32_board_config.h"
#include "ebi_sram.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup HT32F1655_Development_Board
  * @{
  */

/** @addtogroup EBI_SRAM EBI_SRAM
  * @{
  */

/* Private typedef -----------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Private_TypesDefinitions EBI SRAM private types definitions
  * @{
  */
#define SRAM_Bank_ADDR      0x64000000
/**
  * @}
  */

/* Private define ------------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Private_Define EBI SRAM private definitions
  * @{
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Private_Macro EBI SRAM private macros
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Private_Values EBI SRAM private variables
  * @{
  */

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Private_Function_Prototypes EBI SRAM private function prototypes
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Private_Functions EBI SRAM private functions
  * @{
  */

/*********************************************************************************************************//**
  * @brief  Configures the EBI and GPIOs to interface with the SRAM memory.
  * @retval None
  ***********************************************************************************************************/
void EBI_SRAM_Init(void)
{
  EBI_InitTypeDef  EBI_InitStructure;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.EBI        = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* A0 ~ A18 */
  AFIO_GPxConfig(HTCFG_EBI_A0_ID, HTCFG_EBI_A0_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A1_ID, HTCFG_EBI_A1_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A2_ID, HTCFG_EBI_A2_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A3_ID, HTCFG_EBI_A3_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A4_ID, HTCFG_EBI_A4_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A5_ID, HTCFG_EBI_A5_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A6_ID, HTCFG_EBI_A6_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A7_ID, HTCFG_EBI_A7_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A8_ID, HTCFG_EBI_A8_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A9_ID, HTCFG_EBI_A9_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A10_ID, HTCFG_EBI_A10_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A11_ID, HTCFG_EBI_A11_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A12_ID, HTCFG_EBI_A12_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A13_ID, HTCFG_EBI_A13_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A14_ID, HTCFG_EBI_A14_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A15_ID, HTCFG_EBI_A15_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A16_ID, HTCFG_EBI_A16_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A17_ID, HTCFG_EBI_A17_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_A18_ID, HTCFG_EBI_A18_PIN, AFIO_FUN_EBI);

  /* AD0 ~ AD15 */
  AFIO_GPxConfig(HTCFG_EBI_AD0_ID, HTCFG_EBI_AD0_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD1_ID, HTCFG_EBI_AD1_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD2_ID, HTCFG_EBI_AD2_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD3_ID, HTCFG_EBI_AD3_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD4_ID, HTCFG_EBI_AD4_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD5_ID, HTCFG_EBI_AD5_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD6_ID, HTCFG_EBI_AD6_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD7_ID, HTCFG_EBI_AD7_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD8_ID, HTCFG_EBI_AD8_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD9_ID, HTCFG_EBI_AD9_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD10_ID, HTCFG_EBI_AD10_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD11_ID, HTCFG_EBI_AD11_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD12_ID, HTCFG_EBI_AD12_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD13_ID, HTCFG_EBI_AD13_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD14_ID, HTCFG_EBI_AD14_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_AD15_ID, HTCFG_EBI_AD15_PIN, AFIO_FUN_EBI);

  /* OE, WE, CS1, BL0, BL1 */
  AFIO_GPxConfig(HTCFG_EBI_OE_ID, HTCFG_EBI_OE_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_WE_ID, HTCFG_EBI_WE_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_CS1_ID, HTCFG_EBI_CS1_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_BL0_ID, HTCFG_EBI_BL0_PIN, AFIO_FUN_EBI);
  AFIO_GPxConfig(HTCFG_EBI_BL1_ID, HTCFG_EBI_BL1_PIN, AFIO_FUN_EBI);

  /*  EBI Configuration */
  EBI_InitStructure.EBI_Bank = EBI_BANK_1;
  EBI_InitStructure.EBI_Mode = EBI_MODE_D16;
  EBI_InitStructure.EBI_ByteLane = EBI_BYTELANE_ENABLE;
  EBI_InitStructure.EBI_IdleCycleTime = EBI_IDLECYCLE_DISABLE;
  EBI_InitStructure.EBI_AsynchronousReady = EBI_ASYNCHRONOUSREADY_DISABLE;
  EBI_InitStructure.EBI_ARDYTimeOut = EBI_ARDYTIMEOUT_DISABLE;
  EBI_InitStructure.EBI_ChipSelectPolarity = EBI_CHIPSELECTPOLARITY_LOW;
  EBI_InitStructure.EBI_AddressLatchPolarity = EBI_ADDRESSLATCHPOLARITY_LOW;
  EBI_InitStructure.EBI_WriteEnablePolarity = EBI_WRITEENABLEPOLARITY_LOW;
  EBI_InitStructure.EBI_ReadEnablePolarity = EBI_READENABLEPOLARITY_LOW;
  EBI_InitStructure.EBI_ByteLanePolarity = EBI_BYTELANEPOLARITY_LOW;
  EBI_InitStructure.EBI_ReadySignalPolarity = EBI_READYSIGNALPOLARITY_LOW;
  EBI_InitStructure.EBI_IdleCycleTime = 0x0;
  EBI_InitStructure.EBI_AddressSetupTime = 0x0;
  EBI_InitStructure.EBI_AddressHoldTime = 0x0;
  EBI_InitStructure.EBI_WriteSetupTime = 0x1;
  EBI_InitStructure.EBI_WriteStrobeTime = 0x2;
  EBI_InitStructure.EBI_WriteHoldTime = 0x1;
  EBI_InitStructure.EBI_ReadSetupTime = 0x1;
  EBI_InitStructure.EBI_ReadStrobeTime = 0x2;
  EBI_InitStructure.EBI_ReadHoldTime = 0x1;
  EBI_InitStructure.EBI_PageMode = EBI_PAGEMODE_DISABLE;
  EBI_InitStructure.EBI_PageLength = EBI_PAGELENGTH_4;
  EBI_InitStructure.EBI_PageHitMode = EBI_PAGEHITMODE_ADDINC;
  EBI_InitStructure.EBI_PageAccessTime = 0x0;
  EBI_InitStructure.EBI_PageOpenTime = 0x0;
  EBI_Init(&EBI_InitStructure);

  EBI_Cmd(EBI_BANK_1, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  Writes a half-word buffer to the SRAM memory.
  * @param  WriteBuffer: pointer to buffer.
  * @param  WriteAddr: SRAM memory internal address from which the data will be written.
  * @param  NumOfWriteHalfWord: number of Half words to write.
  * @retval None
  ***********************************************************************************************************/
void EBI_SRAM_WriteBuffer(u16* WriteBuffer, u32 WriteAddr, u32 NumOfWriteHalfWord)
{
  while(NumOfWriteHalfWord--)
  {
    *(vu16 *)(SRAM_Bank_ADDR + WriteAddr) = *WriteBuffer++;
    WriteAddr += 2;
  }
}

/*********************************************************************************************************//**
  * @brief  Reads a block of data from the EBI SRAM memory.
  * @param  ReadBuffer: pointer to the buffer that receives the data read from the SRAM memory.
  * @param  ReadAddr: NOR memory internal address to read from.
  * @param  NumOfReadHalfWord : number of Half word to read.
  * @retval None
  ***********************************************************************************************************/
void EBI_SRAM_ReadBuffer(u16* ReadBuffer, u32 ReadAddr, u32 NumOfReadHalfWord)
{
  while(NumOfReadHalfWord--)
  {
    *ReadBuffer++ = *(vu16 *)(SRAM_Bank_ADDR + ReadAddr);
    ReadAddr += 2;
	}
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
