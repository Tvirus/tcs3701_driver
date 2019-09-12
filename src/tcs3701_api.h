#ifndef __TCS3701_API_H__
#define __TCS3701_API_H__


#include <linux/i2c.h>





/* 配置寄存器时要先关闭相应的als或者接近检测功能 */
extern int tcs3701_check_id(const struct i2c_client *client);
extern int tcs3701_reset(const struct i2c_client *client);
extern int tcs3701_set_regs(const struct i2c_client *client, const u8 regs[][2], int n);
extern int tcs3701_set_prox_threshold_0(const struct i2c_client *client, u16 high, u16 low);
extern int tcs3701_set_prox_threshold_1(const struct i2c_client *client, u16 high, u16 low);
extern int tcs3701_device_init(const struct i2c_client *client);
extern int tcs3701_prox_enable(const struct i2c_client *client);
extern int tcs3701_prox_disable(const struct i2c_client *client);
extern int tcs3701_routine(const struct i2c_client *client);
extern int tcs3701_poweroff(const struct i2c_client *client);




#define TCS3701_DEVICE_ID              0x18




/* Register map */
#define TCS3701_REG_ENABLE             0x80
#define TCS3701_REG_ATIME              0x81
#define TCS3701_REG_PTIME              0x82
#define TCS3701_REG_WTIME              0x83
#define TCS3701_REG_AILTL              0x84
#define TCS3701_REG_AILTH              0x85
#define TCS3701_REG_AIHTL              0x86
#define TCS3701_REG_AIHTH              0x87
#define TCS3701_REG_PILT0L             0x88
#define TCS3701_REG_PILT0H             0x89
#define TCS3701_REG_PILT1L             0x8A
#define TCS3701_REG_PILT1H             0x8B
#define TCS3701_REG_PIHT0L             0x8C
#define TCS3701_REG_PIHT0H             0x8D
#define TCS3701_REG_PIHT1L             0x8E
#define TCS3701_REG_PIHT1H             0x8F
#define TCS3701_REG_AUXID              0x90
#define TCS3701_REG_REVID              0x91
#define TCS3701_REG_ID                 0x92
#define TCS3701_REG_STATUS             0x93
#define TCS3701_REG_ASTATUS            0x94
#define TCS3701_REG_ADATA0L            0x95
#define TCS3701_REG_ADATA0H            0x96
#define TCS3701_REG_ADATA1L            0x97
#define TCS3701_REG_ADATA1H            0x98
#define TCS3701_REG_ADATA2L            0x99
#define TCS3701_REG_ADATA2H            0x9A
#define TCS3701_REG_ADATA3L            0x9B
#define TCS3701_REG_ADATA3H            0x9C
#define TCS3701_REG_ADATA4L            0x9D
#define TCS3701_REG_ADATA4H            0x9E
#define TCS3701_REG_ADATA5L            0x9F
#define TCS3701_REG_ADATA5H            0xA0
#define TCS3701_REG_PDATAL             0xA1
#define TCS3701_REG_PDATAH             0xA2
#define TCS3701_REG_STATUS2            0xA3
#define TCS3701_REG_STATUS3            0xA4
#define TCS3701_REG_STATUS4            0xA5
#define TCS3701_REG_STATUS5            0xA6
#define TCS3701_REG_STATUS6            0xA7
#define TCS3701_REG_CFG0               0xA9
#define TCS3701_REG_CFG1               0xAA
#define TCS3701_REG_CFG3               0xAC
#define TCS3701_REG_CFG4               0xAD
#define TCS3701_REG_CFG8               0xB1
#define TCS3701_REG_CFG10              0xB3
#define TCS3701_REG_CFG11              0xB4
#define TCS3701_REG_CFG12              0xB5
#define TCS3701_REG_CFG14              0xB7
#define TCS3701_REG_PCFG1              0xB8
#define TCS3701_REG_PCFG2              0xB9
#define TCS3701_REG_PCFG4              0xBB
#define TCS3701_REG_PCFG5              0xBC
#define TCS3701_REG_PERS               0xBD
#define TCS3701_REG_GPIO               0xBE
#define TCS3701_REG_POFFSETL           0xC7
#define TCS3701_REG_POFFSETH           0xC8
#define TCS3701_REG_ASTEPL             0xCA
#define TCS3701_REG_ASTEPH             0xCB
#define TCS3701_REG_AGC_GAIN_MAX       0xCF
#define TCS3701_REG_PXAVGL             0xD0
#define TCS3701_REG_PXAVGH             0xD1
#define TCS3701_REG_PBSLNL             0xD2
#define TCS3701_REG_PBSLNH             0xD3
#define TCS3701_REG_AZ_CONFIG          0xD6
#define TCS3701_REG_CALIB              0xEA
#define TCS3701_REG_CALIBCFG0          0xEB
#define TCS3701_REG_CALIBCFG1          0xEC
#define TCS3701_REG_CALIBCFG2          0xED
#define TCS3701_REG_CALIBSTAT          0xEE
#define TCS3701_REG_INTENAB            0xF9
#define TCS3701_REG_CONTROL            0xFA
#define TCS3701_REG_RVED1              0xFC
#define TCS3701_REG_RVED2              0xFD
#define TCS3701_REG_RVED3              0xFE
#define TCS3701_REG_RVED4              0xFF

