#include "ht32.h"
#include "ht32_board.h"
#include "common/i2c_eeprom.h"
#include "tsc2013.h"

#define ENABLE_DEBUG_UART 1

static void tsc2013_i2c_init( void );
static void tsc2013_reset( void );
static void tsc2013_interrupt_init( void );
static void tsc2013_write_config_values( void );
static void tsc2013_i2c_read_multi_byte( u8 * data,u8 length );
static void tsc2013_i2c_write_byte( u8 value );
static void tsc2013_i2c_write_register( u8 reg_address,u16 value );
static void swap( u8 * high_byte,u8 * low_byte );

u16 tsc2013_touch_status;
tsc2013_data_set data_set;
volatile u16 g_touch_detected = 0;

void tsc2013_init( void )
{
	u8 i;

	// tsc2013_i2c_init();
	printf( "\r\ntsc2013_init( )" );

	tsc2013_reset( );

	// enable_interrupt();

	tsc2013_write_config_values( );

	printf( "\r\ntsc2013_write_config_values( )" );

	//Control Byte ID : 1
	//			C3:C0 : 0
	//			  RM  : 0  (10 Bit)
	//			SWRST : 0
	//			  STS : 0
	//tsc2013_i2c_write_byte(0x80);   //Scan X,Y,Z
	//Control Byte ID : 1
	//			C3:C0 : 0
	//			  RM  : 1  (12 Bit)
	//			SWRST : 0
	//			  STS : 0
	tsc2013_i2c_write_byte( 0x80 ); 	// Scan X,Y,Z

	tsc2013_interrupt_init( );
}

#if 0
void tsc2013_i2c_init( void )
{
	i2c0_init();
}
#endif


void tsc2013_interrupt_init( void )
{
    #if 0
	P2SEL &= ( ~TSC_IRQ );

	P2IE = TSC_IRQ; 								// P2.7 interrupt enabled
	P2IES |= TSC_IRQ;								// P2.7 Hi/lo edge
	P2IFG &= ~TSC_IRQ;								// P2.7 IFG cleared
    #endif

    EXTI_InitTypeDef EXTI_InitStruct = { 0 };
	CKCU_PeripClockConfig_TypeDef CKCUClock = {	{ 0 } };

	CKCUClock.Bit.PD	= 1;
	CKCUClock.Bit.PB	= 1;
	CKCUClock.Bit.PE	= 1;
	CKCUClock.Bit.AFIO	= 1;
	CKCUClock.Bit.EXTI	= 1;
	CKCU_PeripClockConfig( CKCUClock,ENABLE );

	// PA0
	// CTP-INT
    AFIO_GPxConfig( GPIO_PA, AFIO_PIN_0, AFIO_MODE_DEFAULT );

	GPIO_DirectionConfig( HT_GPIOA, GPIO_PIN_0, GPIO_DIR_IN );

	GPIO_PullResistorConfig( HT_GPIOA, GPIO_PIN_0, GPIO_PR_UP );

	GPIO_InputConfig( HT_GPIOA, GPIO_PIN_0, ENABLE );

	AFIO_EXTISourceConfig( AFIO_EXTI_CH_0, AFIO_ESS_PA );

	EXTI_InitStruct.EXTI_Channel = EXTI_CHANNEL_0;

	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;

	EXTI_InitStruct.EXTI_DebounceCnt = 0;

	EXTI_InitStruct.EXTI_IntType = EXTI_BOTH_EDGE;

	EXTI_Init( &EXTI_InitStruct );

	NVIC_EnableIRQ( EXTI0_IRQn );

	EXTI_IntConfig( EXTI_CHANNEL_0, ENABLE );
}

#if 0
// Port 2 interrupt service routine
#pragma vector = PORT2_VECTOR

__interrupt void tsc2013_isr( void )
{
	if ( P2IFG & TSC_IRQ )
	{
		g_touch_detected = 1;
		P2IFG &= ~TSC_IRQ;							// P2.7 IFG cleared
	}
}

#endif

void tsc2013_reset( void )
{
	// 使用硬體IO Reset

	#if 0
	unsigned char i;

	P3DIR |= TSC_RST;								// Set TSC_RST to output direction

	//P3SEL &= (~TSC_RST);
	P3OUT &= ~TSC_RST;								// Set RESET Pin Low

	/*
	for ( i = 0; i < 100; i++ )
	{
		;
	}
	*/

	wait_ms( 5 );


	P3OUT |= TSC_RST;								//Set RESET Pin High after a short delay
	#endif
}


void tsc2013_read_touch_status( void )
{
	tsc2013_register reg;

	tsc2013_i2c_write_byte( REG_STATUS_RD );

	tsc2013_i2c_read_multi_byte( ( u8* ) &reg.value, 2 );

	swap( &reg.bytes.high_byte,&reg.bytes.low_byte );

	tsc2013_touch_status = reg.value;
}


void tsc2013_i2c_write_byte( u8 value )
{
	u16 i = 0xFFFF;

	// Send DevAddr
	AckPolling( EEPROM_I2C, TSC2013_I2C_ADDRESS );

	while ( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TX_EMPTY ) );

	// Sned ReadAddr
	I2C_SendData( EEPROM_I2C, value );

	while( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TX_EMPTY ) );

	I2C_GenerateSTOP( EEPROM_I2C );
}


