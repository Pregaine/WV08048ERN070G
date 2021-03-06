/*********************************************************************************************************//**
 * @file    ht32f12366_dvbtemp.h
 * @version $Rev:: 277          $
 * @date    $Date:: 2016-12-05 #$
 * @brief   The header file of ht32f12366_dvbtemp.
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

/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F12366_DVBTEMP_H
#define __HT32F12366_DVBTEMP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "common/i2c_eeprom.h"
#include "common/spi_flash.h"
#include "common/ebi_lcd.h"
#include "common/ebi_sram.h"
#include "TimerManager.h"
#include "RA8875_Touch.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup HT32_Board
  * @{
  */

/** @addtogroup HT32F12366_DVBTEMP
  * @{
  */


/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup HT32F12366_DVBTEMP_Exported_Constants HT32F12366 DVB exported constants
  * @{
  */
extern HT_GPIO_TypeDef* const GPIO_PORT[GPIO_PORT_NUM];

/** @addtogroup HT32F12366_DVBTEMP_LED
  * @{
  */
#define LEDn                        (4)

#define LED1_GPIO_ID                ( GPIO_PC )
#define LED1_GPIO_PIN               ( GPIO_PIN_3 )
#define LED1_AFIO_MODE              ( AFIO_MODE_DEFAULT )

#define LED2_GPIO_ID                ( GPIO_PD )
#define LED2_GPIO_PIN               ( GPIO_PIN_8 )
#define LED2_AFIO_MODE              ( AFIO_MODE_DEFAULT )

#define LED3_GPIO_ID                ( GPIO_PA )
#define LED3_GPIO_PIN               ( GPIO_PIN_10 )
#define LED3_AFIO_MODE              ( AFIO_MODE_DEFAULT)

#define LED4_GPIO_ID                ( GPIO_PD )
#define LED4_GPIO_PIN               ( GPIO_PIN_3 )
#define LED4_AFIO_MODE              ( AFIO_MODE_DEFAULT )

typedef enum
{
  HT_LED1 = 0,
  HT_LED2 = 1,
  HT_LED3 = 2,
  HT_LED4 = 3,

} LED_TypeDef;
/**
  * @}
  */

/** @addtogroup HT32F12366_DVBTEMP_BUTTON
  * @{
  */
#define BUTTONn                     (3)

#define WAKEUP_BUTTON_GPIO_ID       (GPIO_PC)
#define WAKEUP_BUTTON_GPIO_PIN      (GPIO_PIN_15)
#define WAKEUP_BUTTON_AFIO_MODE     (AFIO_FUN_SYSTEM)
#define WAKEUP_BUTTON_EXTI_CHANNEL  (15)

#define KEY1_BUTTON_GPIO_ID         (GPIO_PD)
#define KEY1_BUTTON_GPIO_PIN        (GPIO_PIN_13)
#define KEY1_BUTTON_AFIO_MODE       (AFIO_MODE_DEFAULT)
#define KEY1_BUTTON_EXTI_CHANNEL    (13)

#define KEY2_BUTTON_GPIO_ID         (GPIO_PD)
#define KEY2_BUTTON_GPIO_PIN        (GPIO_PIN_14)
#define KEY2_BUTTON_AFIO_MODE       (AFIO_MODE_DEFAULT)
#define KEY2_BUTTON_EXTI_CHANNEL    (14)

typedef enum
{
  BUTTON_WAKEUP = 0,
  BUTTON_KEY1   = 1,
  BUTTON_KEY2   = 2
} BUTTON_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1,
  BUTTON_MODE_WEAK_UP=2
} BUTTON_MODE_TypeDef;
/**
  * @}
  */

/** @addtogroup HT32F12366_DVBTEMP_BUZZER
  * @{
  */
#define BUZZER_GPIO_ID              (GPIO_PE)
#define BUZZER_AFIO_PIN             (AFIO_PIN_3)
#define BUZZER_AFIO_MODE            (AFIO_FUN_MCTM_GPTM)
#define BUZZER_GPIO_CLK(CK)         (CK.Bit.PE)