/* Register bits map */
//ENABLE @ 0x80
#define PON                             (0x01 << 0)
#define AEN                             (0x01 << 1)
#define PEN                             (0x01 << 2)
#define WEN                             (0x01 << 3)


//STATUS @ 0x93
#define SINT                            (0x01 << 0)
#define CINT                            (0x01 << 1)
#define AINT                            (0x01 << 3)
#define PINT0                           (0x01 << 4)
#define PINT1                           (0x01 << 5)
#define PSAT                            (0x01 << 6)
#define ASAT                            (0x01 << 7)

//ASTATUS @0x94
#define AGAIN_STATUS_SHIFT               0
#define AGAIN_STATUS_MASK                0x0F
#define ASAT_STATUS                     (0x01 << 7)

//STATUS2 @0xA3
#define ASAT_ANALOG                     (0x01 << 3)
#define ASAT_DIGITAL                    (0x01 << 4)
#define PVALID                          (0x01 << 5)
#define AVALID                          (0x01 << 6)

//STATUS3 @0xA4
#define PSAT_AMBIENT                    (0x01 << 0)
#define PSAT_REFLECTIVE                 (0x01 << 1)
#define PSAT_ADC                        (0x01 << 2)
#define STATUS3_RVED                    (0x01 << 3)
#define AINT_AILT                       (0x01 << 4)
#define AINT_AIHT                       (0x01 << 5)

//STATUS4 @0xA5
#define PINT0_PILT                      (0x01 << 0)
#define PINT0_PIHT                      (0x01 << 1)
#define PINT1_PILT                      (0x01 << 2)
#define PINT1_PIHT                      (0x01 << 3)

//STATUS6 @0xA7
#define INIT_BUSY                       (0x01 << 0)
#define SAI_ACTIVE                      (0x01 << 1)
#define ALS_TRIGGER_ERROR               (0x01 << 2)
#define PROX_TRIGGER_ERROR              (0x01 << 3)
#define OVTEMP_DETECTED                 (0x01 << 5)

//CFG0 @0xA9
#define ALS_TRIGGER_LONG                (0x01 << 2)
#define PROX_TRIGGER_LONG               (0x01 << 3)
#define LOWPOWER_IDLE                   (0x01 << 5)

//CFG1 @0xAA
#define AGAIN_MASK                       0x1F
#define AGAIN_0_5X                       0x00
#define AGAIN_1X                         0x01
#define AGAIN_2X                         0x02
#define AGAIN_4X                         0x03
#define AGAIN_8X                         0x04
#define AGAIN_16X                        0x05
#define AGAIN_32X                        0x06
#define AGAIN_64X                        0x07
#define AGAIN_128X                       0x08
#define AGAIN_256X                       0x09
#define AGAIN_512X                       0x0A
#define AGAIN_1024X                      0x0B
#define AGAIN_2048X                      0x0C

