/***************************************************************************************************
                                   ExploreEmbedded
****************************************************************************************************
 * File:   rtc.c
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the library functions for RTC-Ds1307 date and time read write

The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the
library works on any of development boards for respective controllers. However, ExploreEmbedded
disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
Files may be subject to change without prior notice. The revision history contains the information
related to updates.


GNU GENERAL PUBLIC LICENSE:
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Errors and omissions should be reported to codelibraries@exploreembedded.com
***************************************************************************************************/



/***************************************************************************************************
                             Revision History
****************************************************************************************************
15.0: Initial version
***************************************************************************************************/
#include "ht32.h"
#include "ht32_board.h"
#include "RTC.h"
#include "common/i2c_eeprom.h"


uint8_t TM_DS1307_Bcd2Bin(uint8_t bcd)
{
	u8 dec = 10 * (bcd >> 4);
	dec += bcd & 0x0F;

	return dec;
}

uint8_t TM_DS1307_Bin2Bcd(uint8_t bin)
{
	u8 low = 0;
	u8 high = 0;

	/* High nibble */
	high = bin / 10;
	/* Low nibble */
	low = bin - (high * 10);

	/* Return */
	return high << 4 | low;
}

uint8_t TM_DS1307_CheckMinMax( u8 val, u8 min, u8 max )
{
	if (val < min)
	{
		return min;
	}
	else if (val > max)
	{
		return max;
	}
	return val;
}

void _I2C_AckPolling( HT_I2C_TypeDef *I2Cx, u8 DevAdd, u32 I2C_Direction )
{
	u32 reg;

	/* wait if bus busy                                                                                       */
	while( I2C_GetFlagStatus( I2Cx, I2C_FLAG_BUSBUSY ) );

	while (1)
	{
		/* send slave address                                                                                   */
		I2C_TargetAddressConfig( I2Cx, DevAdd, I2C_Direction );

		/* check status                                                                                         */
		while( 1 )
		{
			reg = I2Cx->SR;

			if ( reg & I2C_FLAG_ADRS )
			{
				return;
			}

			if ( reg & I2C_FLAG_RXNACK )
			{
				I2C_ClearFlag( EEPROM_I2C, I2C_FLAG_RXNACK );
				break;
			}
		}
	}

}


#if 0
void I2C_BufferRead( HT_I2C_TypeDef* I2Cx, u8* pBuffer, u8 DevAddr, u8 ReadAddr, u16 NumByteToRead )
{
	Assert_Param(NumByteToRead > 0);



  	AckPolling(DevAddr);



  while (!I2C_CheckStatus(EEPROM_I2C, I2C_MASTER_TX_EMPTY));
  I2C_SendData(EEPROM_I2C, ReadAddr);
  I2C_TargetAddressConfig(EEPROM_I2C, DevAddr, I2C_MASTER_READ);
  while (!I2C_CheckStatus(EEPROM_I2C, I2C_MASTER_SEND_START));
  while (!I2C_CheckStatus(EEPROM_I2C, I2C_MASTER_RECEIVER_MODE));
  if (NumByteToRead > 1){
    I2C_AckCmd(EEPROM_I2C, ENABLE);
  }
  while (NumByteToRead){
    while (!I2C_CheckStatus(EEPROM_I2C, I2C_MASTER_RX_NOT_EMPTY));
    *pBuffer = I2C_ReceiveData(EEPROM_I2C);
    pBuffer++;
    NumByteToRead--;

    if (NumByteToRead == 1){
      I2C_AckCmd(EEPROM_I2C, DISABLE);
    }
  }
  I2C_GenerateSTOP(EEPROM_I2C);

}

