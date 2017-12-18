#include "RA8875.h"
#include "ebi_lcd.h"
#include "font.h"


void KeyBoard_Int( void )
{

	EXTI_InitTypeDef EXTI_InitStruct =
	{
		0
	};
	CKCU_PeripClockConfig_TypeDef CKCUClock =
	{
		{
			0
		}
	};

	CKCUClock.Bit.PD	= 1;
	CKCUClock.Bit.PB	= 1;
	CKCUClock.Bit.PE	= 1;
	CKCUClock.Bit.AFIO	= 1;
	CKCUClock.Bit.EXTI	= 1;
	CKCU_PeripClockConfig( CKCUClock,ENABLE );

	AFIO_GPxConfig( GPIO_PD,AFIO_PIN_1,AFIO_MODE_DEFAULT ); //Key4
	AFIO_GPxConfig( GPIO_PD,AFIO_PIN_2,AFIO_MODE_DEFAULT ); //Key 3
	AFIO_GPxConfig( GPIO_PE,AFIO_PIN_14,AFIO_MODE_DEFAULT ); //Key2
	AFIO_GPxConfig( GPIO_PE,AFIO_PIN_13,AFIO_MODE_DEFAULT ); //Key1

	AFIO_GPxConfig( GPIO_PB,AFIO_PIN_9,AFIO_MODE_DEFAULT ); //Key1


	GPIO_DirectionConfig( HT_GPIOD,GPIO_PIN_1,GPIO_DIR_IN );
	GPIO_DirectionConfig( HT_GPIOD,GPIO_PIN_2,GPIO_DIR_IN );
	GPIO_DirectionConfig( HT_GPIOE,GPIO_PIN_14,GPIO_DIR_IN );
	GPIO_DirectionConfig( HT_GPIOE,GPIO_PIN_13,GPIO_DIR_IN );

	GPIO_DirectionConfig( HT_GPIOB,GPIO_PIN_9,GPIO_DIR_IN );


	GPIO_PullResistorConfig( HT_GPIOD,GPIO_PIN_1,GPIO_PR_UP );
	GPIO_PullResistorConfig( HT_GPIOD,GPIO_PIN_2,GPIO_PR_UP );
	GPIO_PullResistorConfig( HT_GPIOE,GPIO_PIN_14,GPIO_PR_UP );
	GPIO_PullResistorConfig( HT_GPIOE,GPIO_PIN_13,GPIO_PR_UP );

	GPIO_PullResistorConfig( HT_GPIOB,GPIO_PIN_9,GPIO_PR_UP );


	GPIO_InputConfig( HT_GPIOD,GPIO_PIN_1,ENABLE );
	GPIO_InputConfig( HT_GPIOD,GPIO_PIN_2,ENABLE );
	GPIO_InputConfig( HT_GPIOE,GPIO_PIN_14,ENABLE );
	GPIO_InputConfig( HT_GPIOE,GPIO_PIN_13,ENABLE );

	GPIO_InputConfig( HT_GPIOB,GPIO_PIN_9,ENABLE );


	AFIO_EXTISourceConfig( AFIO_EXTI_CH_1,AFIO_ESS_PD );
	AFIO_EXTISourceConfig( AFIO_EXTI_CH_2,AFIO_ESS_PD );
	AFIO_EXTISourceConfig( AFIO_EXTI_CH_14,AFIO_ESS_PE );
	AFIO_EXTISourceConfig( AFIO_EXTI_CH_13,AFIO_ESS_PE );

	AFIO_EXTISourceConfig( AFIO_EXTI_CH_9,AFIO_ESS_PB );


	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_1;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;
	EXTI_Init( &EXTI_InitStruct );

	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_2;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;
	EXTI_Init( &EXTI_InitStruct );

	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_14;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;
	EXTI_Init( &EXTI_InitStruct );


	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_13;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;
	EXTI_Init( &EXTI_InitStruct );

	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_9;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_ENABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;
	EXTI_Init( &EXTI_InitStruct );

	GPIO_InputConfig( HT_GPIOD,GPIO_PIN_1,ENABLE );
	GPIO_InputConfig( HT_GPIOD,GPIO_PIN_2,ENABLE );
	GPIO_InputConfig( HT_GPIOE,GPIO_PIN_14,ENABLE );
	GPIO_InputConfig( HT_GPIOE,GPIO_PIN_13,ENABLE );

	GPIO_InputConfig( HT_GPIOB,GPIO_PIN_9,ENABLE );


	AFIO_EXTISourceConfig( AFIO_EXTI_CH_1,AFIO_ESS_PD );
	AFIO_EXTISourceConfig( AFIO_EXTI_CH_2,AFIO_ESS_PD );
	AFIO_EXTISourceConfig( AFIO_EXTI_CH_14,AFIO_ESS_PE );
	AFIO_EXTISourceConfig( AFIO_EXTI_CH_13,AFIO_ESS_PE );

	AFIO_EXTISourceConfig( AFIO_EXTI_CH_9,AFIO_ESS_PB );

	NVIC_EnableIRQ( EXTI1_IRQn );
	NVIC_EnableIRQ( EXTI2_IRQn );
	NVIC_EnableIRQ( EXTI14_IRQn );
	NVIC_EnableIRQ( EXTI13_IRQn );

	NVIC_EnableIRQ( EXTI9_IRQn );

	EXTI_IntConfig( EXTI_CHANNEL_1,ENABLE );
	EXTI_IntConfig( EXTI_CHANNEL_2,ENABLE );
	EXTI_IntConfig( EXTI_CHANNEL_14,ENABLE );
	EXTI_IntConfig( EXTI_CHANNEL_13,ENABLE );

	EXTI_IntConfig( EXTI_CHANNEL_9,ENABLE );

}


