#include "tcs3701_i2c.h"
#include <linux/i2c.h>



int tcs3701_read_reg(const struct i2c_client *client, u8 reg, u8 *value)
{
    int ret;

    ret = i2c_smbus_read_byte_data(client, reg);
    if (0 > ret)
        return ret;
    *value = ret & 0xff;

    return 0;
}


int tcs3701_write_reg(const struct i2c_client *client, u8 reg, u8 value)
{
    return i2c_smbus_write_byte_data(client, reg, value);
}

int tcs3701_setbits(const struct i2c_client *client, u8 reg, u8 mask, u8 value)
{
    u8 read = 0;

    if (0xFF == mask)
        return tcs3701_write_reg(client, reg, value);

    if (tcs3701_read_reg(client, reg, &read))
        return -1;

    read &= ~mask;
    read |= value & mask;
    return tcs3701_write_reg(client, reg, read);
}