//CFG3 @0xAC
#define CFG3_RVED                       (0x0C << 0)
#define SAI                             (0x01 << 4)
#define HXTALK_MODE1                    (0x01 << 5)

//CFG4 @0xAD
#define GPIO_PINMAP_DEFAULT             (0x00 << 0)
#define GPIO_PINMAP_RVED                (0x01 << 0)
#define GPIO_PINMAP_AINT                (0x02 << 0)
#define GPIO_PINMAP_PINT0               (0x03 << 0)
#define GPIO_PINMAP_PINT1               (0x04 << 0)
#define GPIO_PINMAP_MASK                (0x07 << 0)
#define INT_INVERT                      (0x01 << 3)
#define INT_PINMAP_NORMAL               (0x00 << 4)
#define INT_PINMAP_RVED                 (0x01 << 4)
#define INT_PINMAP_AINT                 (0x02 << 4)
#define INT_PINMAP_PINT0                (0x03 << 4)
#define INT_PINMAP_PINT1                (0x04 << 4)
#define INT_PINMAP_MASK                 (0x07 << 4)

//CFG8_REG @0xB1
#define SWAP_PROX_ALS5                  (0x01 << 0)
#define ALS_AGC_ENABLE                  (0x01 << 2)
#define CONCURRENT_PROX_AND_ALS         (0x01 << 4)

//CFG10_REG @0xB3
#define ALS_AGC_LOW_HYST_MASK           (0x03 << 4)
#define ALS_AGC_LOW_HYST_12_5           (0x00 << 4)
#define ALS_AGC_LOW_HYST_25             (0x01 << 4)
#define ALS_AGC_LOW_HYST_37_5           (0x02 << 4)
#define ALS_AGC_LOW_HYST_50             (0x03 << 4)
#define ALS_AGC_HIGH_HYST_MASK          (0x03 << 6)
#define ALS_AGC_HIGH_HYST_50            (0x00 << 6)
#define ALS_AGC_HIGH_HYST_62_5          (0x01 << 6)
#define ALS_AGC_HIGH_HYST_75            (0x02 << 6)
#define ALS_AGC_HIGH_HYST_87_5          (0x03 << 6)

//CFG11_REG @0xB4
#define PINT_DIRECT                     (0x01 << 6)
#define AINT_DIRECT                     (0x01 << 7)

//CFG12_REG @0xB5
#define ALS_TH_CHANNEL_MASK              0x07
#define ALS_TH_CHANNEL_0                 0x00
#define ALS_TH_CHANNEL_1                 0x01
#define ALS_TH_CHANNEL_2                 0x02
#define ALS_TH_CHANNEL_3                 0x03
#define ALS_TH_CHANNEL_4                 0x04

//CFG14_REG @0xB7
#define PROX_OFFSET_COARSE_MASK         (0x1F << 0)
#define EN_PROX_OFFSET_RANGE            (0x01 << 5)
#define AUTO_CO_CAL_EN                  (0x01 << 6)
#define PPULSE_LEN16X                   (0x01 << 7) // 把ppulse配置的长度变为16倍，手册中没有描述

//PCFG1_REG @0xB8
#define PROX_FILTER_MASK                (0x03 << 0)
#define PROX_FILTER_1                   (0x00 << 0)
#define PROX_FILTER_2                   (0x01 << 0)
#define PROX_FILTER_4                   (0x02 << 0)
#define PROX_FILTER_8                   (0x03 << 0)
#define PROX_FILTER_DOWNSAMPLE          (0x01 << 2)
#define PCFG1_RVED                      (0x01 << 3)
#define HXTALK_MODE2                    (0x01 << 7)

//PCFG2_REG @0xB9
#define PLDRIVE0_SHIFT                   0
#define PLDRIVE0_MASK                    0x7F

//PCFG4_REG @0xBB
#define PGAIN_MASK                       0x03
#define PGAIN_1X                         0x00
#define PGAIN_2X                         0x01
#define PGAIN_4X                         0x02
#define PGAIN_8X                         0x03