void LCD_Init( void )
{
	EBI_InitTypeDef EBI_InitStructure;

	//Enable AFIO clock
	//Enable EBI & EBI CS port clock
	CKCU_PeripClockConfig_TypeDef CKCUClock =
	{
		{
			0
		}
	};
	LCD_EBI_RST_CLK( CKCUClock ) = 1;
	CKCUClock.Bit.PA	= 1;
	CKCUClock.Bit.PB	= 1;
	CKCUClock.Bit.PC	= 1;
	CKCUClock.Bit.PD	= 1;
	CKCUClock.Bit.PE	= 1;

#if (								LCD_EBI_BL_ENABLE == 1)
	LCD_EBI_BL_CLK( CKCUClock ) = 1;
#endif

	CKCUClock.Bit.EBI	= 1;
	CKCUClock.Bit.AFIO	= 1;

	CKCUClock.Bit.PDMA	= 1;						//20170925
	CKCUClock.Bit.SDIO	= 1;						//20170925

	CKCU_PeripClockConfig( CKCUClock,ENABLE );

	//Configure EBI pins & LCD RESET pin

	HT32F_DVB_GPxConfig( LCD_EBI_CS_GPIO_ID,LCD_EBI_CS_AFIO_PIN,LCD_EBI_CS_AFIO_MODE );

	HT32F_DVB_GPxConfig( LCD_EBI_OE_GPIO_ID,LCD_EBI_OE_AFIO_PIN,LCD_EBI_OE_AFIO_MODE ); // Read Enable

	HT32F_DVB_GPxConfig( LCD_EBI_WE_GPIO_ID,LCD_EBI_WE_AFIO_PIN,LCD_EBI_WE_AFIO_MODE );

	// #if (SPIDUALOUT_WITH_EBI_8BIT_MODE == 0)
	//	HT32F_DVB_GPxConfig(LCD_EBI_RS_GPIO_ID, LCD_EBI_RS_AFIO_PIN, LCD_EBI_RS_AFIO_MODE);
	// #else
	//	HT32F_DVB_GPxConfig(LCD_EBI_RS1_GPIO_ID, LCD_EBI_RS1_AFIO_PIN, LCD_EBI_RS1_AFIO_MODE);
	// #endif

	// AFIO_GPxConfig(GPIO_WAIT, WAIT, AFIO_FUN_GPIO);
	// GPIO_DirectionConfig(HTCFG_WAIT,WAIT,GPIO_DIR_OUT);
	// Control

	// AFIO_GPxConfig( GPIO_CS,CS,AFIO_FUN_GPIO );
	// AFIO_GPxConfig( GPIO_RD,RD,AFIO_FUN_GPIO );

	// AFIO_GPxConfig(GPIO_WR, WR, AFIO_FUN_GPIO);

	AFIO_GPxConfig( GPIO_DC, DC, AFIO_FUN_GPIO );

	// GPIO_DirectionConfig( HTCFG_CS,CS,GPIO_DIR_OUT );
	// GPIO_DirectionConfig( HTCFG_RD,RD,GPIO_DIR_OUT );

	// GPIO_DirectionConfig( HTCFG_WR,WR,GPIO_DIR_OUT );

	GPIO_DirectionConfig( HTCFG_DC, DC, GPIO_DIR_OUT );

	AFIO_GPxConfig( GPIO_DIP,DIP,AFIO_FUN_GPIO );
	AFIO_GPxConfig( GPIO_RST,RST,AFIO_FUN_GPIO );
	GPIO_DirectionConfig( HTCFG_DIP,DIP,GPIO_DIR_OUT );
	GPIO_DirectionConfig( HTCFG_RST,RST,GPIO_DIR_OUT );

	// AFIO_GPxConfig(GPIO_INT, TP_INT, AFIO_FUN_GPIO);
	// GPIO_DirectionConfig(HTCFG_INT,TP_INT,GPIO_DIR_OUT);
	AFIO_GPxConfig( GPIO_D00,D00,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D01,D01,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D02,D02,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D03,D03,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D04,D04,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D05,D05,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D06,D06,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D07,D07,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D08,D08,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D09,D09,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D10,D10,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D11,D11,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D12,D12,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D13,D13,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D14,D14,AFIO_FUN_EBI );
	AFIO_GPxConfig( GPIO_D15,D15,AFIO_FUN_EBI );

	AFIO_GPxConfig( GPIO_DIP,DIP,AFIO_FUN_GPIO );
	AFIO_GPxConfig( GPIO_RST,RST,AFIO_FUN_GPIO );
	GPIO_DirectionConfig( HTCFG_DIP,DIP,GPIO_DIR_OUT );
	GPIO_DirectionConfig( HTCFG_RST,RST,GPIO_DIR_OUT );

	//EBI Configuration
	EBI_InitStructure.EBI_Bank = EBI_BANK_0;

#if ( LCD_BIT_MODE == LCD_16_BIT_MODE )

	EBI_InitStructure.EBI_Mode = EBI_MODE_D16;

#else

	EBI_InitStructure.EBI_Mode = EBI_MODE_D8A8;

#endif

#if (								EBI_FUN_BYTELAND == 1)
	EBI_InitStructure.EBI_ByteLane = EBI_BYTELANE_DISABLE;
	EBI_InitStructure.EBI_ByteLanePolarity = EBI_BYTELANEPOLARITY_LOW;
#endif

	EBI_InitStructure.EBI_IdleCycle = EBI_IDLECYCLE_DISABLE;

#if (								EBI_FUN_ASYNCREADY == 1)
	EBI_InitStructure.EBI_AsynchronousReady = EBI_ASYNCHRONOUSREADY_DISABLE;
	EBI_InitStructure.EBI_ARDYTimeOut = EBI_ARDYTIMEOUT_DISABLE;
	EBI_InitStructure.EBI_ReadySignalPolarity = EBI_READYSIGNALPOLARITY_LOW;
#endif

	EBI_InitStructure.EBI_ChipSelectPolarity = EBI_CHIPSELECTPOLARITY_LOW;
	EBI_InitStructure.EBI_AddressLatchPolarity = EBI_ADDRESSLATCHPOLARITY_LOW;
	EBI_InitStructure.EBI_WriteEnablePolarity = EBI_WRITEENABLEPOLARITY_LOW;
	EBI_InitStructure.EBI_ReadEnablePolarity = EBI_READENABLEPOLARITY_LOW;

	EBI_InitStructure.EBI_IdleCycleTime = 0;
	EBI_InitStructure.EBI_AddressSetupTime = 0;
	EBI_InitStructure.EBI_AddressHoldTime = 0;

#if defined USE_HT32F1654_DVB // reduce speed since pin shared by analog switch

	EBI_InitStructure.EBI_WriteSetupTime = 2;
	EBI_InitStructure.EBI_WriteStrobeTime = 2;
	EBI_InitStructure.EBI_WriteHoldTime = 5;

#else

	EBI_InitStructure.EBI_WriteSetupTime = 3;
	EBI_InitStructure.EBI_WriteStrobeTime = 3;
	EBI_InitStructure.EBI_WriteHoldTime = 3;

#endif

	EBI_InitStructure.EBI_ReadSetupTime  = 3;
	EBI_InitStructure.EBI_ReadStrobeTime = 3;
	EBI_InitStructure.EBI_ReadHoldTime   = 3;

	EBI_InitStructure.EBI_PageMode       = EBI_PAGEMODE_DISABLE;
	EBI_InitStructure.EBI_PageLength     = EBI_PAGELENGTH_4;
	EBI_InitStructure.EBI_PageHitMode 	 = EBI_PAGEHITMODE_ADDINC;
	EBI_InitStructure.EBI_PageAccessTime = 0xF;
	EBI_InitStructure.EBI_PageOpenTime   = 0xFF;

	EBI_Init( &EBI_InitStructure );

	EBI_Cmd( EBI_BANK_0,ENABLE );
}





