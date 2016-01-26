/**
 * @file badcall_call.c
 *
 * @brief This file provides the implementation of the test application
 *			that makes use of the badcall system call.
 *
 * @author Cameron Whipple, Robert Miller
 * @date 01/24/2016
 */
#include <stdio.h>   /* printf */
#include <unistd.h> /* getuid */
#include <sys/types.h> /* getuid() */
#include <errno.h> /* strerror() */

int main()
{

	long rv = 0;	
	uid_t postID = 0;
	uid_t priorID = 0;

	/* -- retrieve user id -- */
	priorID = getuid();

	/* -- set user id to root -- */
	/* rv = setuid( 0 ); */
	rv = syscall(388);
	if( rv != 0 )
	{
		printf( "Error to setuid failed! Reason: %s\n", strerror(errno));
	}

	/* -- retrieve user id -- */
	postID = getuid();

	/* -- print results -- */
	printf( "UID Before: %d, UID After: %d\n", priorID, postID );

	return 0;

}

