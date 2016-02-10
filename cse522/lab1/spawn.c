#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

/* -- holds reference to kernel thread that was created -- */
struct task_struct* pThread = 0;

/* -- entry point for the spawned thread -- */
static int threadEntryPoint( void *data )
{
	unsigned int count = 0;

	printk( KERN_DEBUG "Spawned thread started\n" );

   /* -- detect if thread should stop -- */
	while(!kthread_should_stop())
	{
		msleep(1000);
		schedule();
		count++;
		printk( KERN_DEBUG "monitor_framework_thread; count=0x%08x\n", count );
	}

	printk( KERN_DEBUG "monitor_framework_thread; CLOSING\n" );

	return 0;
}


static int spawn_init(void)
{
	printk(KERN_DEBUG "spawn_init\n" );

	/* -- creates and runs the thread -- */
	pThread = kthread_run(&threadEntryPoint, NULL, "Framework Monitor");
	if( pThread == NULL )
	{
		printk( KERN_ALERT "KThread_Create Failed\n" );
		return -1;
	}

	return 0;
}


static void spawn_exit(void)
{
	int retVal = 0;

	printk(KERN_DEBUG "spawn_exit\n" );

	if( pThread != 0 )
	{
		retVal = kthread_stop(pThread);
		if( retVal != 0 )
		{
			printk( KERN_ALERT "kthread_stop failed; ret=0x%08x\n", retVal );
		}
	}
}


module_init(spawn_init);
module_exit(spawn_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple and Robert Miller");
MODULE_DESCRIPTION("Kernel Monitoring Framework");
