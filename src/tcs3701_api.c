#include "tcs3701_api.h"
#include "tcs3701_i2c.h"
#include <linux/delay.h>





#if 1
#define DEBUG(fmt, arg...)  printk("--TCS3701-- " fmt "\n", ##arg)
#else
#define DEBUG(fmt, arg...)
#endif



/* 默认配置 */
const u8 default_config[][2] =
{
  //{ TCS3701_REG_ATIME,        0                                             },
    { TCS3701_REG_PTIME,        PTIME_MS(50)                                  }, //adc采样周期
  //{ TCS3701_REG_WTIME,        0                                             },
    { TCS3701_REG_CFG3,         CFG3_RVED | HXTALK_MODE1                      },
  //{ TCS3701_REG_CFG8,         CONCURRENT_PROX_AND_ALS                       },
  //{ TCS3701_REG_CFG11,        PINT_DIRECT                                   },
  //{ TCS3701_REG_CFG14,        0                                             },
    { TCS3701_REG_PERS,         ALS_PERSIST(0) | PROX_PERSIST(2)              }, //硬件滤波次数
    { TCS3701_REG_PCFG1,        HXTALK_MODE2                                  },
    { TCS3701_REG_PCFG2,        PLDRIVE_MA(250)                               }, //led 电流
    { TCS3701_REG_PCFG4,        PGAIN_8X                                      }, // 接收增益
    { TCS3701_REG_PCFG5,        PPULSES(64) | PPULSE_LEN_32US                 }, //脉冲宽度     个数最大64
    { TCS3701_REG_CALIBCFG0,    DCAVG_AUTO_OFFSET_ADJUST | 4                  },
  //{ TCS3701_REG_CALIBCFG1,    PROX_AUTO_OFFSET_ADJUST | PXAVG_AUTO_BSLN | 4 },
    { TCS3701_REG_CALIBCFG2,    BINSRCH_TARGET_127                            }, //校准目标
    { TCS3701_REG_INTENAB,      0x00                                          },
    { TCS3701_REG_PILT0L,       170                                           },
    { TCS3701_REG_PILT0H,       0x00                                          },
    { TCS3701_REG_PIHT0L,       00                                            },
    { TCS3701_REG_PIHT0H,       0x01                                          }
};



/* 内部光敏的固定配置，手册中无说明 */
static u8 smux_data[5][20] =
{
    {       /* 2 channel pos-neg */
            0x12, 0x10, 0x21, 0x21,
            0x11, 0x20, 0x12, 0x22,
            0x01, 0x21, 0x20, 0x12,
            0x12, 0x22, 0x21, 0x12,
            0x11, 0x02, 0x00, 0x00
    },
    {       /* 4 channel CRBGW */
            0x14, 0x20, 0x23, 0x41,
            0x33, 0x12, 0x14, 0x24,
            0x03, 0x23, 0x10, 0x14,
            0x32, 0x44, 0x21, 0x23,
            0x13, 0x04, 0x00, 0x67
    },
    {       /* 4 channel CRGB */
            0x14, 0x20, 0x23, 0x41,
            0x33, 0x12, 0x14, 0x24,
            0x03, 0x23, 0x10, 0x14,
            0x32, 0x44, 0x21, 0x23,
            0x13, 0x04, 0x00, 0x00
    },
    {        /* 4 channel */
              // C, R, G, B
            0x14, 0x20, 0x23, 0x41,
            0x33, 0x12, 0x14, 0x24,
            0x03, 0x23, 0x10, 0x14,
            0x32, 0x44, 0x21, 0x23,
            0x13, 0x04, 0x00, 0x76
    },
    {       /* 2 channel clear/green/windband */
            0x10, 0x02, 0x01, 0x01,
            0x11, 0x10, 0x10, 0x00,
            0x21, 0x00, 0x12, 0x10,
            0x10, 0x00, 0x01, 0x01,
            0x11, 0x20, 0x00, 0x76
    }
};