#define BUZZER_TM_CLK(CK)           (CK.Bit.MCTM1)
#define BUZZER_TM                   (HT_MCTM1)
#define BUZZER_TM_CHANNEL           (MCTM_CH_1)
#define BUZZER_IRQn                 (MCTM1UP_IRQn)
/**
  * @}
  */

/** @addtogroup HT32F12366_DVBTEMP_COM
  * @{
  */
#define COMn                        (2)

#define COM1_CLK(CK)                (CK.Bit.USART0)
#define COM1_PORT                   (HT_USART0)
#define COM1_IRQn                   (USART0_IRQn)

/*

#define COM1_TX_GPIO_ID             (GPIO_PA)
#define COM1_TX_AFIO_PIN            (AFIO_PIN_2)
#define COM1_TX_AFIO_MODE           (AFIO_FUN_USART_UART)

#define COM1_RX_GPIO_ID             (GPIO_PA)
#define COM1_RX_AFIO_PIN            (AFIO_PIN_3)
#define COM1_RX_AFIO_MODE           (AFIO_FUN_USART_UART)
*/

#define COM1_TX_GPIO_ID             (GPIO_PA)
#define COM1_TX_AFIO_PIN            (AFIO_PIN_8)
#define COM1_TX_AFIO_MODE           (AFIO_FUN_USART_UART)

#define COM1_RX_GPIO_ID             (GPIO_PA)
#define COM1_RX_AFIO_PIN            (AFIO_PIN_10)
#define COM1_RX_AFIO_MODE           (AFIO_FUN_USART_UART)


#define COM2_CLK(CK)                (CK.Bit.UART0)
#define COM2_PORT                   (HT_UART0)
#define COM2_IRQn                   (UART0_IRQn)

#define COM2_TX_GPIO_ID             (GPIO_PA)
#define COM2_TX_AFIO_PIN            (AFIO_PIN_8)
#define COM2_TX_AFIO_MODE           (AFIO_FUN_USART_UART)

#define COM2_RX_GPIO_ID             (GPIO_PA)
#define COM2_RX_AFIO_PIN            (AFIO_PIN_10)
#define COM2_RX_AFIO_MODE           (AFIO_FUN_USART_UART)

typedef enum
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;
/**
  * @}
  */

/** @addtogroup HT32F12366_DVBTEMP_SPI_FLASH
  * @{
  */

#if 0
#define FLASH_SPI_CLK(CK)           (CK.Bit.SPI0)
#define FLASH_SPI                   (HT_SPI0)

#define FLASH_SPI_SCK_GPIO_ID       (GPIO_PD)
#define FLASH_SPI_SCK_AFIO_PIN      (AFIO_PIN_10)
#define FLASH_SPI_SCK_AFIO_MODE     (AFIO_FUN_SPI)

#define FLASH_SPI_MOSI_GPIO_ID      (GPIO_PD)
#define FLASH_SPI_MOSI_AFIO_PIN     (AFIO_PIN_11)
#define FLASH_SPI_MOSI_AFIO_MODE    (AFIO_FUN_SPI)

#define FLASH_SPI_MISO_GPIO_ID      (GPIO_PD)
#define FLASH_SPI_MISO_AFIO_PIN     (AFIO_PIN_12)
#define FLASH_SPI_MISO_AFIO_MODE    (AFIO_FUN_SPI)

#define FLASH_SPI_SEL_GPIO_ID       (GPIO_PE)
#define FLASH_SPI_SEL_GPIO_PIN      (GPIO_PIN_7)
#define FLASH_SPI_SEL_AFIO_MODE     (AFIO_MODE_DEFAULT)
#define FLASH_SPI_SEL_CLK(CK)       (CK.Bit.PE)
#endif