/*********************************************************************************************************/ /**
  * @brief	Configures the LCD by the EBI lcd driver.
  * @retval None
  ***********************************************************************************************************/
void LCD_Config( void )
{
	Delay100msec();
	LCD_Reset( );


	// GPIO_WriteOutBits( HTCFG_RD, RD, SET );
	LCD_Initial();

	//Active_Window(0,799,0,479);
	CmdWrite( 0x01 );								//Display on
	DataWrite( 0x80 );
	Delay100msec();
	PWM1_enable();
	PWM1_fnuction_sel();
	PWM1_clock_ratio( 0x0A );
	PWM1_duty_cycle( 0xFF );

	GPIO_WriteOutBits( HTCFG_DIP,DIP,SET );

}


void RA8875_PLL_ini( void )
{
	CmdWrite( 0x88 );
	DataWrite( 0x0b );
	Delay1msec();
	CmdWrite( 0x89 );
	DataWrite( 0x02 );
	Delay1msec();
}


void LCD_Reset( void )
{
	//GPIO_WriteOutBits(HTCFG_RST,GPIO_RST,SET);
	GPIO_WriteOutBits( HTCFG_RST,RST,RESET );
	Delay100msec();

	//Delay10msec();
	GPIO_WriteOutBits( HTCFG_RST,RST,SET );
	Delay100msec();

	//Delay10msec();
	//GPIO_WriteOutBits(HTCFG_INT,TP_INT,SET);
}


void PWM1_duty_cycle( u8 setx ) //bit0~7
{
	CmdWrite( 0x8b );								//PTNO
	DataWrite( setx );
}


void PWM1_clock_ratio( u8 setx ) //bit0~3
{
	u8 temp,temp1;
	temp1				= setx & 0x0F;

	//CmdWrite(0x8a);//MCLR
	temp				= DataRead( 0x8A );
	temp				&= 0xF0;
	temp				|= temp1;
	DataWrite( temp );
}


void PWM1_enable( void )
{
	u8 temp;
	temp				= DataRead( 0x8a ); 		//MCLR
	temp				|= 0x80;					//0x80,0x11
	DataWrite( temp );
}


void PWM1_fnuction_sel( void )
{
	u8 temp;
	temp				= DataRead( 0x8A ); 		//MCLR
	temp				&= 0xEF;
	DataWrite( temp );
}


void CmdWrite( u16 cmd )
{
	GPIO_WriteOutBits( HTCFG_DC, DC, SET );

	EBI_LCD->EBI_LCD_REG = cmd;

	GPIO_WriteOutBits( HTCFG_DC, DC, RESET );
}


void DataWrite( u16 data )
{
	GPIO_WriteOutBits( HTCFG_DC, DC, RESET );

	EBI_LCD->EBI_LCD_RAM = data;

	GPIO_WriteOutBits( HTCFG_DC, DC, SET );
}