//PCFG5_REG @0xBC
#define PPULSE_SHIFT                     0
#define PPULSE_MASK                      0x3F
#define PPULSE_LEN_MASK                 (0x03 << 6)
#define PPULSE_LEN_4US                  (0x00 << 6)
#define PPULSE_LEN_8US                  (0x01 << 6)
#define PPULSE_LEN_16US                 (0x02 << 6)
#define PPULSE_LEN_32US                 (0x03 << 6)

//PERS_REG @0xBD
#define APERS_SHIFT                      0
#define APERS_MASK                       0x0F
#define PPERS_SHIFT                      4
#define PPERS_MASK                      (0x0F << PPERS_SHIFT)

//GPIO_REG @0xBE
#define GPIO_IN                         (0x01 << 0)
#define GPIO_OUT                        (0x01 << 1)
#define GPIO_IN_EN                      (0x01 << 2)
#define GPIO_INVERT                     (0x01 << 3)

//GAIN_MAX_REG @0xCF
#define AGC_AGAIN_MAX_SHIFT              0
#define AGC_AGAIN_MAX_MASK               0x0F  //2^(AGC_AGAIN_MAX)

//CALIB_REG @0xEA
#define START_OFFSET_CALIB               0x01

//CALIBCFG0_REG @0xEB
#define DCAVG_ITERATIONS_SHIFT           0
#define DCAVG_ITERATIONS_MASK            0x07
#define BINSRCH_SKIP                    (0x01 << 3)
#define DCAVG_AUTO_OFFSET_ADJUST        (0x01 << 6)
#define DCAVG_AUTO_BSLN                 (0x01 << 7)

//CALIBCFG1_REG @0xEC
#define PXAVG_ITERATIONS_SHIFT           0
#define PXAVG_ITERATIONS_MASK            0x07
#define PXAVG_AUTO_BSLN                 (0x01 << 3)
#define PROX_AUTO_OFFSET_ADJUST         (0x01 << 6)

//CALIBCFG1_REG @0xED
#define BINSRCH_TARGET_MASK             (0x07 << 5)
#define BINSRCH_TARGET_3                (0x00 << 5)
#define BINSRCH_TARGET_7                (0x01 << 5)
#define BINSRCH_TARGET_15               (0x02 << 5)
#define BINSRCH_TARGET_31               (0x03 << 5)
#define BINSRCH_TARGET_63               (0x04 << 5)
#define BINSRCH_TARGET_127              (0x05 << 5)
#define BINSRCH_TARGET_255              (0x06 << 5)
#define BINSRCH_TARGET_511              (0x07 << 5)

//CALIBSTAT_REG @0xEE
#define CALIB_FINISHED                  (0x01 << 0)
#define OFFSET_ADJUSTED                 (0x01 << 1)
#define BASELINE_ADJUSTED               (0x01 << 2)

//INTENAB_REG @0xF9
#define SIEN                            (0x01 << 0)
#define CIEN                            (0x01 << 1)
#define AIEN                            (0x01 << 3)
#define PIEN0                           (0x01 << 4)
#define PIEN1                           (0x01 << 5)
#define PSIEN                           (0x01 << 6)
#define ASIEN                           (0x01 << 7)

//CONTROL_REG @0xFA
#define CLEAR_SAI_ACTIVE                (0x01 << 0)
#define ALS_MANUAL_AZ                   (0x01 << 2)

//Configration calculations
#define ASTEP_US(us)                    ((u16)((((u32)us)*100 + (278 >> 1)) / 278 - 1))
#define PTIME_MS(ms)                    ( (u8)((((u32)ms)*100 + (278 >> 1)) / 278 - 1))
#define WTIME_MS(ms)                    ( (u8)((((u32)ms)*100 + (278 >> 1)) / 278 - 1))
#define PLDRIVE_MA(ma)                  ((u8)((6 <= ma)?(((ma-4) >> 1) << PLDRIVE0_SHIFT): 0))
#define PPULSES(c)                      ((u8)((1 <= c)?((c - 1) << PPULSE_SHIFT): 0))
#define ALS_PERSIST(p)                  ((u8)((p) & APERS_MASK))
#define PROX_PERSIST(p)                 ((u8)(((p) & 0x0F) << PPERS_SHIFT))




#endif
