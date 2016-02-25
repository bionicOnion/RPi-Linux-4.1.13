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
#include <linux/futex.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <limits.h>

//ITERS = 500,000,000 will take about five seconds on a 900MHz Raspberry Pi 2
#define ITERS 500000000
#define UNLOCKED 1
#define LOCKED       0

void lock(volatile int* lock_ptr)
{
	do
	{
		int ret_val = __atomic_sub_fetch( lock_ptr, 1, __ATOMIC_ACQ_REL );
		if (ret_val < LOCKED)
		{
			__atomic_store_n( lock_ptr, -1, __ATOMIC_RELEASE );
			syscall( SYS_futex, lock_ptr, FUTEX_WAIT, -1, NULL );
		}
		else return;
	} while (1);
}

void unlock(volatile int* lock_ptr)
{
	int ret_val = __atomic_add_fetch( lock_ptr, 1, __ATOMIC_ACQ_REL );
	if (ret_val != UNLOCKED)
	{
		__atomic_store_n( lock_ptr, UNLOCKED, __ATOMIC_RELEASE );
		syscall( SYS_futex, lock_ptr, FUTEX_WAKE, INT_MAX );
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