void tsc2013_i2c_write_register( u8 reg_address, u16 value )
{
	// TODO 需確認Salve ACK Wait 方法

	// Send DevAddr
	AckPolling( EEPROM_I2C, TSC2013_I2C_ADDRESS );

	while ( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TX_EMPTY ) );

	// Sned value hi byte
	I2C_SendData( EEPROM_I2C, reg_address );

	while( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TX_EMPTY ) );

	// Sned value hi byte
	I2C_SendData( EEPROM_I2C, value >> 8 );

	while( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TX_EMPTY ) );

	// Sned value lo byte
	I2C_SendData( EEPROM_I2C, value & 0x00FF );

	while( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TX_EMPTY ) );

	I2C_GenerateSTOP( EEPROM_I2C );

}

void _AckPolling( HT_I2C_TypeDef* I2Cx, u8 Addr, u32 RW  )
{
  u32 reg;

  /* wait if bus busy                                                                                       */
  while( I2C_GetFlagStatus( EEPROM_I2C, I2C_FLAG_BUSBUSY ) );

  while( 1 )
  {
    /* send slave address                                                                                   */
    I2C_TargetAddressConfig( EEPROM_I2C, Addr, RW );

    /* check status                                                                                         */
    while (1)
    {
      reg = EEPROM_I2C->SR;

      if (reg & I2C_FLAG_ADRS)
      {
        return;
      }

      if (reg & I2C_FLAG_RXNACK)
      {
        I2C_ClearFlag( EEPROM_I2C, I2C_FLAG_RXNACK );
        break;
      }
    }
  }
}


void tsc2013_i2c_read_multi_byte( unsigned char *data, unsigned char length )
{
	// i2c0_read_multi_byte( data, length );
	// printf( "\r\nCompelete" );

	Assert_Param( length > 0 );

	// Send DevAddr
	_AckPolling( EEPROM_I2C, TSC2013_I2C_ADDRESS, I2C_MASTER_READ  );

	/* Check on Master Transmitter ADRS condition and clear it                                                */
  	// while ( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TRANSMITTER_MODE ) );

	// while( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_TX_EMPTY ) );

	// while( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_RECEIVER_MODE ) );

	while( I2C_GetFlagStatus( EEPROM_I2C, I2C_FLAG_RXNACK ) );

	// printf( "\r\ntsc2013_i2c_read_multi_byte" );

	if( length > 1 )
	{
		I2C_AckCmd( EEPROM_I2C, ENABLE );
	}

	while( length )
	{
		while( !I2C_CheckStatus( EEPROM_I2C, I2C_MASTER_RX_NOT_EMPTY ) );

		*data = I2C_ReceiveData( EEPROM_I2C );

		data ++;

		length --;

		if( length == 1 )
		{
	  		I2C_AckCmd( EEPROM_I2C, DISABLE );
		}
	}

	I2C_GenerateSTOP( EEPROM_I2C );

}


void tsc2013_read_touch_data( void )
{
	uint8_t i;

	tsc2013_i2c_write_byte( REG_X1_RD );

	tsc2013_i2c_read_multi_byte( data_set.buffer, 16 );

	for ( i = 0; i < 8; i++ )
	{
		swap( &data_set.buffer[i * 2], &data_set.buffer[i * 2 + 1] );
	}
}


void swap( uint8_t * high_byte,uint8_t * low_byte )
{
	uint8_t temp;

	temp = *high_byte;
	*high_byte = *low_byte;
	*low_byte = temp;
}


void tsc2013_write_config_values( void )
{
	tsc2013_register reg;
	u16 value;

	value = CFR0_PSM_1 |
				CFR0_STS_0 |   //Normal Operation
				CFR0_RESOLUTION_10_BIT |
				CFR0_CONVERSION_CLOCK_4MHz |
				//CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_2 | //500us
				CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_3 |	//5ms
				CFR0_PRECHARGE_TIME_4 | 					//1.044ms
				CFR0_SENSE_TIME_3 | 						//608us
				CFR0_DTW_DISABLED |
				CFR0_LSM_DISABLED;


	// Salve Address + Write Bit REG_CFR0_WR =>
	tsc2013_i2c_write_register( REG_CFR0_WR, value );

	tsc2013_i2c_write_byte( REG_CFR0_RD );
	tsc2013_i2c_read_multi_byte( ( unsigned char* )&reg.value, 2 );
	swap( &reg.bytes.high_byte, &reg.bytes.low_byte );

	if ( reg.value != value )
	{
		printf( "\r\nREG_CFR0_RD" );
		printf( "\r\nread - %x, expect - %x", reg.value, value );
	}

	value = CFR1_BTD_1;

	tsc2013_i2c_write_register( REG_CFR1_WR, value );

	tsc2013_i2c_write_byte( REG_CFR1_RD );

    tsc2013_i2c_read_multi_byte( ( u8* ) &reg.value, 2 );

	swap( &reg.bytes.high_byte, &reg.bytes.low_byte );

	if ( reg.value != value )
	{
		printf( "\r\nREG_CFR1_RD" );
		printf( "\r\nread-%x,expect-%x", reg.value, value );
	}

	value = CFR2_PINTS_0 |	///DAV
				//CFR2_M_1 |	 //M = 3
				//CFR2_W_0 |	 //W = 1
				CFR2_M_3 |	   //M
				CFR2_W_2 |	   //W
				CFR2_MAVE_X_ENABLED |
				CFR2_MAVE_Y_ENABLED |
				CFR2_MAVE_Z_ENABLED |
				CFR2_MAVE_AUX_DISABLED;


	tsc2013_i2c_write_register( REG_CFR2_WR, value );

	tsc2013_i2c_write_byte( REG_CFR2_RD );

	tsc2013_i2c_read_multi_byte( ( u8* )&reg.value, 2 );

	swap( &reg.bytes.high_byte, &reg.bytes.low_byte );


	if ( reg.value != value )
	{
		printf( "\r\nREG_CFR2_RD" );
		printf( "\r\nread-%x,expect-%x", reg.value, value );
	}


}


