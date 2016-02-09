#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


static int spawn_init(void)
{
   printk(KERN_DEBUG "spawn_init\n" );
	return 0;
}


static void spawn_exit(void)
{
   printk(KERN_DEBUG "spawn_exit\n" );
}


module_init(spawn_init);
module_exit(spawn_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple and Robert Miller");
MODULE_DESCRIPTION("Kernel Monitoring Framework");
