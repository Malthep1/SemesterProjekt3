#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xfef71adc, "module_layout" },
	{ 0xb09c900d, "cdev_del" },
	{ 0xfe990052, "gpio_free" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xadb42e9b, "class_destroy" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x4add4d87, "cdev_add" },
	{ 0x6904051d, "cdev_init" },
	{ 0x2817a91e, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x52f00843, "gpiod_direction_input" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x2cfde9a2, "warn_slowpath_fmt" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x963e0acd, "finish_wait" },
	{ 0x93f6bff1, "prepare_to_wait_event" },
	{ 0x1000e51, "schedule" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x413c0466, "__wake_up" },
	{ 0x6d695099, "gpiod_get_raw_value" },
	{ 0x822863cd, "gpiod_to_irq" },
	{ 0x5f754e5a, "memset" },
	{ 0x125debe4, "gpiod_set_raw_value" },
	{ 0xff41b612, "gpio_to_desc" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xc5850110, "printk" },
	{ 0x6a6e05bf, "kstrtou8" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x8f678b07, "__stack_chk_guard" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "EB2EC1502B1D655F5C452AA");
