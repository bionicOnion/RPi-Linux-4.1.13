/*    Simulated workload with OpenMP
 *
 * This program will create about five seconds of work on each processor
 * on the system.
 *
 * This program requires the OpenMP compiler flag and optimizations turned
 * off to build correctly, e.g.: gcc -fopenmp workload.c -o work
 */

#include <stdio.h> //For printf()
#include <sched.h> //For sched_getcpu()
#include <stdbool.h>
#include <time.h>

//ITERS = 500,000,000 will take about five seconds on a 900MHz Raspberry Pi 2
#define ITERS 500000000
#define UNLOCKED 0
#define LOCKED       1

void lock(volatile int* thing)
{
	int unlocked = UNLOCKED;
	int locked = LOCKED;

	while (!__atomic_compare_exchange(thing, &unlocked, &locked, true,
		__ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE))
	{
		unlocked = UNLOCKED;
	}
}

void unlock(volatile int* thing)
{
	int unlocked = UNLOCKED;
	int locked = LOCKED;

	if (!__atomic_compare_exchange(thing, &locked, &unlocked, true,
		__ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE))
	{
		locked = LOCKED;
		printf("Failed to release the lock\n");
	}
}

void critical_section( void ){
	int index = 0;
	while(index < ITERS){ index++; }
}

int main (int argc, char* argv[]){
	volatile int thing = UNLOCKED;

	//Create a team of threads on each processor
	#pragma omp parallel
	{
		//Each thread executes this code block independently
		lock(&thing);
		critical_section();
		unlock(&thing);

		struct timespec time;
		clock_gettime(CLOCK_MONOTONIC, &time);		

		printf("CPU %d finished: %lu\n", sched_getcpu(), time.tv_sec);
	}

	return 0;
}