#define FLASH_SPI_CLK(CK)           ( CK.Bit.SPI1 )
#define FLASH_SPI                   ( HT_SPI1 )

#define FLASH_SPI_SCK_GPIO_ID       ( GPIO_PE )
#define FLASH_SPI_SCK_AFIO_PIN      ( AFIO_PIN_9 )
#define FLASH_SPI_SCK_AFIO_MODE     ( AFIO_FUN_SPI )

#define FLASH_SPI_MOSI_GPIO_ID      ( GPIO_PE )
#define FLASH_SPI_MOSI_AFIO_PIN     ( AFIO_PIN_10 )
#define FLASH_SPI_MOSI_AFIO_MODE    ( AFIO_FUN_SPI )

#define FLASH_SPI_MISO_GPIO_ID      ( GPIO_PE )
#define FLASH_SPI_MISO_AFIO_PIN     ( AFIO_PIN_11 )
#define FLASH_SPI_MISO_AFIO_MODE    ( AFIO_FUN_SPI )

#define FLASH_SPI_SEL_GPIO_ID       ( GPIO_PA )
#define FLASH_SPI_SEL_GPIO_PIN      ( GPIO_PIN_7 )
#define FLASH_SPI_SEL_AFIO_MODE     ( AFIO_MODE_DEFAULT )
#define FLASH_SPI_SEL_CLK(CK)       ( CK.Bit.PE )

/**
  * @}
  */

/** @addtogroup HT32F12366_DVBTEMP_EBI_LCD
  * @{
  */
#define LCD_EBI                     (HT_EBI)
/*

//============= 有波形 =============//
#define LCD_EBI_WE_GPIO_ID          (GPIO_PB)
#define LCD_EBI_WE_AFIO_PIN         (AFIO_PIN_8)
#define LCD_EBI_WE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_RS_GPIO_ID          (GPIO_PA)
#define LCD_EBI_RS_AFIO_PIN         (AFIO_PIN_11)
#define LCD_EBI_RS_AFIO_MODE        (AFIO_FUN_EBI)

//==== Control Define Start =====//
#define HTCFG_DC      (HT_GPIOB)
#define GPIO_DC       (GPIO_PB)
#define DC            (AFIO_PIN_9)

#define HTCFG_WR      (HT_GPIOD)
#define GPIO_WR       (GPIO_PD)
#define WR            (AFIO_PIN_8)

#define HTCFG_RD      (HT_GPIOD)
#define GPIO_RD       (GPIO_PD)
#define RD            (AFIO_PIN_12)

#define HTCFG_CS      (HT_GPIOB)
#define GPIO_CS       (GPIO_PB)
#define CS            (AFIO_PIN_7)

#define HTCFG_DIP     (HT_GPIOD)
#define GPIO_DIP      (GPIO_PD)
#define DIP           (AFIO_PIN_12)

#define HTCFG_RST     (HT_GPIOD)
#define GPIO_RST      (GPIO_PD)
#define RST           (AFIO_PIN_14)
*/

#define LCD_EBI_RST_CLK(CK)         (CK.Bit.PA)

#define LCD_EBI_CS_GPIO_ID          (GPIO_PB)
#define LCD_EBI_CS_AFIO_PIN         (AFIO_PIN_7)
#define LCD_EBI_CS_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_OE_GPIO_ID          (GPIO_PB)		//always high
#define LCD_EBI_OE_AFIO_PIN         (AFIO_PIN_6)
#define LCD_EBI_OE_AFIO_MODE        (AFIO_FUN_EBI)

#define LCD_EBI_WE_GPIO_ID          (GPIO_PB)
#define LCD_EBI_WE_AFIO_PIN         (AFIO_PIN_8)
#define LCD_EBI_WE_AFIO_MODE        (AFIO_FUN_EBI)

