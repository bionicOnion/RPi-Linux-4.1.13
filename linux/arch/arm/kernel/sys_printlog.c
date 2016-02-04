/* This file implements a demonstration syscall for an OS course. This is a
*  horrible function and your kernel should not include it. It sets the
*  userid of calling process to 0, giving it root permission.
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

//Define a system call implementation that takes no arguments
SYSCALL_DEFINE1( printlog, int, argument ){

	return printk( "CSE522 Print: %d\n", argument );
	
}

//End of file
