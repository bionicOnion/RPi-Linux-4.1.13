/**
 * @file printlog_call.c
 *
 * @brief
 *	This file makes use of the printlog syscall that was added to the
 *	kernel. The printlog syscall will print a message to the kernel log.
 *
 * @author Cameron Whipple, Robert Miller
 * @date 01/24/2016
 */
#include <stdio.h>   /* printf */
#include <unistd.h> /* getuid */
#include <sys/types.h> /* getuid() */
#include <errno.h> /* strerror() */
#include <sys/syscall.h>

int main()
{

	long rv = 0;
	int index = 0;

	for( index = 0; index < 10; index++ )
	{
		syscall( 389, index );
	}

	return 0;

}