#define HTCFG_DC      (HT_GPIOD)//(HT_GPIOA)
#define HTCFG_WR      (HT_GPIOB)
#define HTCFG_RD      (HT_GPIOB)//(HT_GPIOB)
#define HTCFG_CS      (HT_GPIOB)//(HT_GPIOD)
#define HTCFG_DIP     (HT_GPIOA)//(HT_GPIOD)
#define HTCFG_RST     (HT_GPIOC)//(HT_GPIOB)

#define GPIO_DC       (GPIO_PD)//(GPIO_PA)
#define GPIO_WR       (GPIO_PB)
#define GPIO_RD       (GPIO_PB)//(GPIO_PB)
#define GPIO_CS       (GPIO_PB)//(GPIO_PD)
#define GPIO_DIP      (GPIO_PA)//(GPIO_PD)
#define GPIO_RST      (GPIO_PC)//(GPIO_PB)

#define DC            (AFIO_PIN_6)		// (AFIO_PIN_11)
#define WR            (AFIO_PIN_8)
#define RD            (AFIO_PIN_6)		// (AFIO_PIN_6)
#define CS            (AFIO_PIN_7)		// (AFIO_PIN_14)
#define DIP           (AFIO_PIN_0)		// (AFIO_PIN_13)
#define RST           (AFIO_PIN_12)		// (AFIO_PIN_7)


//#define HTCFG_INT      (HT_GPIOB)
//#define GPIO_INT       (GPIO_PB)
//#define TP_INT           (AFIO_PIN_9)

//#define HTCFG_WAIT      (HT_GPIOB)
//#define GPIO_WAIT       (GPIO_PB)//(GPIO_PD)
//#define WAIT            (AFIO_PIN_11)


//==== Control Define End =====//
//==== Data Bus Define Start =====//
#define GPIO_D00        (GPIO_PA)
#define GPIO_D01        (GPIO_PA)
#define GPIO_D02        (GPIO_PB)
#define GPIO_D03        (GPIO_PB)
#define GPIO_D04        (GPIO_PB)
#define GPIO_D05        (GPIO_PB)
#define GPIO_D06        (GPIO_PB)
#define GPIO_D07        (GPIO_PB)
#define GPIO_D08        (GPIO_PC)
#define GPIO_D09        (GPIO_PC)
#define GPIO_D10        (GPIO_PC)
#define GPIO_D11        (GPIO_PC)
#define GPIO_D12        (GPIO_PC)
#define GPIO_D13        (GPIO_PC)
#define GPIO_D14        (GPIO_PC)
#define GPIO_D15        (GPIO_PC)

#define HTCFG_D00      (HT_GPIOA)
#define HTCFG_D01      (HT_GPIOA)
#define HTCFG_D02      (HT_GPIOB)
#define HTCFG_D03      (HT_GPIOB)
#define HTCFG_D04      (HT_GPIOB)
#define HTCFG_D05      (HT_GPIOB)
#define HTCFG_D06      (HT_GPIOB)
#define HTCFG_D07      (HT_GPIOB)
#define HTCFG_D08      (HT_GPIOC)
#define HTCFG_D09      (HT_GPIOC)
#define HTCFG_D10      (HT_GPIOC)
#define HTCFG_D11      (HT_GPIOC)
#define HTCFG_D12      (HT_GPIOC)
#define HTCFG_D13      (HT_GPIOC)
#define HTCFG_D14      (HT_GPIOC)
#define HTCFG_D15      (HT_GPIOC)