void I2C_BufferWrite( HT_I2C_TypeDef* I2Cx, u8* pBuffer, u8 DevAddr, u8 WriteAddr, u16 NumByteToWrite )
{
	while( !I2C_CheckStatus( I2Cx, I2C_MASTER_TX_EMPTY ) );

    printf( "\r\nwhile( !I2C_CheckStatus( I2Cx, I2C_MASTER_TX_EMPTY ) );" );

	/* Send I2C0 START & I2C1 slave address for write*/
	I2C_TargetAddressConfig( I2Cx, DevAddr, I2C_MASTER_WRITE );

	I2C_SendData( I2Cx, WriteAddr );

    printf( "\r\nI2C_SendData( I2Cx, WriteAddr )" );

	/* Check on Master Transmitter STA condition and clear it */
	// while ( !I2C_CheckStatus( I2Cx, I2C_MASTER_SEND_START ) );

	/* Check on Slave Receiver ADRS condition and clear it*/
	// while ( !I2C_CheckStatus( I2Cx, I2C_MASTER_RECEIVER_MODE ) );

	/* Send data*/
	while( NumByteToWrite )
	{
		/* Check on Master Transmitter TXDE condition */
		while (!I2C_CheckStatus( I2Cx, I2C_MASTER_TX_EMPTY) );

		/* Send I2C0 data */
		I2C_SendData( I2Cx, *pBuffer++ );

		NumByteToWrite --;

        printf( "\r\nNumByteToWrite %d", NumByteToWrite );

		/* Check on Slave Receiver RXDNE condition */
		while ( !I2C_CheckStatus( I2Cx, I2C_MASTER_TX_EMPTY ) );

		// Store received data on I2C1
		// I2C1_Buffer_Rx[Rx_Index ++ ] = I2C_ReceiveData(HT_I2C1);
	}

	/* Send I2C0 STOP condition */
	I2C_GenerateSTOP( I2Cx );

	/* Check on Slave Receiver STO condition */
	// while ( !I2C_CheckStatus( I2Cx, I2C_SLAVE_STOP_DETECTED ) );

}

#endif

/***************************************************************************************************
                         void RTC_Init(void)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value    : none

 * description :This function is used to Initialize the Ds1307 RTC.
***************************************************************************************************/
void RTC_Init( void )
{
	u8 buf = 0;

	// I2C_EEPROM_BufferWrite( &buf, 0x68, 1 );

	/*
    I2C_Start();                            // Start I2C communication

    I2C_Write(C_Ds1307WriteMode_U8);        // Connect to DS1307 by sending its ID on I2c Bus
    I2C_Write(C_Ds1307ControlRegAddress_U8);// Select the Ds1307 ControlRegister to configure Ds1307

    I2C_Write(0x00);                        // Write 0x00 to Control register to disable SQW-Out

    I2C_Stop();                             // Stop I2C communication after initializing DS1307
	*/


}



/***************************************************************************************************
                    void RTC_SetDateTime(rtc_t *rtc)
****************************************************************************************************
 * I/P Arguments: rtc_t *: Pointer to structure of type rtc_t. Structure contains hour,min,sec,day,date,month and year
 * Return value    : none

 * description  :This function is used to update the Date and time of Ds1307 RTC.
                 The new Date and time will be updated into the non volatile memory of Ds1307.
        Note: The date and time should be of BCD format,
             like 0x12,0x39,0x26 for 12hr,39min and 26sec.
                  0x15,0x08,0x47 for 15th day,8th month and 47th year.
***************************************************************************************************/
void RTC_SetDateTime( rtc_t *rtc )
{
	u8 i;
 	unsigned char data[ 7 ] = { 0 };

 	rtc->sec = 20;
 	rtc->min = 32;
 	rtc->hour = 13;
	rtc->weekDay = 5;
	rtc->date  = 5;
	rtc->month = 1;
 	rtc->year  = 18;

	#if 1
	/* Format data */
	data[DS1307_SECONDS] = TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->sec, 0, 59 ) );
	data[DS1307_MINUTES] = TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->min, 0, 59 ) );
	data[DS1307_HOURS] 	 = TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->hour, 0, 23 ) );
	data[DS1307_DAY]     = TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->weekDay, 1, 7 ) );
	data[DS1307_DATE]    = TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->date, 1, 31 ) );
	data[DS1307_MONTH]   = TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->month, 1, 12 ) );
	data[DS1307_YEAR]    = TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->year, 0, 99 ) );
	#endif

	#if 0
	printf( "\r\nrtc->weekDay %d, TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->weekDay, 1, 7 ) ) %x", rtc->weekDay, TM_DS1307_Bin2Bcd( TM_DS1307_CheckMinMax( rtc->weekDay, 1, 7 ) ) );

	for( i = 0; i < 8; i++ )
		printf( "\r\n data[ %d ] 0x%x", i, data[ i ] );
	#endif


	/* Write to device */
	I2C_BufferWrite( EEPROM_I2C, data, 0x68, 0, 8 );

	#if 0
    I2C_Start();                          	// Start I2C communication

    I2C_Write(C_Ds1307WriteMode_U8);      	// connect to DS1307 by sending its ID on I2c Bus
    I2C_Write(C_Ds1307SecondRegAddress_U8); // Request sec RAM address at 00H

    I2C_Write(rtc->sec);                    // Write sec from RAM address 00H
    I2C_Write(rtc->min);                    // Write min from RAM address 01H
    I2C_Write(rtc->hour);                   // Write hour from RAM address 02H
    I2C_Write(rtc->weekDay);                // Write weekDay on RAM address 03H
    I2C_Write(rtc->date);                   // Write date on RAM address 04H
    I2C_Write(rtc->month);                  // Write month on RAM address 05H
    I2C_Write(rtc->year);                   // Write year on RAM address 06h

    I2C_Stop();                             // Stop I2C communication after Setting the Date
    #endif

}