u8 DataRead( u8 cmd )
{
	u8 Data;

	CmdWrite( cmd );

	// GPIO_WriteOutBits( HTCFG_RD, RD, RESET );

	GPIO_WriteOutBits( HTCFG_DC, DC, RESET );

	Data = EBI_LCD->EBI_LCD_RAM;

	// GPIO_WriteOutBits( HTCFG_RD,RD,SET );

	GPIO_WriteOutBits( HTCFG_DC, DC, SET );

	return Data;
}


u8 LCD_StatusRead( void )
{
	u8 Data;

	// GPIO_WriteOutBits( HTCFG_RD, RD, RESET );

	GPIO_WriteOutBits( HTCFG_DC, DC, RESET );

	Data = EBI_LCD->EBI_LCD_RAM;

	// GPIO_WriteOutBits( HTCFG_RD, RD, SET );

	GPIO_WriteOutBits( HTCFG_DC, DC, SET );

	return Data;
}


void LCD_Initial( void )
{
	RA8875_PLL_ini();
	CmdWrite( 0x10 );								//SYSR	 bit[4:3]=00 256 color	bit[2:1]=  00 8bit MPU interface

	//DataWrite(0x0c); // if 8bit MCU interface   and 65k color display
	DataWrite( 0x0F );								// if 16bit MCU interface	and 65k color display

	//==============	Display Window	800x480 ==================
	CmdWrite( 0x04 );								//PCLK inverse
	DataWrite( 0x81 );
	Delay1msec();

	//Horizontal set
	CmdWrite( 0x14 );								//HDWR//Horizontal Display Width Setting Bit[6:0]
	DataWrite( 0x63 );								//Horizontal display width(pixels) = (HDWR + 1)*8
	CmdWrite( 0x15 );								//Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)
	DataWrite( 0x03 );								//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]
	CmdWrite( 0x16 );								//HNDR//Horizontal Non-Display Period Bit[4:0]
	DataWrite( 0x03 );								//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
	CmdWrite( 0x17 );								//HSTR//HSYNC Start Position[4:0]
	DataWrite( 0x02 );								//HSYNC Start Position(PCLK) = (HSTR + 1)*8
	CmdWrite( 0x18 );								//HPWR//HSYNC Polarity ,The period width of HSYNC.
	DataWrite( 0x00 );								//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8

	//Vertical set
	CmdWrite( 0x19 );								//VDHR0 //Vertical Display Height Bit [7:0]
	DataWrite( 0xdf );								//Vertical pixels = VDHR + 1
	CmdWrite( 0x1a );								//VDHR1 //Vertical Display Height Bit [8]
	DataWrite( 0x01 );								//Vertical pixels = VDHR + 1
	CmdWrite( 0x1b );								//VNDR0 //Vertical Non-Display Period Bit [7:0]
	DataWrite( 0x14 );								//Vertical Non-Display area = (VNDR + 1)
	CmdWrite( 0x1c );								//VNDR1 //Vertical Non-Display Period Bit [8]
	DataWrite( 0x00 );								//Vertical Non-Display area = (VNDR + 1)
	CmdWrite( 0x1d );								//VSTR0 //VSYNC Start Position[7:0]
	DataWrite( 0x06 );								//VSYNC Start Position(PCLK) = (VSTR + 1)
	CmdWrite( 0x1e );								//VSTR1 //VSYNC Start Position[8]
	DataWrite( 0x00 );								//VSYNC Start Position(PCLK) = (VSTR + 1)
	CmdWrite( 0x1f );								//VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	DataWrite( 0x01 );								//VSYNC Pulse Width(PCLK) = (VPWR + 1)

	//Active window  set
	//setting active window X
	CmdWrite( 0x30 );								//Horizontal Start Point 0 of Active Window (HSAW0)
	DataWrite( 0x00 );								//Horizontal Start Point of Active Window [7:0]
	CmdWrite( 0x31 );								//Horizontal Start Point 1 of Active Window (HSAW1)
	DataWrite( 0x00 );								//Horizontal Start Point of Active Window [9:8]
	CmdWrite( 0x34 );								//Horizontal End Point 0 of Active Window (HEAW0)
	DataWrite( 0x1F );								//Horizontal End Point of Active Window [7:0]
	CmdWrite( 0x35 );								//Horizontal End Point 1 of Active Window (HEAW1)
	DataWrite( 0x03 );								//Horizontal End Point of Active Window [9:8]

	//setting active window Y
	CmdWrite( 0x32 );								//Vertical Start Point 0 of Active Window (VSAW0)
	DataWrite( 0x00 );								//Vertical Start Point of Active Window [7:0]
	CmdWrite( 0x33 );								//Vertical Start Point 1 of Active Window (VSAW1)
	DataWrite( 0x00 );								//Vertical Start Point of Active Window [8]
	CmdWrite( 0x36 );								//Vertical End Point of Active Window 0 (VEAW0)
	DataWrite( 0xdf );								//Vertical End Point of Active Window [7:0]
	CmdWrite( 0x37 );								//Vertical End Point of Active Window 1 (VEAW1)
	DataWrite( 0x01 );								//Vertical End Point of Active Window [8]}
}



void SetColor( u16 color )
{
	int i				= 0;
	CmdWrite( 0x02 );

	for ( i = 0; i < 800 * 480; i++ )
	{
		EBI_LCD->EBI_LCD_RAM = color;
	}
}


void SetColor2( u8 color )
{
	int i				= 0;
	CmdWrite( 0x02 );

	for ( i = 0; i < 800 * 480 * 2; i++ )
	{
		EBI_LCD->EBI_LCD_RAM = color;
	}
}