#define D00            (AFIO_PIN_14)
#define D01            (AFIO_PIN_15)
#define D02            (AFIO_PIN_0)
#define D03            (AFIO_PIN_1)
#define D04            (AFIO_PIN_2)
#define D05            (AFIO_PIN_3)
#define D06            (AFIO_PIN_4)
#define D07            (AFIO_PIN_5)
#define D08            (AFIO_PIN_7)
#define D09            (AFIO_PIN_8)
#define D10            (AFIO_PIN_4)
#define D11            (AFIO_PIN_5)
#define D12            (AFIO_PIN_6)
#define D13            (AFIO_PIN_0)
#define D14            (AFIO_PIN_1)
#define D15            (AFIO_PIN_2)
/*
#define LCD_EBI_BL_ENABLE           (1)
#define LCD_EBI_BL_GPIO_ID          (GPIO_PD)
#define LCD_EBI_BL_GPIO_PIN         (GPIO_PIN_13)
#define LCD_EBI_BL_AFIO_MODE        (AFIO_MODE_DEFAULT)
#define LCD_EBI_BL_CLK(CK)          (CK.Bit.PD)
*/
#define LCD_EBI_BL_ENABLE           (0)
#define LCD_EBI_BL_GPIO_ID          (GPIO_PE)//(GPIO_PD)
#define LCD_EBI_BL_GPIO_PIN         (GPIO_PIN_6)//(GPIO_PIN_13)
#define LCD_EBI_BL_AFIO_MODE        (AFIO_MODE_DEFAULT)
#define LCD_EBI_BL_CLK(CK)          (CK.Bit.PE)//(CK.Bit.PD)


/**
  * @}
  */

/** @addtogroup HT32F12366_DVBTEMP_I2C_EEPROM
  * @{
  */

#if 0
#define EEPROM_I2C_CLK(CK)          ( CK.Bit.I2C1 )
#define EEPROM_I2C                  ( HT_I2C1 )

#define EEPROM_I2C_SCL_GPIO_ID      (GPIO_PA)
#define EEPROM_I2C_SCL_AFIO_PIN     (AFIO_PIN_0)
#define EEPROM_I2C_SCL_AFIO_MODE    (AFIO_FUN_I2C)

#define EEPROM_I2C_SDA_GPIO_ID      (GPIO_PA)
#define EEPROM_I2C_SDA_AFIO_PIN     (AFIO_PIN_1)
#define EEPROM_I2C_SDA_AFIO_MODE    (AFIO_FUN_I2C)
#endif

#define EEPROM_I2C_CLK(CK)          ( CK.Bit.I2C0 )
#define EEPROM_I2C                  ( HT_I2C0 )

#define EEPROM_I2C_SCL_GPIO_ID      ( GPIO_PB )
#define EEPROM_I2C_SCL_AFIO_PIN     ( AFIO_PIN_12 )
#define EEPROM_I2C_SCL_AFIO_MODE    ( AFIO_FUN_I2C )

#define EEPROM_I2C_SDA_GPIO_ID      ( GPIO_PB )
#define EEPROM_I2C_SDA_AFIO_PIN     ( AFIO_PIN_13 )
#define EEPROM_I2C_SDA_AFIO_MODE    ( AFIO_FUN_I2C )


/**
  * @}
  */

/** @addtogroup HT32F12366_DVBTEMP_SDIO_SDCARD
  * @{
  */
#define SDCARD_SDIO                 (HT_SDIO)

#if 0
#define SDCARD_SD_CD_GPIO_ID        (GPIO_PD)//(GPIO_PA)
#define SDCARD_SD_CD_GPIO_PIN       (GPIO_PIN_6)//(GPIO_PIN_0)
#define SDCARD_SD_CD_AFIO_MODE      (AFIO_FUN_DEFAULT)
#define SDCARD_SD_CD_GPIO_CLK(CK)   (CK.Bit.PD)//(CK.Bit.PA)

#define SDCARD_SD_CLK_GPIO_ID       (GPIO_PA)
#define SDCARD_SD_CLK_AFIO_PIN      (AFIO_PIN_5)
#define SDCARD_SD_CLK_AFIO_MODE     (AFIO_FUN_SDIO)
#define SDCARD_SD_CLK_GPIO_CLK(CK)  (CK.Bit.PA)

