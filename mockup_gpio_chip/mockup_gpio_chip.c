#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>      /* For platform devices */
#include <linux/of.h>                   /* For device tree*/

#define DEV_NAME "mockup_gpio_chip"

// Number of gpio lines
#define NGPIO 16

static struct gpio_chip chip;

// Mockup array to read gpio values from
static int gpioLines[NGPIO];

static int fake_get_value(struct gpio_chip *gc, unsigned offset)
{
    return gpioLines[offset];
}

static void fake_set_value(struct gpio_chip *gc, unsigned offset, int val)
{
    pr_info("Write operation for %s (driver) is not supported\n", DEV_NAME);
}

static int fake_direction_output(struct gpio_chip *gc,
				       unsigned offset, int val)
{
	return 0;
}

static int fake_direction_input(struct gpio_chip *gc,
				       unsigned offset)
{
    return 0;
}

static const struct of_device_id fake_gpiochip_ids[] = {
    { .compatible = "packt,mockup_gpio_chip", },
    { /* sentinel */ }
};

static int my_pdrv_probe (struct platform_device *pdev)
{
    int ret;
	
    chip.label = pdev->name;
	chip.base = -1;
	chip.parent = &pdev->dev;
	chip.owner = THIS_MODULE;
	chip.ngpio = NGPIO;
	chip.can_sleep = 1;
	chip.get = fake_get_value;
	chip.set = fake_set_value;
	chip.direction_output = fake_direction_output;
	chip.direction_input = fake_direction_input;

    ret = gpiochip_add(&chip);
    
    if(ret < 0)
    {
        pr_info("Could not add %s (driver)\n", DEV_NAME);
        return -ENOMEM;
    }

    // Initalize gpioLines to 0
    memset(gpioLines, 0, sizeof(gpioLines));
    
    // Set some gpioLines in the mockup array to high
    gpioLines[0] = 1;
    gpioLines[1] = 1;
    gpioLines[4] = 1;
    gpioLines[5] = 1;
    
    pr_info("Loaded %s (driver)\n", DEV_NAME);

	return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    gpiochip_remove(&chip);
    pr_info("Unloaded %s (driver)\n", DEV_NAME);

    return 0;
}

static struct platform_driver mypdrv = {
    .probe      = my_pdrv_probe,
    .remove     = my_pdrv_remove,
    .driver     = {
        .name     = "mockup_gpio_chip",
        .of_match_table = of_match_ptr(fake_gpiochip_ids),
        .owner    = THIS_MODULE,
    },
};
module_platform_driver(mypdrv);
MODULE_AUTHOR("John Madieu <john.madieu@gmail.com> & Feirik");
MODULE_LICENSE("GPL");
