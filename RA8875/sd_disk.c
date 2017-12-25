/*********************************************************************************************************//**
 * @file    SD/SD_FAT_Test/sd_disk.c
 * @version $Rev:: 279          $
 * @date    $Date:: 2016-12-05 #$
 * @brief   The SD disk interface of FAT file system.
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
#include "ht32.h"
#include "sd_disk.h"
#include "sdio_sd.h"
#include "integer.h"
#include <stdio.h>
#include <string.h>
/* Settings ------------------------------------------------------------------------------------------------*/
//#define SDISK_DEBUG

/* Private macro -------------------------------------------------------------------------------------------*/
#ifdef SDISK_DEBUG
  #define SKDBG printf
#else
  #define SKDBG(...)
#endif
/* Private functions ---------------------------------------------------------------------------------------*/
u32 SD_IsWriteBusy(void);
void SD_ClearSPIFIFO(void);

/* Private variables ---------------------------------------------------------------------------------------*/
static volatile DSTATUS DiskState = STA_NOINIT;  /* Disk status                                             */
SD_CardInfo SDCardInfo;
extern SD_CardInfo gSDCardInfo;
/* Global functions ----------------------------------------------------------------------------------------*/
void dump(char *data,int len)
{
  #ifdef SDISK_DEBUG
  int i=0;
  for(i=0;i<len;i++)
  {
    if(i%8==0) printf("\r\n");
    printf("0x%02X ",data[i]);

  }
  #endif
}
/*********************************************************************************************************//**
  * @brief  Disk initialize.
  * @param  drv: Physical drive nmuber (should be 0)
  * @retval DiskState: STA_NOINIT, STA_NODISK, STA_PROTECT
  ***********************************************************************************************************/
DSTATUS disk_initialize(BYTE drv)
{
  if (drv != 0)
    return STA_NOINIT;
  if (SD_Init(&SDCardInfo) == SD_OK)
  {
    DiskState &= ~STA_NOINIT;
    printf("\r\nPASS");
    printf("\r\n\tCardType : %u", SDCardInfo.CardType);
    printf("\r\n\tCapacity : %u Bytes", SDCardInfo.CardCapacity);
    printf("\r\n\tBlockSize: %u", SDCardInfo.CardBlockSize);
    printf("\r\n\tRCA      : 0x%04x", SDCardInfo.RCA);
    printf("\r\n\tManufID  : %u", SDCardInfo.SD_cid.ManufacturerID);
  }

  return DiskState;
}

/*********************************************************************************************************//**
  * @brief  Return SD disk current status.
  * @param  drv: Physical drive nmuber (should be 0)
  * @retval DiskState: STA_NOINIT, STA_NODISK, STA_PROTECT
  ***********************************************************************************************************/
DSTATUS disk_status(BYTE drv)
{
  if (drv != 0)
    return STA_NOINIT;

  return DiskState;
}

/*********************************************************************************************************//**
  * @brief  Read the specific sector(s).
  * @param  drv: physical drive nmuber (should be 0)
  * @param  buf: Pointer to the data buffer to store read data
  * @param  sector: Sector number of read beginning
  * @param  count: Sector count
  * @retval RES_OK, RES_ERROR, RES_NOTRDY, RES_PARERR
  ***********************************************************************************************************/
DRESULT disk_read(BYTE drv, BYTE *buf, DWORD sector, BYTE count)
{
  u32 *u32ReadBuff=(u32*)buf;
  if (drv != 0)
    return RES_PARERR;
  if (count == 0)
    return RES_PARERR;
  if (DiskState & STA_NOINIT)
    return RES_NOTRDY;
  SKDBG("\r\nRxData:\r\n");
  SKDBG("SD info:\r\n");
  dump((char*)&SDCardInfo,sizeof(SD_CardInfo));
  SKDBG("sector:%d\r\n",sector);
  SKDBG("count:%d\r\n",count);
  sector *= 512;
  if (count == 1)
  {
    /* Single block read                                                                                    */
    //if (SD_ReadBlock(buf, sector, 512) == SD_RESPONSE_NOERR)
    if (SD_ReadBlock(&SDCardInfo, sector,u32ReadBuff, 512) == SD_OK)
      count = 0;
  }
  else
  {
    /* Multiple block read                                                                                  */
    if (SD_ReadMultiBlocks(&SDCardInfo, sector, u32ReadBuff,  512, count) == SD_OK)
      count = 0;
  }
  if(count>0)
  {
    SKDBG("Error\r\n");
  }
  SKDBG("\r\nData:\r\n");
    dump((char*)buf,512);
  SKDBG("\r\n");
//  disk_u32tou8(u32ReadBuff,buf,512*count);
  return (count ? RES_ERROR : RES_OK);
}