#define SDCARD_SD_CMD_GPIO_ID       (GPIO_PA)
#define SDCARD_SD_CMD_AFIO_PIN      (AFIO_PIN_4)
#define SDCARD_SD_CMD_AFIO_MODE     (AFIO_FUN_SDIO)
#define SDCARD_SD_CMD_GPIO_CLK(CK)  (CK.Bit.PA)

#define SDCARD_SD_DAT0_GPIO_ID      (GPIO_PC)
#define SDCARD_SD_DAT0_AFIO_PIN     (AFIO_PIN_9)
#define SDCARD_SD_DAT0_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT0_GPIO_CLK(CK) (CK.Bit.PC)

#define SDCARD_SD_DAT1_GPIO_ID      (GPIO_PC)
#define SDCARD_SD_DAT1_AFIO_PIN     (AFIO_PIN_10)
#define SDCARD_SD_DAT1_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT1_GPIO_CLK(CK) (CK.Bit.PC)

#define SDCARD_SD_DAT2_GPIO_ID      (GPIO_PC)
#define SDCARD_SD_DAT2_AFIO_PIN     (AFIO_PIN_11)
#define SDCARD_SD_DAT2_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT2_GPIO_CLK(CK) (CK.Bit.PC)

#define SDCARD_SD_DAT3_GPIO_ID      (GPIO_PC)
#define SDCARD_SD_DAT3_AFIO_PIN     (AFIO_PIN_12)
#define SDCARD_SD_DAT3_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT3_GPIO_CLK(CK) (CK.Bit.PC)
#endif

#define SDCARD_SD_CD_GPIO_ID        (GPIO_PE)
#define SDCARD_SD_CD_GPIO_PIN       (GPIO_PIN_13)
#define SDCARD_SD_CD_AFIO_MODE      (AFIO_FUN_DEFAULT)
#define SDCARD_SD_CD_GPIO_CLK(CK)   (CK.Bit.PE)

#define SDCARD_SD_CLK_GPIO_ID       (GPIO_PA)
#define SDCARD_SD_CLK_AFIO_PIN      (AFIO_PIN_5)
#define SDCARD_SD_CLK_AFIO_MODE     (AFIO_FUN_SDIO)
#define SDCARD_SD_CLK_GPIO_CLK(CK)  (CK.Bit.PA)

#define SDCARD_SD_CMD_GPIO_ID       (GPIO_PA)
#define SDCARD_SD_CMD_AFIO_PIN      (AFIO_PIN_4)
#define SDCARD_SD_CMD_AFIO_MODE     (AFIO_FUN_SDIO)
#define SDCARD_SD_CMD_GPIO_CLK(CK)  (CK.Bit.PA)

#define SDCARD_SD_DAT0_GPIO_ID      (GPIO_PA)
#define SDCARD_SD_DAT0_AFIO_PIN     (AFIO_PIN_6)
#define SDCARD_SD_DAT0_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT0_GPIO_CLK(CK) (CK.Bit.PA)

#define SDCARD_SD_DAT1_GPIO_ID      (GPIO_PA)
#define SDCARD_SD_DAT1_AFIO_PIN     (AFIO_PIN_1)
#define SDCARD_SD_DAT1_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT1_GPIO_CLK(CK) (CK.Bit.PA)

#define SDCARD_SD_DAT2_GPIO_ID      (GPIO_PA)
#define SDCARD_SD_DAT2_AFIO_PIN     (AFIO_PIN_2)
#define SDCARD_SD_DAT2_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT2_GPIO_CLK(CK) (CK.Bit.PA)

#define SDCARD_SD_DAT3_GPIO_ID      (GPIO_PA)
#define SDCARD_SD_DAT3_AFIO_PIN     (AFIO_PIN_3)
#define SDCARD_SD_DAT3_AFIO_MODE    (AFIO_FUN_SDIO)
#define SDCARD_SD_DAT3_GPIO_CLK(CK) (CK.Bit.PA)

/**
  * @}
  */

/**
  * @}
  */

/* A0 ~ A18 */