/***************************************************************************************************
                     void RTC_GetDateTime(rtc_t *rtc)
****************************************************************************************************
 * I/P Arguments: rtc_t *: Pointer to structure of type rtc_t. Structure contains hour,min,sec,day,date,month and year
 * Return value    : none

 * description  :This function is used to get the Date(d,m,y) from Ds1307 RTC.

    Note: The date and time read from Ds1307 will be of BCD format,
          like 0x12,0x39,0x26 for 12hr,39min and 26sec.
               0x15,0x08,0x47 for 15th day,8th month and 47th year.
***************************************************************************************************/
void RTC_GetDateTime( rtc_t *rtc )
{
	u8 buf[ 8 ] = { 0 };
	u8 i;

	I2C_BufferRead( buf, 0x68, 0, 8 );	//EEPROM_I2C, buf, 0x50

	rtc->sec = TM_DS1307_Bcd2Bin( buf[ 0 ] );

	rtc->min = TM_DS1307_Bcd2Bin( buf[ 1 ] );

	rtc->hour = TM_DS1307_Bcd2Bin( buf[ 2 ] );

	rtc->weekDay = TM_DS1307_Bcd2Bin( buf[ 3 ] );

	rtc->date = TM_DS1307_Bcd2Bin( buf[ 4 ] );

	rtc->month = TM_DS1307_Bcd2Bin( buf[ 5 ] );

	rtc->year = TM_DS1307_Bcd2Bin( buf[ 6 ] );

	#if 0
    I2C_Start();                            // Start I2C communication

    I2C_Write(C_Ds1307WriteMode_U8);        // connect to DS1307 by sending its ID on I2c Bus
    I2C_Write(C_Ds1307SecondRegAddress_U8); // Request Sec RAM address at 00H

    I2C_Stop();                             // Stop I2C communication after selecting Sec Register

    I2C_Start();                            // Start I2C communication
    I2C_Write(C_Ds1307ReadMode_U8);            // connect to DS1307(Read mode) by sending its ID

    rtc->sec = I2C_Read(1);                // read second and return Positive ACK
    rtc->min = I2C_Read(1);                 // read minute and return Positive ACK
    rtc->hour= I2C_Read(1);               // read hour and return Negative/No ACK
    rtc->weekDay = I2C_Read(1);           // read weekDay and return Positive ACK
    rtc->date= I2C_Read(1);              // read Date and return Positive ACK
    rtc->month=I2C_Read(1);            // read Month and return Positive ACK
    rtc->year =I2C_Read(0);             // read Year and return Negative/No ACK

    I2C_Stop();                              // Stop I2C communication after reading the Date

    #endif


}