/*********************************************************************************************************//**
  * @brief  Write the specific sector(s).
  * @param  drv: physical drive nmuber (should be 0)
  * @param  buf: Pointer to the data buffer to store read data
  * @param  sector: Sector number of read beginning
  * @param  count: Sector count
  * @retval RES_OK, RES_ERROR, RES_WRPRT, RES_NOTRDY, RES_PARERR
  ***********************************************************************************************************/
DRESULT disk_write(BYTE drv, const BYTE *buf,    DWORD sector, BYTE count)
{
  u32 *u32WriteBuff=(u32*)buf;
//  disk_u8tou32(buf,u32WriteBuff,512*count);
  if (drv != 0)
    return RES_PARERR;
  if (count == 0)
    return RES_PARERR;
  if (DiskState & STA_NOINIT)
    return RES_NOTRDY;
  if (DiskState & STA_PROTECT)
    return RES_WRPRT;

  SKDBG("SD info:\r\n");
  dump((char*)&SDCardInfo,sizeof(SD_CardInfo));
  SKDBG("sector:%l\r\n",sector);
  SKDBG("count:%d\r\n",count);
  SKDBG("\r\nTxData:\r\n");
  dump((char*)buf,count*sector);
  sector *= 512;
  if (count == 1)
  {
    /* Single block write                                                                                   */
    if (SD_WriteBlock(&SDCardInfo, sector,u32WriteBuff, 512) == SD_OK)
      count = 0;
  }
  else
  {
      /* Multiple block write                                                                                 */
      if (SD_WriteMultiBlocks(&SDCardInfo,sector ,u32WriteBuff ,512, count) == SD_OK)
        count = 0;
  }

  return (count ? RES_ERROR : RES_OK);
}

/*********************************************************************************************************//**
  * @brief  Support some commands for file system control (incomplete).
  * @param  drv: Physical drive nmuber (should be 0)
  *         @arg ARGDESCRIPTION
  *         @arg ARGDESCRIPTION
  * @param  ctrl: Control code
  * @param  buf: Point to send or receive control data
  * @retval RES_OK, RES_ERROR, RES_NOTRDY, RES_PARERR
  ***********************************************************************************************************/
DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buf)
{
  DWORD temp1, temp2;
  DRESULT result = RES_ERROR;;
  if (drv != 0)
    return RES_PARERR;

  if (ctrl == CTRL_POWER)
  {
    // ..... not yet
  }
  else
  {
    if (DiskState & STA_NOINIT)
      return RES_NOTRDY;

    switch (ctrl)
    {
      case CTRL_SYNC:
      {
        if (SD_IsWriteBusy() != TRUE)
          result = RES_OK;
        break;
      }
      case GET_SECTOR_SIZE:
      {
        *(WORD*) buf = 512;
        result = RES_OK;
        break;
      }
      case GET_SECTOR_COUNT:
      {
        if (SD_GetCSDRegister(&SDCardInfo) == SD_OK)
        {
          if (SDCardInfo.SD_csd.CSDStruct != 0x1)
          {
            temp1 = SDCardInfo.SD_csd.DeviceSize + 1;
            temp2 = (1 << (SDCardInfo.SD_csd.DeviceSizeMul + 2)) * ((1 << SDCardInfo.SD_csd.RdBlockLen) / 512);
            *(WORD*) buf = temp1 * temp2;
          }
          else
          {
            *(WORD*) buf = (SDCardInfo.SD_csd.DeviceSize + 1) * 512;
          }
          result = RES_OK;
        }
        break;
      }
      case GET_BLOCK_SIZE:                                        // ..... check
      {
        if (SD_GetCSDRegister(&SDCardInfo) == SD_OK)
        {
          *(WORD*) buf = SDCardInfo.SD_csd.EraseGrMul;
          result = RES_OK;
        }
        break;
      }
      default:
      {
        result = RES_PARERR;
      }
    }
  }

  return result;
}

/*********************************************************************************************************//**
  * @brief  Get FAT time.
  * @retval Always 0 (Not implement)
  ***********************************************************************************************************/
DWORD get_fattime(void)
{
  return 0;
}

/*********************************************************************************************************//**
  * @brief  Check the block write status.
  * @retval TRUE: write operation is busy. FALSE: write operation is finished.
  ***********************************************************************************************************/
u32 SD_IsWriteBusy(void)
{
  if(SDIO_GetFlagStatus(SDIO_FLAG_TRANS_END))
  {
    return TRUE;
  }
  return FALSE;
}

