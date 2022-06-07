#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define ERRGOTO(label, ...) \
    do {                    \
    pr_err(__VA_ARGS__);    \
    goto label;             \
    } while(0)

// REMEMBER!!! CHANGE TO GPIO 19 OR WHATEVER WE USE FOR THE BUTTON!!!
#define GV 16

const int first_minor = 0;
const int max_devices = 255;
static dev_t denvo;
static struct class *button_class;
static struct cdev button_cdev;

// initializes wait_queue_head_t wq
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int event_flag = 0;
static int isr_gpio_value;


static irqreturn_t isr_button_handler(int irq, void *dev_id){
    isr_gpio_value = gpio_get_value(GV);
    event_flag = 1;
    wake_up_interruptible(&wq);
    return IRQ_HANDLED;
} 

static ssize_t button_state_store(struct device *dev, struct device_attribute * attr, const char *buf, size_t size)
{
    u8 value;

    int err = kstrtou8(buf, 0, &value);
    if (err<0) 
    {
        printk(KERN_ALERT "Unable to parse string\n");
        return err;
    }

    printk("Store called with %i\n", value);

    return size;
}

static ssize_t button_state_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    int len = sprintf(buf, "%d\n", 1);

    return len;
}

DEVICE_ATTR_RW(button_state);

static struct attribute* button_attrs[] = {
    &dev_attr_button_state.attr,
    NULL
};

int mygpio_open(struct inode *inode, struct file *filep)
{
    int major, minor, err;
    major = MAJOR(inode->i_rdev);
    minor = MINOR(inode->i_rdev);
    err = request_irq(gpio_to_irq(GV), isr_button_handler, IRQF_TRIGGER_FALLING, "button_isr", NULL);
    printk("Opening MyGpio Device [major], [minor]: %i, %i\n", major, minor);
    return 0;
}

int mygpio_release(struct inode *inode, struct file *filep)
{
    int minor, major;

    major = MAJOR(inode->i_rdev);
    minor = MINOR(inode->i_rdev);
    free_irq(gpio_to_irq(GV), NULL);
    printk("Closing/Releasing MyGpio Device [major], [minor]: %i, %i\n", major, minor);

    return 0;
}

ssize_t mygpio_read(struct file *filep, char __user *buf, 
            size_t count, loff_t *f_pos)
{
    wait_event_interruptible(wq, event_flag != 0);
    char kbuf[12];
    int len, value;
    value = isr_gpio_value;
    len = count< 12 ? count: 12;   /* Truncate to smallest */
    len = snprintf(kbuf, len, "%i", value); /* Create string */
    pr_info("ISR triggered. Button value: %i\n", value);
    int ctu = copy_to_user(buf, kbuf, ++len);
    if(ctu < 0){pr_info("Error reading GPIO\n");}
    event_flag = 0;
    *f_pos += len;
    return len;
}

struct file_operations button_fops = {
    .owner =    THIS_MODULE,
    .open =     mygpio_open,
    .release =  mygpio_release,
    .read =     mygpio_read,
};

static int mygpio_probe(struct platform_device *bdev)
{
    printk("Hello from probe %s\n", bdev->name);
    device_create(button_class, NULL, MKDEV(MAJOR(denvo), MINOR(denvo)), NULL, "button", GV);
    return 0;
}

static int mygpio_remove(struct platform_device *bdev)
{
    printk("Hello from remove %s\n", bdev->name);
    device_destroy(button_class, MKDEV(MAJOR(denvo), MINOR(denvo)));
    return 0;
}

static const struct of_device_id my_button_device_match[] = 
{
    { .compatible = "ase, button_drv",},{},
};

static struct platform_driver my_button_driver = 
{
    .probe = mygpio_probe,
    .remove = mygpio_remove,
    .driver = {
        .name = "button_drv",
        .of_match_table = my_button_device_match,
        .owner = THIS_MODULE, }, 
};

ATTRIBUTE_GROUPS(button);

static int __init mygpio_init(void)
{
    int err=0;

    int gpio = gpio_request(GV, "Button");
    if (gpio < 0) ERRGOTO(GPIO_REQUEST_FAILED, "Failed to request GPIO\n");

    int gpio_dr = gpio_direction_input(GV);
    if (gpio_dr < 0) ERRGOTO(GPIO_DIRECTION_FAILED, "Failed to set GPIO direction");
    
    err = alloc_chrdev_region(&denvo, first_minor, max_devices, "button");
    if (MAJOR(denvo) <= 0)
        ERRGOTO(ALLOC_MAJOR_MINOR_FAILED, "Failed to register chardev\n");
    
    pr_info("Button driver got Major %i\n", MAJOR(denvo));
    
    button_class = class_create(THIS_MODULE, "Button_class");
    if(IS_ERR(button_class)) ERRGOTO(CLASS_CREATE_FAILED,"Failed to create class");

    button_class->dev_groups = button_groups;

    err = platform_driver_register(&my_button_driver);

    cdev_init(&button_cdev, &button_fops);
    err = cdev_add(&button_cdev, denvo, 255);
    if (err) ERRGOTO(CDEV_INIT_ADD_FAILED,"Failed to add cdev");



    return err;

    CDEV_INIT_ADD_FAILED: class_destroy(button_class);
    CLASS_CREATE_FAILED: unregister_chrdev_region(first_minor, max_devices);
    ALLOC_MAJOR_MINOR_FAILED: 
    GPIO_DIRECTION_FAILED: gpio_free(GV);
    GPIO_REQUEST_FAILED: 

    return err;
}

static void __exit mygpio_exit(void)
{
    cdev_del(&button_cdev);
    class_destroy(button_class);
    unregister_chrdev_region(first_minor, max_devices);
    gpio_free(GV);
}

module_init(mygpio_init);
module_exit(mygpio_exit);
MODULE_AUTHOR("Frederik");
MODULE_LICENSE("GPL");

