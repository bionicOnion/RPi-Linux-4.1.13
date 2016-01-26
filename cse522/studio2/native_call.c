/**
 * @file native_call.c
 *
 * @brief This file attempts to switch the user id of the application
 *			to 0. This application will make use of the system calls for
 *			setuid() and getuid().
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
	uid_t postID = 0;
	uid_t priorID = 0;

	/* -- retrieve user id -- */
	priorID = syscall(24);

	/* -- set user id to root -- */
	/* rv = setuid( 0 ); */
	rv = syscall( 23, 0 );
	if( rv != 0 )
	{
		printf( "Error to setuid failed! Reason: %s\n", strerror(errno));
	}

	/* -- retrieve user id -- */
	postID = syscall(24);

	/* -- print results -- */
	printf( "UID Before: %d, UID After: %d\n", priorID, postID );

	return 0;

}

