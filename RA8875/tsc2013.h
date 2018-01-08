#ifndef TSC2013_H
#define TSC2013_H

#define TSC2013_I2C_ADDRESS                            0x48

#define TSC_IRQ                                        BIT7
#define TSC_RST                                        BIT0

#define REG_CFR0_WR                                    ((0x0c << 3) | 0x02)
#define REG_CFR1_WR                                    ((0x0d << 3) | 0x02)
#define REG_CFR2_WR                                    ((0x0e << 3) | 0x02)
#define REG_CFN_WR                                     ((0x0f << 3) | 0x02)

#define REG_X1_RD                                      ((0x00 << 3) | 0x03)
#define REG_STATUS_RD                                  ((0x08 << 3) | 0x03)
#define REG_CFR0_RD                                    ((0x0c << 3) | 0x03)
#define REG_CFR1_RD                                    ((0x0d << 3) | 0x03)
#define REG_CFR2_RD                                    ((0x0e << 3) | 0x03)
#define REG_CFN_RD                                     ((0x0f << 3) | 0x03)

#define CFR0_PSM_0                                     (0 << 15)
#define CFR0_PSM_1                                     (1 << 15)

#define CFR0_STS_0                                     (0 << 14)
#define CFR0_STS_1                                     (1 << 14)

#define CFR0_RESOLUTION_10_BIT                         (0 << 13)
#define CFR0_RESOLUTION_12_BIT                         (1 << 13)

#define CFR0_CONVERSION_CLOCK_4MHz                     (0 << 11)
#define CFR0_CONVERSION_CLOCK_2MHz                     (1 << 11)
#define CFR0_CONVERSION_CLOCK_1MHz                     (2 << 11)

#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_0        (0 << 8)
#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_1        (1 << 8)
#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_2        (2 << 8)
#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_3        (3 << 8)
#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_4        (4 << 8)
#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_5        (5 << 8)
#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_6        (6 << 8)
#define CFR0_PANEL_VOLTAGE_STABILIZATION_TIME_7        (7 << 8)

#define CFR0_PRECHARGE_TIME_0                          (0 << 5)
#define CFR0_PRECHARGE_TIME_1                          (1 << 5)
#define CFR0_PRECHARGE_TIME_2                          (2 << 5)
#define CFR0_PRECHARGE_TIME_3                          (3 << 5)
#define CFR0_PRECHARGE_TIME_4                          (4 << 5)
#define CFR0_PRECHARGE_TIME_5                          (5 << 5)
#define CFR0_PRECHARGE_TIME_6                          (6 << 5)
#define CFR0_PRECHARGE_TIME_7                          (7 << 5)

#define CFR0_SENSE_TIME_0                              (0 << 2)
#define CFR0_SENSE_TIME_1                              (1 << 2)
#define CFR0_SENSE_TIME_2                              (2 << 2)
#define CFR0_SENSE_TIME_3                              (3 << 2)
#define CFR0_SENSE_TIME_4                              (4 << 2)
#define CFR0_SENSE_TIME_5                              (5 << 2)
#define CFR0_SENSE_TIME_6                              (6 << 2)
#define CFR0_SENSE_TIME_7                              (7 << 2)

#define CFR0_DTW_ENABLED                               (1 << 1)
#define CFR0_DTW_DISABLED                              (0 << 1)

#define CFR0_LSM_ENABLED                               1
#define CFR0_LSM_DISABLED                              0

#define CFR1_BTD_0                                     0
#define CFR1_BTD_1                                     1
#define CFR1_BTD_2                                     2
#define CFR1_BTD_3                                     3
#define CFR1_BTD_4                                     4
#define CFR1_BTD_5                                     5
#define CFR1_BTD_6                                     6
#define CFR1_BTD_7                                     7

#define CFR2_PINTS_0                                   (0 << 14)
#define CFR2_PINTS_1                                   (1 << 14)
#define CFR2_PINTS_2                                   (2 << 14)
#define CFR2_PINTS_3                                   (3 << 14)

#define CFR2_M_0                                       (0 << 12)
#define CFR2_M_1                                       (1 << 12)
#define CFR2_M_2                                       (2 << 12)
#define CFR2_M_3                                       (3 << 12)

#define CFR2_W_0                                       (0 << 10)
#define CFR2_W_1                                       (1 << 10)
#define CFR2_W_2                                       (2 << 10)
#define CFR2_W_3                                       (3 << 10)

#define CFR2_MAVE_X_ENABLED                            (1 << 4)
#define CFR2_MAVE_X_DISABLED                           (0 << 4)

#define CFR2_MAVE_Y_ENABLED                            (1 << 3)
#define CFR2_MAVE_Y_DISABLED                           (0 << 3)

#define CFR2_MAVE_Z_ENABLED                            (1 << 2)
#define CFR2_MAVE_Z_DISABLED                           (0 << 2)

#define CFR2_MAVE_AUX_ENABLED                          (1 << 1)
#define CFR2_MAVE_AUX_DISABLED                         (0 << 1)

#define TSC2013_STATUS_DAVX                            (1 << 15)
#define TSC2013_STATUS_DAVY                            (1 << 14)
#define TSC2013_STATUS_DAVZ1                           (1 << 13)
#define TSC2013_STATUS_DAVZ2                           (1 << 12)

typedef union
{
   unsigned short value;

   struct
   {
      unsigned char low_byte;
      unsigned char high_byte;

   }bytes;

}tsc2013_register;

typedef union
{
  unsigned char buffer[ 16 ];

	struct
	{
		unsigned short X1;
		unsigned short X2;
		unsigned short Y1;
		unsigned short Y2;
		unsigned short IX;
		unsigned short IY;
		unsigned short Z1;
		unsigned short Z2;
		unsigned short AUX;
	}values;

}tsc2013_data_set;

extern unsigned short tsc2013_touch_status;;
extern volatile unsigned short g_touch_detected;
extern tsc2013_data_set data_set;

extern void tsc2013_init( void );
extern void tsc2013_read_touch_status( void );
extern void tsc2013_read_touch_data( void );

#endif