void Graphic_Mode( void )
{
	u8 temp;
	Chk_Busy();
	temp				= DataRead( 0x40 );
	temp				&= 0x7f;
	DataWrite( temp );
}


/*
Bit 7 0:Graphic mode
Bit 7 1:Text mode
*/
void Text_Mode( void )
{
	Chk_Busy();
	Write_Dir( 0x40,0x80 );

	/*
	u8 temp;
	printf("Text_Mode:%x\r\n",temp);
	temp = DataRead(0x40);
	printf("temp:%x\r\n",temp);
	temp |= 0x80 ;
	printf("temp:%x\r\n",temp);
	DataWrite(temp);
	*/
}


void Vertical_FontEnlarge_x1( void )
{
	u8 temp;
	temp				= DataRead( 0x22 );
	temp				&= 0xfc;
	DataWrite( temp );
}


void Horizontal_FontEnlarge_x1( void )
{
	u8 temp;
	temp				= DataRead( 0x22 );
	temp				&= 0xf3;
	DataWrite( temp );
}





/*********************************************************************************************************/ /**
  * @brief	畫三角形第3個點
  * @retval None
  ***********************************************************************************************************/
void Draw_Triangle( u16 X3,u16 Y3 )
{
	u16 temp;
	temp				= X3;
	CmdWrite( 0xA9 );
	DataWrite( temp );
	temp				= X3 >> 8;
	CmdWrite( 0xAA );
	DataWrite( temp );

	temp				= Y3;
	CmdWrite( 0xAB );
	DataWrite( temp );
	temp				= Y3 >> 8;
	CmdWrite( 0xAC );
	DataWrite( temp );
}


void Chk_Busy( void )
{
	u8 temp;

	do
	{
		temp = LCD_StatusRead();
	}
	while( ( temp & 0x80 ) == 0x80 );

	// __NOP;
}


void Chk_Busy_BTE( void )
{
	do
	{
	}
	while( ( LCD_StatusRead() & 0x40 ) == 0x40 );
}


/*
  Bit 7 	0:Full aligment is disable
  Bit 7 	1:Full aligment is enable
  Bit 6 	0:With background color
  Bit 6 	1:With background Transparency
  Bit 4 	0:Normal
  Bit 4 	1:90 degree display
  Bit 3-2  00(x1)~11(x4)
  Bit 1-0  00(x1)~11(x4)
*/
void DrawString( u16 X,u16 Y,u8 * str,u8 size,u8 size2,bool degree,bool transparency,u16 color,u16 color2 )
{
	u8 cmd				= 0x00;
	size				&= 0x03;					//Get Bit0 & Bit1
	size2				&= 0x03;					//Get Bit0 & Bit1
	Font_Coordinate( X,Y );
	Text_Mode();
	Foreground_Color( color );
	Background_color( color2 );
	cmd 				|= size;					//Vertical Font Enlargement
	cmd 				|= size2 << 2;				//Horizontal Font Enlargement

	if ( degree ) //Font Rotation
	{
		cmd 				|= 0x10;
	}

	if ( transparency ) //Font Transparency
	{
		cmd 				|= 0x40;
	}

	Write_Dir( 0x22,cmd );							//Set Font Size

	CmdWrite( 0x02 );

	while ( *str != '\0' )
	{
		DataWrite( *str );
		++str;
		Chk_Busy();
		Delay100usec();
	}
}


void Scroll_Offset( u16 X,u16 Y )
{
	CmdWrite( 0x24 );								//HOFS0
	DataWrite( X );
	CmdWrite( 0x25 );								//HOFS1
	DataWrite( X >> 8 );
	CmdWrite( 0x26 );								//VOFS0
	DataWrite( Y );
	CmdWrite( 0x27 );								//VOFS1
	DataWrite( Y >> 8 );
}


void Scroll( u16 X1,u16 X2,u16 Y1,u16 Y2 )
{
	int i				= 0;
	Chk_Busy();
	Scroll_Window( X1,X2,Y1,Y2 );					//640*480 捲動視窗

	for ( i = 0; i < X2 - X1 + 1; i++ )
	{
		Scroll_Offset( i,0 );
		Delay10msec();
		Delay10msec();
	}
}


void Active_Window( u16 XL,u16 XR,u16 YT,u16 YB )
{
	//setting active window X
	CmdWrite( 0x30 );								//HSAW0
	DataWrite( XL );
	CmdWrite( 0x31 );								//HSAW1
	DataWrite( XL >> 8 );
	CmdWrite( 0x34 );								//HEAW0
	DataWrite( XR );
	CmdWrite( 0x35 );								//HEAW1
	DataWrite( XR >> 8 );

	//setting active window Y
	CmdWrite( 0x32 );								//VSAW0
	DataWrite( YT );
	CmdWrite( 0x33 );								//VSAW1
	DataWrite( YT >> 8 );
	CmdWrite( 0x36 );								//VEAW0
	DataWrite( YB );
	CmdWrite( 0x37 );								//VEAW1
	DataWrite( YB >> 8 );
}


