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

typedef struct
{

  vu16 EBI_SRAM_REG;
  vu16 EBI_SRAM_RAM;

} EBI_SRAM_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Exported_Constants EBI SRAM exported constants
  * @{
  */
#define SRAM_Bank_ADDR      0x64000000

/*
#define LCD_Bank_ADDR      0x60000000
#define LCD_Bank1_ADDR     0x64000000
#define LCD_Bank2_ADDR     0x68000000
#define LCD_Bank3_ADDR     0x6C000000
*/

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Exported_Macros EBI SRAM exported macros
  * @{
  */
#define EBI_SRAM	( ( EBI_SRAM_TypeDef *) SRAM_Bank_ADDR )
/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/

/** @defgroup EBI_SRAM_Exported_Functions EBI SRAM exported functions
  * @{
  */

extern void EBI_SRAM_Init(void);
extern void EBI_SRAM_WriteBuffer( u16* WriteBuffer, u32 WriteAddr, u32 NumOfWriteHalfWord );
extern void EBI_SRAM_ReadBuffer( u16* ReadBuffer, u32 ReadAddr, u32 NumOfReadHalfWord );
extern void EBI_SRAM_Test( void );
extern void EBI_SRAM_Test_1( void );
extern void EBI_SRAM_IO_Init( void );



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
