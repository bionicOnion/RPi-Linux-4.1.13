#define _GNU_SOURCE


#include <math.h>
#include <sched.h>
#include <stdio.h>


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("nope\n");
		return 1;
	}

	int requestedCPU = atoi(argv[1]);
	printf("%d\n", requestedCPU);

	cpu_set_t cpuSet;
	CPU_ZERO(&cpuSet);
	CPU_SET(requestedCPU, &cpuSet);

	sched_setaffinity(getpid(), sizeof(cpuSet), &cpuSet);
	
	for(;;);

	return 0;
}