void Scroll_Window( u16 XL,u16 XR,u16 YT,u16 YB )
{
	CmdWrite( 0x38 );								//HSSW0
	DataWrite( XL );
	CmdWrite( 0x39 );								//HSSW1
	DataWrite( XL >> 8 );
	CmdWrite( 0x3c );								//HESW0
	DataWrite( XR );
	CmdWrite( 0x3d );								//HESW1
	DataWrite( XR >> 8 );

	CmdWrite( 0x3a );								//VSSW0
	DataWrite( YT );
	CmdWrite( 0x3b );								//VSSW1
	DataWrite( YT >> 8 );
	CmdWrite( 0x3e );								//VESW0
	DataWrite( YB );
	CmdWrite( 0x3f );								//VESW1
	DataWrite( YB >> 8 );
}


void Print_Hex( u8 buf )
{
	u8 temp;
	temp				= buf;
	temp				= ( temp >> 4 ) & 0x0F;

	if ( temp < 0x0A )
	{
		temp				|= 0x30;
	}
	else
	{
		temp				= temp + 0x37;
	}

	DataWrite( temp );
	Chk_Busy();
	temp				= buf;
	temp				= temp & 0x0F;

	if ( temp < 0x0A )
	{
		temp				|= 0x30;
	}
	else
	{
		temp				= temp + 0x37;
	}

	DataWrite( temp );
	Chk_Busy();
}


void Font_Coordinate( u16 X,u16 Y )
{
	CmdWrite( 0x2A );								//HOFS0
	DataWrite( X );
	CmdWrite( 0x2B );								//HOFS1
	DataWrite( X >> 8 );

	CmdWrite( 0x2C );								//VOFS0
	DataWrite( Y );
	CmdWrite( 0x2D );								//VOFS1
	DataWrite( Y >> 8 );
}


void Font_size_16x16_8x16( void )
{
	u8 temp;
	temp				= DataRead( 0x2E );
	temp				&= 0x3f;
	DataWrite( temp );
}


void Internal_CGROM( void )
{
	u8 temp;
	temp				= DataRead( 0x21 );
	temp				&= cClrD5;
	DataWrite( temp );
}


void Memory_Clear( void )
{
	u8 temp;
	temp				= DataRead( 0x8e );
	temp				|= cSetD7;
	DataWrite( temp );
	Chk_Busy();
}


void Clear_Active_Window( void )
{
	u8 temp;
	temp				= DataRead( 0x8e );
	temp				|= cSetD6;
	DataWrite( temp );
}


void Pattern_Set_16x16( void )
{
	u8 temp;
	temp				= DataRead( 0x66 );
	temp				|= cSetD7;
	DataWrite( temp );
}


void Destination_Layer1( void )
{
	u8 temp;
	temp				= DataRead( 0x5b );
	temp				&= cClrD7;
	DataWrite( temp );
}


void Source_Layer1( void )
{
	u8 temp;
	temp				= DataRead( 0x57 );
	temp				&= cClrD7;
	DataWrite( temp );
}


void Write_To_Bank1( void )
{
	u8 temp;
	temp				= DataRead( 0x41 );
	temp				&= cClrD0;
	DataWrite( temp );
}


void Pattern_Set_number( u8 setx )
{
	u8 temp;
	temp				= DataRead( 0x66 );
	temp				&= 0x80;
	DataWrite( temp | setx );
}


void Write_To_Pattern( void )
{
	u8 temp;

	//CmdWrite(0x41);//MWCR1
	temp				= DataRead( 0x41 );
	temp				&= 0xf3;
	temp				|= cSetD3D2;
	DataWrite( temp );
}


void Write_To_Bank1and2( void )
{
	u8 temp;
	temp				= DataRead( 0x41 );
	temp				&= 0xf3;
	DataWrite( temp );
}


/*
Bit 7 Write 0:No action
Bit 7 Write 1:BTE function enable
Bit 7 Read 0:BTE function is idle
Bit 7 Read 1:BTE function is busy
Bit 6		  0:Source Block mode
Bit 6		  1:Source Linear mode
Bit 5		  0:Destination Block mode
Bit 5		  1:Destination Linear mode
*/
void BTE_enable( void )
{
	Write_Dir( 0x50,0x80 );
}


//REG[51h]
void BTE_ROP_Code( u8 setx )
{
	CmdWrite( 0x51 );								//BECR1
	DataWrite( setx );
}


void Foreground_Color( u16 color )
{
#ifdef Color65k
	CmdWrite( 0x63 );
	DataWrite( ( color & 0xf800 ) >> 11 );

	CmdWrite( 0x64 );
	DataWrite( ( color & 0x07e0 ) >> 5 );

	CmdWrite( 0x65 );
	DataWrite( ( color & 0x001f ) );
#endif

#ifdef Color256
	CmdWrite( 0x63 );
	DataWrite( ( color & 0xf800 ) >> 13 );

	CmdWrite( 0x64 );
	DataWrite( ( color & 0x07e0 ) >> 8 );

	CmdWrite( 0x65 );
	DataWrite( ( color & 0x001f ) >> 3 );
#endif
}


void Background_color( u16 color )
{
#ifdef Color65k
	CmdWrite( 0x60 );
	DataWrite( ( color & 0xf800 ) >> 11 );
	CmdWrite( 0x61 );
	DataWrite( ( color & 0x07e0 ) >> 5 );
	CmdWrite( 0x62 );
	DataWrite( ( color & 0x001f ) );
#endif

#ifdef Color256
	CmdWrite( 0x60 );
	DataWrite( ( color & 0xf800 ) >> 13 );

	CmdWrite( 0x61 );
	DataWrite( ( color & 0x07e0 ) >> 8 );

	CmdWrite( 0x62 );
	DataWrite( ( color & 0x001f ) >> 3 );
#endif
}


