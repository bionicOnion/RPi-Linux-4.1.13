#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/string.h>


volatile int race = 0;


typedef struct
{
	char name[12];
	struct task_struct* pThread;
	struct hrtimer hrTimer;
} ThreadContext_t;

/* -- represents state of each thread -- */
static ThreadContext_t threads[4];

/* -- entry point for the spawned thread -- */
static int threadEntryPoint( void *data )
{
	int retVal = 0;

	int i;
	for (i = 0; i < 1000000; ++i)
		race++;

	return retVal;
}


static int spawn_init(void)
{

	int index = 0;

	printk(KERN_DEBUG "spawn_init\n" );

	/* -- clear all task_struct pointers -- */
	for( index=0; index<4; index++)
	{
		memcpy( threads[index].name, "framework/", 10 );
		threads[index].name[10] = 48 + index;
		threads[index].name[11] = 0; 
		threads[index].pThread = NULL;
	}


	for( index=0; index<4; index++ )
	{

		/* -- create kernel thread in system -- */
		threads[index].pThread = kthread_create(&threadEntryPoint, (void*)index, threads[index].name );
		if( threads[index].pThread == NULL )
		{
			printk( KERN_ALERT "multi_thread_framework; kthread_create failed; index=0x%08x\n", index );
			continue;
		}

		/* -- bind kernel thread to specific cpu -- */
		kthread_bind( threads[index].pThread , index );

		/* -- wake up the process -- */
		wake_up_process( threads[index].pThread );
		
	}
	
	return 0;
}


static void spawn_exit(void)
{
	int retVal = 0;
	int index = 0;

	printk(KERN_DEBUG "spawn_exit\n" );
	printk(KERN_DEBUG "race: %d\n", race);

	if (0)
	{
		for( index = 0; index < 4; index++ )
		{
			if( threads[index].pThread != NULL )
			{

				/* -- stop kernel thread for cpu -- */
				retVal = kthread_stop( threads[index].pThread );
				if( retVal != NULL )
				{
					printk( KERN_ALERT "multi_thread_framework; thread_stop failed; cpu=0x%08x, retVal=0x%08x\n", index, retVal );
				}
			}
		}
	}
}


module_init(spawn_init);
module_exit(spawn_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple and Robert Miller");
MODULE_DESCRIPTION("Studio 8 Framework");
