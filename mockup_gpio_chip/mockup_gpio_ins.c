#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/types.h>

#define DEV_NAME "mockup_gpio_chip"

static struct platform_device *pdev = NULL;

static int platform_dummy_char_add(void)
{
    int ret;

    // Let instance ID be automatically assigned
    int inst_id = -2;

    pdev = platform_device_alloc(DEV_NAME, inst_id);
    if(pdev == NULL)
    {
        return -ENOMEM;
    }

    // Error check
    ret = platform_device_add(pdev);
    if(ret != 0)
    {
        platform_device_put(pdev);
    }

    pr_info("Loaded %s (insert)\n", DEV_NAME);

    return 0;
}

static void fplatform_dummy_char_put(void)
{
	platform_device_del(pdev);
    pr_info("Unloaded %s (insert)\n", DEV_NAME);
}

module_init(platform_dummy_char_add);
module_exit(fplatform_dummy_char_put);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("John Madieu <john.madieu@gmail.com> & Feirik");