#define HTCFG_EBI_A0_ID               (GPIO_PD)
#define HTCFG_EBI_A0_PIN              (AFIO_PIN_8)

/*
#define HTCFG_EBI_A0_ID               (GPIO_PA)
#define HTCFG_EBI_A0_PIN              (AFIO_PIN_11)
*/

#define HTCFG_EBI_A1_ID               (GPIO_PA)
#define HTCFG_EBI_A1_PIN              (AFIO_PIN_9)
#define HTCFG_EBI_A2_ID               (GPIO_PD)
#define HTCFG_EBI_A2_PIN              (AFIO_PIN_7)
#define HTCFG_EBI_A3_ID               (GPIO_PD)
#define HTCFG_EBI_A3_PIN              (AFIO_PIN_9)
#define HTCFG_EBI_A4_ID               (GPIO_PD)
#define HTCFG_EBI_A4_PIN              (AFIO_PIN_10)
#define HTCFG_EBI_A5_ID               (GPIO_PD)
#define HTCFG_EBI_A5_PIN              (AFIO_PIN_11)
#define HTCFG_EBI_A6_ID               (GPIO_PD)
#define HTCFG_EBI_A6_PIN              (AFIO_PIN_12)
#define HTCFG_EBI_A7_ID               (GPIO_PD)
#define HTCFG_EBI_A7_PIN              (AFIO_PIN_13)
#define HTCFG_EBI_A8_ID               (GPIO_PD)
#define HTCFG_EBI_A8_PIN              (AFIO_PIN_14)
#define HTCFG_EBI_A9_ID               (GPIO_PD)
#define HTCFG_EBI_A9_PIN              (AFIO_PIN_15)
#define HTCFG_EBI_A10_ID              (GPIO_PE)
#define HTCFG_EBI_A10_PIN             (AFIO_PIN_0)
#define HTCFG_EBI_A11_ID              (GPIO_PE)
#define HTCFG_EBI_A11_PIN             (AFIO_PIN_1)
#define HTCFG_EBI_A12_ID              (GPIO_PE)
#define HTCFG_EBI_A12_PIN             (AFIO_PIN_2)
#define HTCFG_EBI_A13_ID              (GPIO_PE)
#define HTCFG_EBI_A13_PIN             (AFIO_PIN_3)
#define HTCFG_EBI_A14_ID              (GPIO_PE)
#define HTCFG_EBI_A14_PIN             (AFIO_PIN_4)
#define HTCFG_EBI_A15_ID              (GPIO_PE)
#define HTCFG_EBI_A15_PIN             (AFIO_PIN_5)
#define HTCFG_EBI_A16_ID              (GPIO_PD)
#define HTCFG_EBI_A16_PIN             (AFIO_PIN_4)
#define HTCFG_EBI_A17_ID              (GPIO_PD)
#define HTCFG_EBI_A17_PIN             (AFIO_PIN_5)
#define HTCFG_EBI_A18_ID              (GPIO_PD)
#define HTCFG_EBI_A18_PIN             (AFIO_PIN_0)