void BTE_Source_Destination( u16 XL,u16 XR,u16 YT,u16 YB )
{
	u8 temp,temp1;
	CmdWrite( 0x54 );								//HSBE0
	DataWrite( XL );
	CmdWrite( 0x55 );								//HSBE1
	DataWrite( XL >> 8 );

	CmdWrite( 0x58 );								//HDBE0
	DataWrite( XR );
	CmdWrite( 0x59 );								//HDBE1
	DataWrite( XR >> 8 );

	temp				= YT;
	CmdWrite( 0x56 );								//VSBE0
	DataWrite( temp );
	temp				= YT >> 8;
	temp1				= DataRead( 0x57 ) & 0x80;
	temp				= temp | temp1;
	CmdWrite( 0x57 );
	DataWrite( temp );

	temp				= YB;
	CmdWrite( 0x5a );								//VDBE0
	DataWrite( temp );
	temp				= YB >> 8;
	CmdWrite( 0x5b );								//VDBE1
	temp1				= DataRead( 0x5b ) & 0x80;
	temp				= temp | temp1;
	CmdWrite( 0x5b );								//VDBE1
	DataWrite( temp );
}


void BTE_Size_setting( u16 width,u16 height )
{
	CmdWrite( 0x5c );								//BEWR0 讀寫區域寬度
	DataWrite( width );

	CmdWrite( 0x5d );								//BEWR1 讀寫區域寬度
	DataWrite( width >> 8 );

	CmdWrite( 0x5e );								//BEHR0 讀寫區域高度
	DataWrite( height );

	CmdWrite( 0x5f );								//BEHR1 讀寫區域高度
	DataWrite( height >> 8 );
}


void Write_Dir( u16 Cmd,u16 Data )
{
	CmdWrite( Cmd );
	DataWrite( Data );
}





/*********************************************************************************************************/ /**
  * @brief	畫線,畫矩形,畫三角形 :起始座標
  * @retval None
  ***********************************************************************************************************/
void Draw_Line( u16 XS,u16 XE,u16 YS,u16 YE )
{
	CmdWrite( 0x91 );
	DataWrite( XS );
	CmdWrite( 0x92 );
	DataWrite( XS >> 8 );

	CmdWrite( 0x95 );
	DataWrite( XE );
	CmdWrite( 0x96 );
	DataWrite( XE >> 8 );

	CmdWrite( 0x93 );
	DataWrite( YS );
	CmdWrite( 0x94 );
	DataWrite( YS >> 8 );

	CmdWrite( 0x97 );
	DataWrite( YE );
	CmdWrite( 0x98 );
	DataWrite( YE >> 8 );
}


void DrawCurve( u16 X,u16 Y,u16 R1,u16 R2,u8 ori,u16 BgColor,u16 FgColor,bool bFill )
{
	Chk_Busy();
	CmdWrite( 0xA5 );
	DataWrite( X );
	CmdWrite( 0xA6 );
	DataWrite( X >> 8 );

	CmdWrite( 0xA7 );
	DataWrite( Y );
	CmdWrite( 0xA8 );
	DataWrite( Y >> 8 );

	CmdWrite( 0xA1 );
	DataWrite( R1 );
	CmdWrite( 0xA2 );
	DataWrite( R1 >> 8 );

	CmdWrite( 0xA3 );
	DataWrite( R2 );
	CmdWrite( 0xA4 );
	DataWrite( R2 >> 8 );

	Background_color( BgColor );
	Foreground_Color( FgColor );

	switch ( ori )
	{
		case 4:
			if ( bFill )
			{
				Write_Dir( 0xA0,0x90 | 0x40 );
			}
			else
			{
				Write_Dir( 0xA0,0x90 );
			}

			break;

		case 1:
			if ( bFill )
			{
				Write_Dir( 0xA0,0x91 | 0x40 );
			}
			else
			{
				Write_Dir( 0xA0,0x91 );
			}

			break;

		case 2:
			if ( bFill )
			{
				Write_Dir( 0xA0,0x92 | 0x40 );
			}
			else
			{
				Write_Dir( 0xA0,0x92 );
			}

			break;

		case 3:
			if ( bFill )
			{
				Write_Dir( 0xA0,0x93 | 0x40 );
			}
			else
			{
				Write_Dir( 0xA0,0x93 );
			}

			break;
	}
}


void DrawCircle( u16 X,u16 Y,u16 R,u16 BgColor,u16 FgColor,bool bFill )
{
	Chk_Busy();
	CmdWrite( 0x99 );
	DataWrite( X );
	CmdWrite( 0x9a );
	DataWrite( X >> 8 );
	CmdWrite( 0x9b );
	DataWrite( Y );
	CmdWrite( 0x9c );
	DataWrite( Y >> 8 );
	CmdWrite( 0x9d );
	DataWrite( R );

	Background_color( BgColor );
	Foreground_Color( FgColor );

	if ( bFill )
	{
		Write_Dir( 0x90,0x60 );
	}
	else
	{
		Write_Dir( 0x90,0x40 );
	}

	Delay10msec();
}


