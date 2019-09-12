#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/of_gpio.h>


#include "tcs3701_api.h"


#define VERSION "1.0"


#if 1
#define DEBUG(fmt, arg...)  printk("--TCS3701-- " fmt "\n", ##arg)
#else
#define DEBUG(fmt, arg...)
#endif


#define ERROR(fmt, arg...)  printk(KERN_ERR "--TCS3701-- " fmt "\n", ##arg)



typedef struct
{
    struct i2c_client *client;
    struct input_dev *idev;
    int    gpio_irq;
}tcs3701_data_t;




static int tcs3701_idev_open(struct input_dev *idev)
{
    return 0;
}
static void tcs3701_idev_close(struct input_dev *idev)
{
}

irqreturn_t tcs3701_irq_handler(int irq, void *data)
{
    tcs3701_data_t *tcs3701_data = (tcs3701_data_t*)data;
    int event;

    event = tcs3701_routine(tcs3701_data->client);

    if ((0 == event) || (1 == event))
    {
        input_report_switch(tcs3701_data->idev, SW_FRONT_PROXIMITY, event);
        input_sync(tcs3701_data->idev);
    }

    return IRQ_HANDLED;
}

static int tcs3701_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    tcs3701_data_t *tcs3701_data = NULL;
    struct input_dev *idev = NULL;
    int ret = 0;;

    DEBUG("I2C Address: 0x%02x", client->addr);

    if (tcs3701_check_id(client))
    {
        ERROR("check device id failed !");
        return -EPROBE_DEFER;
    }

    // uapi/linux/i2c.h
    if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_I2C_BLOCK))
    {
        ERROR("I2C check functionality failed !");
        return -ENODEV;
    }

    tcs3701_data = kzalloc(sizeof(tcs3701_data_t), GFP_KERNEL);
    if (IS_ERR_OR_NULL(tcs3701_data))
    {
        ERROR("cannot allocate memory, size: %u !", sizeof(tcs3701_data_t));
        return -ENOMEM;
    }
    tcs3701_data->client = client;
    i2c_set_clientdata(client, tcs3701_data);

    tcs3701_data->gpio_irq = of_get_named_gpio(client->dev.of_node, "gpio-irq", 0);
    if (!gpio_is_valid(tcs3701_data->gpio_irq))
    {
        ERROR("failed to find gpio-irq in dts !");
        ret = -ENODEV;
        goto ERR_1;
    }
    ret = gpio_request(tcs3701_data->gpio_irq, "tcs3701-irq");
    if (ret)
    {
        ERROR("request gpio irq(%d) failed !", tcs3701_data->gpio_irq);
        ret = (-EINVAL == ret)? -EPROBE_DEFER: ret;
        goto ERR_1;
    }


    idev = input_allocate_device();
    if (IS_ERR_OR_NULL(idev))
    {
        ERROR("allocate input device failed !");
        ret = PTR_ERR(idev);
        goto ERR_2;
    }
    tcs3701_data->idev = idev;
    set_bit(EV_SYN, idev->evbit);
    set_bit(EV_SW,  idev->evbit);
    set_bit(SW_FRONT_PROXIMITY, idev->swbit);
    idev->name = "tcs3701";
    idev->open  = tcs3701_idev_open;
    idev->close = tcs3701_idev_close;
    idev->id.bustype = BUS_I2C;
    idev->id.vendor  = 0;
    idev->id.product = 0;
    idev->id.version = 0x1;
    ret = input_register_device(idev);
    if (ret)
    {
        ERROR("register input device failed !\n");
        input_free_device(idev);
        idev = NULL;
        goto ERR_2;
    }


    ret = request_threaded_irq(gpio_to_irq(tcs3701_data->gpio_irq), NULL, tcs3701_irq_handler,
                               IRQF_TRIGGER_FALLING | IRQF_ONESHOT, "tcs3701", (void*)tcs3701_data);
    if (ret)
    {
        ERROR("request threaded irq failed !");
        goto ERR_3;
    }


    /* 设备初始化 */
    ret = tcs3701_device_init(client);
    ret |= tcs3701_prox_enable(client);
    if (ret)
    {
        ERROR("init tcs3701 failed !");
        ret = -EPROBE_DEFER;
        goto ERR_4;
    }

    return 0;


ERR_4:
    free_irq(gpio_to_irq(tcs3701_data->gpio_irq), (void*)tcs3701_data);
ERR_3:
    input_unregister_device(idev);
ERR_2:
    gpio_free(tcs3701_data->gpio_irq);
ERR_1:
    i2c_set_clientdata(client, NULL);
    kfree(tcs3701_data);
    return ret;
}
static int tcs3701_remove(struct i2c_client *client)
{
    tcs3701_data_t *tcs3701_data;

    tcs3701_poweroff(client);

    tcs3701_data = (tcs3701_data_t*)i2c_get_clientdata(client);
    if (IS_ERR_OR_NULL(tcs3701_data))
        return 0;

    free_irq(gpio_to_irq(tcs3701_data->gpio_irq), (void*)tcs3701_data);
    input_unregister_device(tcs3701_data->idev);
    gpio_free(tcs3701_data->gpio_irq);
    kfree(tcs3701_data);

    return 0;
}



static const struct of_device_id tcs3701_match_table[] =
{
    { .compatible = "ams,tcs3701", },
    { /* end */ },
};
static const struct i2c_device_id tcs3701_ids[] =
{
    { "ams,tcs3701", 0 },
    { /* end */ }
};
static struct i2c_driver tcs3701_driver =
{
    .probe    = tcs3701_probe,
    .remove   = tcs3701_remove,
    .id_table = tcs3701_ids,
    .driver = {
        .name  = "tcs3701",
        .owner = THIS_MODULE,
        .of_match_table = tcs3701_match_table,
    },
};


static int __init tcs3701_init(void)
{
    DEBUG("Driver Version: %s", VERSION);
    return i2c_add_driver(&tcs3701_driver);
}
static void __exit tcs3701_exit(void)
{
    DEBUG("exit");
    i2c_del_driver(&tcs3701_driver);
}


late_initcall(tcs3701_init);
module_exit(tcs3701_exit);


MODULE_AUTHOR("LLL");
MODULE_DESCRIPTION("TCS3701 driver");
MODULE_LICENSE("GPL");