int tcs3701_set_enable(const struct i2c_client *client, u8 mask, u8 val)
{
    return tcs3701_setbits(client, TCS3701_REG_ENABLE, mask, val);
}

int tcs3701_set_intenab(const struct i2c_client *client, u8 mask, u8 val)
{
    return tcs3701_setbits(client, TCS3701_REG_INTENAB, mask, val);
}

static int clear_fifo(const struct i2c_client *client)
{
    return tcs3701_write_reg(client, TCS3701_REG_CONTROL, 0x02);
}

int tcs3701_check_id(const struct i2c_client *client)
{
    u8 id;

    if (tcs3701_read_reg(client, TCS3701_REG_ID, &id) || (TCS3701_DEVICE_ID != id))
        return -1;
    return 0;
}

/* 配置内部的光敏，手册中无说明 */
static int set_smux_config(const struct i2c_client *client, u8 als_ch)
{
    u8 i;
    int ret = 0;

    ret |= tcs3701_write_reg(client, 0xAF, 0x10); /* Write command */

    for (i = 0; i < sizeof(smux_data[0]); i++)
        ret |= tcs3701_write_reg(client, i, smux_data[als_ch - 1][i]);

    ret |= tcs3701_write_reg(client, TCS3701_REG_ENABLE, (0x10|PON)); /* Execute the command */

    udelay(500);

    ret |= tcs3701_write_reg(client, 0xAF, 0x00); /* Clear command */

    /* disable unused channels & fifo map */
    if (2 == als_ch)
    {
        ret |= tcs3701_write_reg(client, 0xD5, 0x3C);
        ret |= tcs3701_write_reg(client, 0xFC, 0x06);
    }
    else if (3 == als_ch)
    {
        ret |= tcs3701_write_reg(client, 0xD5, 0x38);
        ret |= tcs3701_write_reg(client, 0xFC, 0x0E);
    }
    else if (4 == als_ch)
    {
        ret |= tcs3701_write_reg(client, 0xD5, 0x30);
        ret |= tcs3701_write_reg(client, 0xFC, 0x1E);
    }

    ret |= clear_fifo(client); /* flush fifo */

    if (ret)
        return -1;
    return 0;
}

int tcs3701_set_regs(const struct i2c_client *client, const u8 regs[][2], int n)
{
    int i;
    int ret = 0;

    for (i = 0; i < n; i++)
        ret |= tcs3701_write_reg(client, regs[i][0], regs[i][1]);

    if (ret)
        return -1;
    return 0;
}

int tcs3701_reset(const struct i2c_client *client)
{
    int ret = 0;

    ret |= tcs3701_write_reg(client, TCS3701_REG_ENABLE, 0);

    /* Enable PON bit */
    ret |= tcs3701_write_reg(client, TCS3701_REG_ENABLE, PON);
    ret |= set_smux_config(client, 4);

    if (ret)
        return -1;
    return 0;
}

int tcs3701_poweroff(const struct i2c_client *client)
{
    return tcs3701_write_reg(client, TCS3701_REG_ENABLE, 0);
}

/* 设置接近检测的防抖次数 */
int tcs3701_set_prox_pers(struct i2c_client *client, u8 pers)
{
    return tcs3701_setbits(client, TCS3701_REG_PERS, PPERS_MASK, PROX_PERSIST(pers));
}

int tcs3701_set_prox_threshold_0(const struct i2c_client *client, u16 high, u16 low)
{
    int ret = 0;

    ret |= tcs3701_write_reg(client, TCS3701_REG_PILT0L, (u8)(low & 0xFF));
    ret |= tcs3701_write_reg(client, TCS3701_REG_PILT0H, (u8)((low >> 8) & 0xFF));
    ret |= tcs3701_write_reg(client, TCS3701_REG_PIHT0L, (u8)(high & 0xFF));
    ret |= tcs3701_write_reg(client, TCS3701_REG_PIHT0H, (u8)((high >> 8) & 0xFF));

    if (ret)
        return -1;
    return 0;
}

