#include <stdio.h>
#include <time.h>

int main()
{

	struct timespec time;

	if( clock_getres( CLOCK_REALTIME, &time) )
	{
		printf( "Error: Realtime\n" );
	}
	printf( "CLOCK_REALTIME: seconds=%d, nano_sec=%d\n", time.tv_sec, time.tv_nsec );

	if( clock_getres( CLOCK_REALTIME_COARSE, &time) )
	{
		printf( "Error: Realtime Coarse\n" );
	}
	printf( "CLOCK_REALTIME_COARSE: seconds=%d, nano_sec=%d\n", time.tv_sec, time.tv_nsec );

	if( clock_getres( CLOCK_MONOTONIC, &time) )
	{
		printf( "Error: Monotonic\n" );
	}
	printf( "CLOCK_MONOTONIC: seconds=%d, nano_sec=%d\n", time.tv_sec, time.tv_nsec );

	if( clock_getres( CLOCK_MONOTONIC_COARSE, &time) )
	{
		printf( "Error: Monotonic\n" );
	}
	printf( "CLOCK_MONOTONIC_COARSE: seconds=%d, nano_sec=%d\n", time.tv_sec, time.tv_nsec );


	return 0;
}
