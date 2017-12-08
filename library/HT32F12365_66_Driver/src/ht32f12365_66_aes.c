/*********************************************************************************************************//**
 * @file    ht32f12365_66_aes.c
 * @version $Rev:: 167          $
 * @date    $Date:: 2016-11-09 #$
 * @brief   This file provides all the AES firmware functions.
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
#include "ht32f12365_66_aes.h"

/** @addtogroup HT32F12365_66_Peripheral_Driver HT32F12365/12366 Peripheral Driver
  * @{
  */

/** @defgroup AES AES
  * @brief AES driver modules
  * @{
  */

/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup AES_Exported_Functions AES exported functions
  * @{
  */

/*********************************************************************************************************//**
  * @brief  Deinitializes the AES peripheral registers to their default reset values.
  * @retval None
  ***********************************************************************************************************/
void AES_DeInit(void)
{
  RSTCU_PeripReset_TypeDef RSTCUReset = {{0}};
  RSTCUReset.Bit.AES = 1;
  RSTCU_PeripReset(RSTCUReset, ENABLE);
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the FIFO Flush.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @retval None
 ************************************************************************************************************/
void AES_FIFOFlush(HT_AES_TypeDef* AESx)
{
  AES_Cmd(AESx,DISABLE);
  AESx->CR |= AES_FLUSH_ENABLE;
  AES_Cmd(AESx,ENABLE);
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified AES.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @param NewState: new state of the AESx peripheral.
 *        This parameter can be: ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void AES_Cmd(HT_AES_TypeDef* AESx,ControlStatus NewState)
{
  Assert_Param(IS_AES(AESx));
  Assert_Param(IS_CONTROL_STATUS(NewState));
  if(NewState != DISABLE)
  {
    AESx->CR |= AES_ENABLE;
  }
  else
  {
    AESx->CR &= ~AES_ENABLE;
  }
}

void AES_StartKey(HT_AES_TypeDef* AESx)
{
  AESx->CR |= 0x10;
}
/*********************************************************************************************************//**
  * @brief  Initialize the AESx peripheral according to the specified parameters in the AES_InitStruct.
  * @param  AESx: where AES is the selected AES peripheral.
  * @param  AES_InitStruct: pointer to a AES_InitTypeDef structure that contains the configuration
  *   information for the specified AES peripheral.
  * @retval None
  ***********************************************************************************************************/
void AES_Init(HT_AES_TypeDef* AESx,AES_InitTypeDef* AES_InitStruct)
{
  Assert_Param(IS_AES(AESx));
  Assert_Param(IS_AES_KEY_SIZE(AES_InitStruct->AES_KeySize));
  Assert_Param(IS_AES_DIR(AES_InitStruct->AES_Dir));
  Assert_Param(IS_AES_MODE(AES_InitStruct->AES_Mode));
  Assert_Param(IS_AES_SWAP(AES_InitStruct->AES_Swap));
  AESx->CR = (AESx->CR & 0xFFFFFE81) | AES_InitStruct->AES_KeySize |
             AES_InitStruct->AES_Dir | AES_InitStruct->AES_Mode |
             AES_InitStruct->AES_Swap;
}

/*********************************************************************************************************//**
  * @brief  Check whether the specified AES status has been set.
  * @param  AESx: where AESx is the selected AES peripheral.
  * @param  AES_Status: specify the flag to be check.
  *   This parameter can be one of the following values:
  *     @arg AES_SR_IFEMPTY                : AES Input FIFO is Empty
  *     @arg AES_SR_IFNFULL                : AES Input FIFO is not Full
  *     @arg AES_SR_OFNEMPTY               : AES Output FIFO is not Empty
  *     @arg AES_SR_OFFULL                 : AES Output FIFO is Full
  *     @arg AES_SR_BUSY                   : AES is busy when AES is in encrypt/decrypt action and key expansion
  * @retval SET or RESET
  ***********************************************************************************************************/
FlagStatus AES_GetStatus(HT_AES_TypeDef* AESx,u32 AES_Status)
{
  Assert_Param(IS_AES(AESx));
  Assert_Param(IS_AES_STATUS(AES_Status));
  if((AESx->SR & AES_Status) != (u32)RESET)
  {
    return (SET);
  }
  else
  {
    return (RESET);
  }
}

/*********************************************************************************************************//**
  * @brief  Enable or Disable the AESx PDMA interface.
  * @param  AESx: where AESx is the selected AESx peripheral.
  * @param  AES_PDMAFIFO: specify the AES FIFO DMA to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg AES_PDMA_IFDMAEN: input FIFO PDMA 
  *     @arg AES_PDMA_OFDMAEN: Output FIFO PDMA
  * @param  NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  ***********************************************************************************************************/
void AES_PDMACmd(HT_AES_TypeDef* AESx, u32 AES_PDMAFIFO, ControlStatus NewState)
{
  Assert_Param(IS_AES(AESx));
  Assert_Param(IS_CONTROL_STATUS(NewState));
  if(NewState != DISABLE)
  {
    AESx->PDMAR |= AES_PDMAFIFO;
  }
  else
  {
    AESx->PDMAR &= ~AES_PDMAFIFO;
  }
}

/*********************************************************************************************************//**
 * @brief Check whether the specified AES interrupt has occurred.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @param AES_INTSR: Specify the interrupt status to check.
     This parameter can be any combination of the following values:
       @arg AES_INTSR_IFINT
       @arg AES_INTSR_OFINT
 * @return SET or RESET
 ************************************************************************************************************/
FlagStatus AES_GetIntStatus(HT_AES_TypeDef* AESx, u32 AES_INTSR)
{
  FlagStatus Status;
  Assert_Param(IS_AES(AESx));
  Assert_Param(IS_AES_INTSR(AES_INTSR));
  Status = (FlagStatus)(AESx->ISR & AESx->IER);
  if ((Status & AES_INTSR) != RESET)
  {
    Status = SET;
  }
  else
  {
    Status = RESET;
  }

  return Status;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified AES interrupts.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @param AES_INT: Specify the AES interrupt sources that is to be enabled or disabled.
     This parameter can be any combination of the following values:
       @arg AES_IER_IFINTEN:
       @arg AES_IER_OFINTEN:
 * @param NewState: new state of the specified AES interrupts.
     This parameter can be: ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void AES_IntConfig(HT_AES_TypeDef* AESx, u32 AES_INT, ControlStatus NewState)
{
  Assert_Param(IS_AES(AESx));
  Assert_Param(IS_AES_INT(AES_INT));
  Assert_Param(IS_CONTROL_STATUS(NewState));
  if (NewState != DISABLE)
  {
    AESx->IER |= AES_INT;
  }
  else
  {
    AESx->IER &= ~AES_INT;
  }
}

/*********************************************************************************************************//**
 * @brief Set the specified AES Input data.
 * @param AESx: where AESx is the selected AES from the AES peripherals
 * @param AES_Data: Data input
 * @retval None
 ************************************************************************************************************/
void AES_SetInputData(HT_AES_TypeDef* AESx, uc32 AES_Data)
{
  Assert_Param(IS_AES(AESx));
  AESx->DINR = AES_Data;
}

/*********************************************************************************************************//**
 * @brief Get the specified AES output data.
 * @param AESx: where AESx is the selected AES from the AES peripherals
 * @retval Output Data 
 ************************************************************************************************************/
u32 AES_GetOutputData(HT_AES_TypeDef* AESx)
{
  Assert_Param(IS_AES(AESx));
  return AESx->DOUTR;
}

/*********************************************************************************************************//**
 * @brief Set the specified AES key table.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @param Key:
 * @param keySize:
 * @retval None
 ************************************************************************************************************/
void AES_SetKeyTable(HT_AES_TypeDef* AESx,uc32* Key,u32 keySize)
{
  int i;
  Assert_Param(IS_AES(AESx));
  if(keySize>8)
  {
    keySize = 8;
  }
  for(i=0;i<keySize;i++)
  {
    AESx->KEYR[i] = Key[i];
  }
}

/*********************************************************************************************************//**
 * @brief Get the specified AES key table.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @param Key:
 * @param keySize:
 * @retval None
 ************************************************************************************************************/
void AES_GetKeyTable(HT_AES_TypeDef* AESx,u32* Key,u32 keySize)
{
  int i;
  Assert_Param(IS_AES(AESx));
  if(keySize>8)
  {
    keySize = 8;
  }
  for(i=0;i<keySize;i++)
  {
    Key[i] = AESx->KEYR[i];
  }
}

/*********************************************************************************************************//**
 * @brief Set the specified AES Vector table.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @param Vector:
 * @param vectorSize:
 * @retval None
 ************************************************************************************************************/
void AES_SetVectorTable(HT_AES_TypeDef* AESx,uc32* Vector,u32 vectorSize)
{
  int i;
  Assert_Param(IS_AES(AESx));
  if(vectorSize>4)
  {
    vectorSize = 4;
  }
  for(i=0;i<vectorSize;i++)
  {
    AESx->IVR[i] = Vector[i];
  }
}

/*********************************************************************************************************//**
 * @brief Get the specified AES Vector table.
 * @param AESx: where AESx is the selected AES from the AES peripherals.
 * @param Vector:
 * @param vectorSize:
 * @retval None
 ************************************************************************************************************/
void AES_GetVectorTable(HT_AES_TypeDef* AESx,u32* Vector,u32 vectorSize)
{
  int i;
  Assert_Param(IS_AES(AESx));
  if(vectorSize>4)
  {
    vectorSize = 4;
  }
  for(i=0;i<vectorSize;i++)
  {
    Vector[i] = AESx->IVR[i];
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
