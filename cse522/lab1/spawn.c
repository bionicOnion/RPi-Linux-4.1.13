#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>

/* -- holds reference to kernel thread that was created -- */
struct task_struct* pThread = 0;

/* -- entry point for the spawned thread -- */
static int threadEntryPoint( void *data )
{

	printk( KERN_DEBUG "Spawned thread started\n" );

//	while(1)
//	{
		printk( KERN_DEBUG "threadEntryPoint\n" );
		schedule();
//	}

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
	printk(KERN_DEBUG "spawn_exit\n" );
}


module_init(spawn_init);
module_exit(spawn_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple and Robert Miller");
MODULE_DESCRIPTION("Kernel Monitoring Framework");
