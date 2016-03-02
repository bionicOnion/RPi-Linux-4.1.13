#define _GNU_SOURCE


#include <math.h>
#include <sched.h>
#include <stdio.h>


int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("Usage: %s [core number] [priority]\n", argv[0]);
		return 1;
	}

	// Pin to the specified CPU
	int requestedCPU = atoi(argv[1]);
	printf("%d\n", requestedCPU);
	cpu_set_t cpuSet;
	CPU_ZERO(&cpuSet);
	CPU_SET(requestedCPU, &cpuSet);
	sched_setaffinity(getpid(), sizeof(cpuSet), &cpuSet);

	// Use the specified scheduling model
	int requestedPriority = atoi(argv[2]);
	struct sched_param schedParam;
	schedParam.sched_priority = requestedPriority;
	if (sched_setscheduler(0, SCHED_RR, &schedParam) != 0)
	{
		printf("Failed to set the requested scheduler priority.\n");
		return 2;
	}
	
	// Perform 'work'
	for(i = 0; i < 500000000; ++i);

	return 0;
}