int tcs3701_set_prox_threshold_1(const struct i2c_client *client, u16 high, u16 low)
{
    int ret = 0;

    ret |= tcs3701_write_reg(client, TCS3701_REG_PILT1L, (u8)(low & 0xFF));
    ret |= tcs3701_write_reg(client, TCS3701_REG_PILT1H, (u8)((low >> 8) & 0xFF));
    ret |= tcs3701_write_reg(client, TCS3701_REG_PIHT1L, (u8)(high & 0xFF));
    ret |= tcs3701_write_reg(client, TCS3701_REG_PIHT1H, (u8)((high >> 8) & 0xFF));

    if (ret)
        return -1;
    return 0;
}

int tcs3701_start_calibration(const struct i2c_client *client)
{
    DEBUG("start proximity calibration");

    if (tcs3701_write_reg(client, TCS3701_REG_STATUS, 0xFF))
        return -1;
    return tcs3701_write_reg(client, TCS3701_REG_CALIB, START_OFFSET_CALIB);
}

/* 使能接近检测功能 */
int tcs3701_prox_enable(const struct i2c_client *client)
{
    int ret = 0;

    /* 开始校准，校准完成后在中断处理中打开接近检测功能 */
    ret |= tcs3701_set_intenab(client, CIEN, CIEN);
    ret |= tcs3701_set_enable(client, PEN | PON, PEN | PON);
    ret |= tcs3701_start_calibration(client);

    if (ret)
        return -1;
    return 0;
}
int tcs3701_prox_disable(const struct i2c_client *client)
{
    tcs3701_set_intenab(client, CIEN, 0);
    tcs3701_set_enable(client, PEN, 0);

    return 0;
}

int tcs3701_device_init(const struct i2c_client *client)
{
    if (tcs3701_reset(client))
        return -1;
    return tcs3701_set_regs(client, default_config, sizeof(default_config) / sizeof(default_config[0]));
}


/* 中断或者轮询处理中断事件，返回值 0:离开 1:接近 -1:错误*/
int tcs3701_routine(const struct i2c_client *client)
{
    u8  status, status4;
    u8  pdatal, pdatah;
    u8  cstat;
    u16 pdata;


    tcs3701_read_reg(client, TCS3701_REG_STATUS,    &status);
    tcs3701_read_reg(client, TCS3701_REG_STATUS4,   &status4);
    tcs3701_read_reg(client, TCS3701_REG_PDATAL,    &pdatal);
    tcs3701_read_reg(client, TCS3701_REG_PDATAH,    &pdatah);
    tcs3701_read_reg(client, TCS3701_REG_CALIBSTAT, &cstat);
    pdata = (pdatah << 8) | pdatal;
    status4 &= 0x0f;

    if (status4 & PINT0_PILT)
        DEBUG("status:0x%02x status4:0x%x cstat:0x%02x PDATA:%3u    leave", status, status4, cstat, pdata);
    else if (status4 & PINT0_PIHT)
        DEBUG("status:0x%02x status4:0x%x cstat:0x%02x PDATA:%3u  come", status, status4, cstat, pdata);
    else
        DEBUG("status:0x%02x status4:0x%x cstat:0x%02x PDATA:%3u", status, status4, cstat, pdata);

    /* 清 STATUS 寄存器，同时也会清掉 STATUS4、CALIBSTAT 等 */
    if (status)
        tcs3701_write_reg(client, TCS3701_REG_STATUS, status);
    else
        return -1;

    if (status & PINT0)
    {
    }
    if (cstat & CALIB_FINISHED)
    {
        DEBUG("Calibration ok");
        tcs3701_set_intenab(client, PIEN0, PIEN0);
    }

    /* 之前校准时有遮挡，重新校准 */
    if ((0 == pdata) || (cstat & OFFSET_ADJUSTED))
        tcs3701_start_calibration(client);


    if (status4 & PINT0_PILT)
        return 0;
    else if (status4 & PINT0_PIHT)
        return 1;
    else
        return -1;
}
