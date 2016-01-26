/**
 * @file sys_printlog.c
 *
 * @brief
 *	This file provides the implementation of the printlog system call. This
 *	call is compiled into the linux kernel.
 *
 * @author Cameron Whipple, Robert Miller
 * @date 01/24/2016
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
