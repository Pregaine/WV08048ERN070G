/*********************************************************************************************************//**
 * @file    ebi_sram.h
 * @version V1.0
 * @date    03/12/2012
 * @brief   The header file of ebi_sram.c module.
 *************************************************************************************************************
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 *
 ************************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __EBI_NOR_H
#define __EBI_NOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup HT32F1655_Development_Board
  * @{
  */

/** @addtogroup EBI_SRAM EBI_SRAM
  * @{
  */

/* Exported types ------------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Exported_Types EBI SRAM exported types
  * @{
  */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Exported_Constants EBI SRAM exported constants
  * @{
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Exported_Macros EBI SRAM exported macros
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Exported_Functions EBI SRAM exported functions
  * @{
  */
void EBI_SRAM_Init(void);
void EBI_SRAM_WriteBuffer(u16* WriteBuffer, u32 WriteAddr, u32 NumOfWriteHalfWord);
void EBI_SRAM_ReadBuffer(u16* ReadBuffer, u32 ReadAddr, u32 NumOfReadHalfWord);

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

#endif /* __EBI_SRAM_H ---------------------------------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
