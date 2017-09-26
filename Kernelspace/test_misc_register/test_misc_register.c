#include <linux/miscdevice.h> 
#include <linux/fs.h>
#include <linux/module.h>

#define RAY_DEVICE "ray"

MODULE_LICENSE("GPL");

static int ray_open(struct inode *inode, struct file *file)
{
    printk("%s, %d\n", __FUNCTION__, __LINE__);;
    return 0;
}

static int ray_release(struct inode *inode, struct file *file)
{
    printk("%s, %d\n", __FUNCTION__, __LINE__);;
    return 0;
}

static int ray_ioctl(struct inode *inode, uint32_t ioctl_num, unsigned long ioctl_param)
{
    printk("%s, %d\n", __FUNCTION__, __LINE__);;
    return 0;
}

static const struct file_operations ray_fops = {
    .owner = THIS_MODULE,
    .open = ray_open,
    .release = ray_release,
    .unlocked_ioctl = (void *)ray_ioctl,
};

static struct miscdevice ray_misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = RAY_DEVICE,
    .fops = &ray_fops,
};

static int __init init_modules(void)
{
    int rc;

    printk("test_misc_register\n");

    rc = misc_register(&ray_misc);
    if (rc != 0) {
        printk("Misc registration failed\n");
        return -1;
    }

    return 0;
}

static void __exit exit_modules(void)
{
    misc_deregister(&ray_misc);
}

module_init(init_modules);
module_exit(exit_modules);
