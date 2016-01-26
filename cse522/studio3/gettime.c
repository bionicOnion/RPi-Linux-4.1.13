#include <stdio.h>
#include <time.h>

int main()
{

	struct timespec start;
	struct timespec end;
	long diffTime = 0;

	clock_gettime( CLOCK_MONOTONIC, &start);
	clock_gettime( CLOCK_MONOTONIC, &end) ;

	diffTime = end.tv_nsec - start.tv_nsec;

	printf( "Diff %d\n", diffTime );

	return 0;
}
