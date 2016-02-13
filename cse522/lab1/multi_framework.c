#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

/* -- holds reference to kernel thread that was created -- */
struct task_struct* pThread = 0;

/* -- holds the amount of time between iterations of loop -- */
ktime_t time;

/* -- holds the timer used to reactivate the background thread -- */
struct hrtimer hrTimer;

/* -- stores the frequency with which the monitoring thread will wakeup; default to 1 second -- */
static long log_sec = 1;
static unsigned long log_nsec = 0;

/* @todo - Determine proper permissions for the variables */
module_param(log_sec, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(log_sec, "Number of seconds as a long");

module_param(log_nsec, unsigned long, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(log_nsec, "Number of nanoseconds as an unsigned long");

/* static int resetStateEntryPoint( void *data ) */
enum hrtimer_restart resetStateEntryPoint(struct hrtimer * pTimer)
{
	
	/* -- mark the thread as scheduler -- */
	wake_up_process( pThread );

	return HRTIMER_NORESTART;
}

/* -- entry point for the spawned thread -- */
static int threadEntryPoint( void *data )
{

	int retVal = 0;


	printk( KERN_DEBUG "monitor_framework_thread; seconds=%ld, nanoseconds=%lu\n", log_sec, log_nsec );

	/* -- configuring timeouts -- */
	time = ktime_set( log_sec, log_nsec );

	/* -- bind timer to use clock monotonic -- */
	hrtimer_init( &hrTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );

	/* -- configure timer structure -- */
	hrTimer.function = &resetStateEntryPoint;

	/* -- detect if thread should stop -- */
	while(!kthread_should_stop())
	{

		/* -- make thread not schedulable -- */
		set_current_state(TASK_INTERRUPTIBLE);

		/* -- set timer to make thread schedulable -- */
		hrtimer_start(&hrTimer, time, HRTIMER_MODE_REL);

		/* -- activate scheduler -- */
		schedule();

      printk( KERN_DEBUG "monitor_framework_thread; nvcsw=%d, nivcsw=%d\n", current->nvcsw, current->nivcsw );
	}

	/* -- attempt to cancel the timer -- */
	retVal = hrtimer_try_to_cancel(&hrTimer);
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
	printk(KERN_DEBUG "spawn_init\n" );
	
   /*	cpu_online_mask */

	/* -- creates and runs the thread -- */
	pThread = kthread_run(&threadEntryPoint, NULL, "Framework Monitor");
	if( pThread == NULL )
	{
		printk( KERN_ALERT "KThread_Create Failed\n" );
		return -1;
	}
	
	/* http://lxr.free-electrons.com/source/kernel/trace/ring_buffer.c#L4883 */

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