/* AD0 ~ AD15 */
#define HTCFG_EBI_AD0_ID              (GPIO_PA)
#define HTCFG_EBI_AD0_PIN             (AFIO_PIN_14)
#define HTCFG_EBI_AD1_ID              (GPIO_PA)
#define HTCFG_EBI_AD1_PIN             (AFIO_PIN_15)
#define HTCFG_EBI_AD2_ID              (GPIO_PB)
#define HTCFG_EBI_AD2_PIN             (AFIO_PIN_0)
#define HTCFG_EBI_AD3_ID              (GPIO_PB)
#define HTCFG_EBI_AD3_PIN             (AFIO_PIN_1)
#define HTCFG_EBI_AD4_ID              (GPIO_PB)
#define HTCFG_EBI_AD4_PIN             (AFIO_PIN_2)
#define HTCFG_EBI_AD5_ID              (GPIO_PB)
#define HTCFG_EBI_AD5_PIN             (AFIO_PIN_3)
#define HTCFG_EBI_AD6_ID              (GPIO_PB)
#define HTCFG_EBI_AD6_PIN             (AFIO_PIN_4)
#define HTCFG_EBI_AD7_ID              (GPIO_PB)
#define HTCFG_EBI_AD7_PIN             (AFIO_PIN_5)
#define HTCFG_EBI_AD8_ID              (GPIO_PC)
#define HTCFG_EBI_AD8_PIN             (AFIO_PIN_7)
#define HTCFG_EBI_AD9_ID              (GPIO_PC)
#define HTCFG_EBI_AD9_PIN             (AFIO_PIN_8)
#define HTCFG_EBI_AD10_ID             (GPIO_PC)
#define HTCFG_EBI_AD10_PIN            (AFIO_PIN_4)
#define HTCFG_EBI_AD11_ID             (GPIO_PC)
#define HTCFG_EBI_AD11_PIN            (AFIO_PIN_5)
#define HTCFG_EBI_AD12_ID             (GPIO_PC)
#define HTCFG_EBI_AD12_PIN            (AFIO_PIN_6)
#define HTCFG_EBI_AD13_ID             (GPIO_PC)
#define HTCFG_EBI_AD13_PIN            (AFIO_PIN_0)
#define HTCFG_EBI_AD14_ID             (GPIO_PC)
#define HTCFG_EBI_AD14_PIN            (AFIO_PIN_1)
#define HTCFG_EBI_AD15_ID             (GPIO_PC)
#define HTCFG_EBI_AD15_PIN            (AFIO_PIN_2)

/* OE, WE, CS1, BL0, BL1 */
#define HTCFG_EBI_OE_ID               (GPIO_PB)
#define HTCFG_EBI_OE_PIN              (AFIO_PIN_6)
#define HTCFG_EBI_WE_ID               (GPIO_PB)
#define HTCFG_EBI_WE_PIN              (AFIO_PIN_8)
#define HTCFG_EBI_CS1_ID              (GPIO_PB)
#define HTCFG_EBI_CS1_PIN             (AFIO_PIN_10)
#define HTCFG_EBI_BL0_ID              (GPIO_PE)
#define HTCFG_EBI_BL0_PIN             (AFIO_PIN_6)
#define HTCFG_EBI_BL1_ID              (GPIO_PE)
#define HTCFG_EBI_BL1_PIN             (AFIO_PIN_7)

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup HT32F12366_DVBTEMP_Exported_Functions HT32F12366 DVB exported functions
  * @{
  */
#define HT32F_DVB_USBConnect()      USBD_DPpullupCmd(ENABLE)
#define HT32F_DVB_USBDisConnect()   USBD_DPpullupCmd(DISABLE)
#define HT32F_DVB_GPxConfig         AFIO_GPxConfig

#define HT32F_DVB_BuzzerInit(hFreq, bDuty)    HT32F_DVB_BuzzerFun(hFreq, bDuty, 0);
#define HT32F_DVB_BuzzerConfig(hFreq, bDuty)  HT32F_DVB_BuzzerFun(hFreq, bDuty, 1);


void HT32F_DVB_LEDInit(LED_TypeDef Led);
void HT32F_DVB_LEDOn(LED_TypeDef Led);
void HT32F_DVB_LEDOff(LED_TypeDef Led);
void HT32F_DVB_LEDToggle(LED_TypeDef Led);

void HT32F_DVB_PBInit(BUTTON_TypeDef Button, BUTTON_MODE_TypeDef Button_Mode);
u32 HT32F_DVB_PBGetState(BUTTON_TypeDef Button);

void HT32F_DVB_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

void HT32F_DVB_BuzzerFun(u16 hFreq, u8 bDuty, u32 mode);
void HT32F_DVB_BuzzerOutputCmd(ControlStatus Control);
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

#ifdef __cplusplus
}
#endif

#endif
