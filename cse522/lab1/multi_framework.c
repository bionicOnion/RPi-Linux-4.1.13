#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/string.h>

typedef struct
{
	char name[12];
	struct task_struct* pThread;
	struct hrtimer hrTimer;
} ThreadContext_t;

/* -- represents state of each thread -- */
static ThreadContext_t threads[4];

/* -- holds the amount of time between iterations of loop -- */
ktime_t time;

/* -- stores the frequency with which the monitoring thread will wakeup; default to 1 second -- */
static long log_sec = 1;
static unsigned long log_nsec = 0;

module_param(log_sec, ulong, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(log_sec, "Number of seconds as a long");

module_param(log_nsec, ulong, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(log_nsec, "Number of nanoseconds as an unsigned long");

/* static int resetStateEntryPoint( void *data ) */
enum hrtimer_restart resetStateEntryPoint(struct hrtimer * pTimer)
{

#if 1
	ThreadContext_t *pContext = container_of( pTimer, ThreadContext_t, hrTimer);
	wake_up_process( pContext->pThread );
#else
	int index = smp_processor_id();
	wake_up_process( threads[index].pThread );
#endif

	return HRTIMER_NORESTART;
}

/* -- entry point for the spawned thread -- */
static int threadEntryPoint( void *data )
{

	int retVal = 0;
	int index = (int)data;

	printk( KERN_DEBUG "monitor_framework_thread; seconds=%ld, nanoseconds=%lu, cpu=%d\n", log_sec, log_nsec, index );

	/* -- configuring timeouts -- */
	time = ktime_set( log_sec, log_nsec );

	/* -- bind timer to use clock monotonic -- */
	hrtimer_init( &(threads[index].hrTimer), CLOCK_MONOTONIC, HRTIMER_MODE_REL );

	/* -- configure timer structure -- */
	threads[index].hrTimer.function = &resetStateEntryPoint;

	/* -- detect if thread should stop -- */
	while(!kthread_should_stop())
	{

		/* -- make thread not schedulable -- */
		set_current_state(TASK_INTERRUPTIBLE);

		/* -- set timer to make thread schedulable -- */
		hrtimer_start( &(threads[index].hrTimer), time, HRTIMER_MODE_REL);

		/* -- activate scheduler -- */
		schedule();

		printk( KERN_DEBUG "monitor_framework_thread; nvcsw=%lu, nivcsw=%lu, cpu=0x%08x\n", current->nvcsw, current->nivcsw, smp_processor_id() );
	}

	/* -- attempt to cancel the timer -- */
	retVal = hrtimer_try_to_cancel( &(threads[index].hrTimer));
	if( retVal == 1 )
	{
		/* -- timer was active -- */
		printk( KERN_DEBUG "monitor_framework_thread; timer was active\n" );
	}
	else if( retVal == 0 )
	{
		/* -- timer wasn't active -- */
		printk( KERN_DEBUG "monitor_framework_thread; timer wasn't active\n" );
	}
	else if( retVal == -1 )
	{
		/* -- timer within callback handler -- */
		printk( KERN_ALERT "monitor_framework_thread; within callback\n" );
	}

	printk( KERN_DEBUG "monitor_framework_thread; CLOSING\n" );

	return 0;
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
	
	/* http://lxr.free-electrons.com/source/kernel/trace/ring_buffer.c#L4883 */

	return 0;
}


static void spawn_exit(void)
{
	int retVal = 0;
	int index = 0;

	printk(KERN_DEBUG "spawn_exit\n" );

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


module_init(spawn_init);
module_exit(spawn_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple and Robert Miller");
MODULE_DESCRIPTION("Kernel Monitoring Framework");
