#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

extern unsigned long volatile jiffies;

static int hello_init(void)
{
	printk(KERN_ALERT "I bear a charmed life. jiffies=%lu\n", jiffies);
	/* return 0; */
	/* return -1; */
	return 2;
}


static void hello_exit(void)
{
	printk(KERN_ALERT "Out, out brief candle! jiffies=%lu\n", jiffies);
}


module_init(hello_init);
module_exit(hello_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shakespeare");
MODULE_DESCRIPTION("A Hello, World Module");
