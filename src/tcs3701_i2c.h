#ifndef __TCS3701_I2C_H__
#define __TCS3701_I2C_H__


#include <linux/i2c.h>





extern int tcs3701_read_reg( const struct i2c_client *client, u8 reg, u8 *value);
extern int tcs3701_write_reg(const struct i2c_client *client, u8 reg, u8 value);
extern int tcs3701_setbits(  const struct i2c_client *client, u8 reg, u8 mask, u8 value);






#endif
