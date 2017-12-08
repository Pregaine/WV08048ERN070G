/*********************************************************************************************************//**
 * @file    SD/SD_FAT_Test/sd_disk.h
 * @version $Rev:: 142          $
 * @date    $Date:: 2016-10-27 #$
 * @brief   The header file of SD disk interface with FAT file system.
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
#ifndef __SD_DISK_H
#define __SD_DISK_H

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "integer.h"

/* Settings ------------------------------------------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------------------------------------*/
typedef bool BOOL;
typedef BYTE DSTATUS;

/* Results of Disk Functions                                                                                */
typedef enum {
    RES_OK = 0,       /* 0: Successful                                                                      */
    RES_ERROR,        /* 1: R/W Error                                                                       */
    RES_WRPRT,        /* 2: Write Protected                                                                 */
    RES_NOTRDY,       /* 3: Not Ready                                                                       */
    RES_PARERR        /* 4: Invalid Parameter                                                               */
} DRESULT;

/* Exported constants --------------------------------------------------------------------------------------*/
/* Disk Status Bits (DSTATUS)                                                                               */
#define STA_NOINIT        (0x01)  /* Drive not initialized                                                  */
#define STA_NODISK        (0x02)  /* No medium in the drive                                                 */
#define STA_PROTECT       (0x04)  /* Write protected                                                        */

/* Command code for disk_ioctrl()                                                                           */
/* Generic command                                                                                          */
#define CTRL_SYNC          (0)    /* Mandatory for write functions                                          */
#define GET_SECTOR_COUNT   (1)    /* Mandatory for only f_mkfs()                                            */
#define GET_SECTOR_SIZE    (2)
#define GET_BLOCK_SIZE     (3)    /* Mandatory for only f_mkfs()                                            */
#define CTRL_POWER         (4)
#define CTRL_LOCK          (5)
#define CTRL_EJECT         (6)

/* Exported macro ------------------------------------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
DSTATUS disk_initialize(BYTE drv);
DSTATUS disk_status(BYTE drv);
DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count);
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count);
DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff);
DWORD get_fattime(void);
#endif /* __SD_DISK_H --------------------------------------------------------------------------------------*/