void DrawRoundRect( u16 x1,u16 x2,u16 y1,u16 y2,u16 X,u16 Y,u16 R1,u16 R2,u16 BgColor,u16 FgColor,bool bFill )
{
	Chk_Busy();
	Draw_Line( x1,x2,y1,y2 );

	CmdWrite( 0xA5 );
	DataWrite( X );
	CmdWrite( 0xA6 );
	DataWrite( X >> 8 );

	CmdWrite( 0xA7 );
	DataWrite( Y );
	CmdWrite( 0xA8 );
	DataWrite( Y >> 8 );

	CmdWrite( 0xA1 );
	DataWrite( R1 );
	CmdWrite( 0xA2 );
	DataWrite( R1 >> 8 );

	CmdWrite( 0xA3 );
	DataWrite( R2 );
	CmdWrite( 0xA4 );
	DataWrite( R2 >> 8 );

	Foreground_Color( Blue );

	if ( bFill )
	{
		Write_Dir( 0xA0,0xA0 | 0x40 );
	}
	else
	{
		Write_Dir( 0xA0,0xA0 );
	}

}


void DrawEllipse( u16 X,u16 Y,u16 R1,u16 R2,u16 BgColor,u16 FgColor,bool bFill )
{
	Chk_Busy();
	CmdWrite( 0xA5 );
	DataWrite( X );
	CmdWrite( 0xA6 );
	DataWrite( X >> 8 );

	CmdWrite( 0xA7 );
	DataWrite( Y );
	CmdWrite( 0xA8 );
	DataWrite( Y >> 8 );

	CmdWrite( 0xA1 );
	DataWrite( R1 );
	CmdWrite( 0xA2 );
	DataWrite( R1 >> 8 );

	CmdWrite( 0xA3 );
	DataWrite( R2 );
	CmdWrite( 0xA4 );
	DataWrite( R2 >> 8 );

	Background_color( BgColor );
	Foreground_Color( FgColor );

	if ( bFill )
	{
		Write_Dir( 0xA0,0xC0 );
	}
	else
	{
		Write_Dir( 0xA0,0x80 );
	}
}


void XY_Coordinate( u16 X,u16 Y )
{
	CmdWrite( 0x46 );
	DataWrite( X );
	CmdWrite( 0x47 );
	DataWrite( X >> 8 );

	CmdWrite( 0x48 );
	DataWrite( Y );
	CmdWrite( 0x49 );
	DataWrite( Y >> 8 );
}


/*
  Bit 7    0:Full aligment is disable
  Bit 7    1:Full aligment is enable
  Bit 6    0:With background color
  Bit 6    1:With background Transparency
  Bit 4    0:Normal
  Bit 4    1:90 degree display
  Bit 3-2  00(x1)-11(x4)
  Bit 1-0  00(x1)-11(x4)
*/
void Horizontal_FontEnlarge_x2( void )
{
	Write_Dir( 0x22,0x4F );

	/*
	u8 temp;
	temp = DataRead(0x22);
	printf("temp:%x\r\n",temp);
	printf("cClrD3D2:%x\r\n",cClrD3D2);
	temp &= cClrD3D2 ;
	printf("temp:%x\r\n",temp);
	printf("cClrD3SetD2:%x\r\n",cClrD3SetD2);
	temp |= cClrD3SetD2 ;
	printf("temp:%x\r\n",temp);
	DataWrite(temp);

	temp = DataRead(0x22);
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	DataWrite(temp);
	*/
}


void Vertical_FontEnlarge_x2( void )
{
	/*
	u8 temp;
	temp = DataRead(0x22);
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	DataWrite(temp);
	*/
}


void DrawTri( u16 x1,u16 x2,u16 x3,u16 y1,u16 y2,u16 y3,u16 BgColor,u16 FgColor,bool bFill )
{
	Chk_Busy();
	Draw_Line( x1,x2,y1,y2 );
	Draw_Triangle( x3,y3 );
	Background_color( BgColor );
	Foreground_Color( FgColor );

	if ( bFill )
	{
		Write_Dir( 0x90,0xA1 );
	}
	else
	{
		Write_Dir( 0x90,0x81 );
	}


}


void DrawRect( u16 left,u16 top,u16 right,u16 bottom,u16 BgColor,u16 FgColor,bool bFill )
{
	Chk_Busy();
	Draw_Line( left,right,top,bottom );
	Background_color( BgColor );
	Foreground_Color( FgColor );

	if ( bFill )
	{
		Write_Dir( 0x90,0xB0 );
	}
	else
	{
		Write_Dir( 0x90,0x90 );
	}
}


void DrawLine( u16 x1,u16 x2,u16 y1,u16 y2,u16 BgColor,u16 FgColor )
{
	Chk_Busy();
	Draw_Line( x1,x2,y1,y2 );
	Background_color( BgColor );
	Foreground_Color( FgColor );
	Write_Dir( 0x90,0x80 );
}


void Delay248nsec( void )
{
	__NOP();
}


void Delay510nsec( void )
{
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}


void Delay1usec( void )
{
	u8 i				= 7;

	while ( i )
	{
		__NOP();
		i--;
	}
}


void Delay5usec( void )
{
	u8 i				= 5;
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	while ( i )
	{
		Delay1usec();
		i--;
	}
}


void Delay10usec( void )
{
	u8 i				= 2;

	while ( i )
	{
		Delay5usec();
		i--;
	}
}


void Delay100usec( void )
{
	u8 i				= 10;

	while ( i )
	{
		Delay10usec();
		i--;
	}
}


void Delay1msec( void )
{
	u8 i				= 10;

	while ( i )
	{
		Delay100usec();
		i--;
	}
}


void Delay10msec( void )
{
	u8 i				= 10;

	while ( i )
	{
		Delay1msec();
		i--;
	}
}


void Delay100msec( void )
{
	u8 i				= 10;

	while ( i )
	{
		Delay10msec();
		i--;
	}
}


void Delay1sec( void )
{
	u8 i				= 10;

	while ( i )
	{
		Delay100msec();
		i--;
	}
}


