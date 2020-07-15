/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5	//number of philosophers and forks

//TODO - locks must be declared and initialized here
mutex_t forks[N] = {FALSE};
mutex_t printLock = FALSE;

/**
 * Delay for a random amount of time
 * @param alpha delay factor
 */
void	holdup(int32 alpha)
{
	long delay = rand() * alpha;
	while (delay-- > 0)
		;	//no op
}

/**
 * Eat for a random amount of time
 */
void	eat()
{
	holdup(10000);
}

/**
 * Think for a random amount of time
 */
void	think()
{
	holdup(1000);
}



/**
 * Philosopher's code
 * @param phil_id philosopher's id
 */
void	philosopher(uint32 phil_id)
{
	uint32 right = (phil_id+N-1)%N;	//TODO - right fork
	uint32 left = phil_id;	//TODO - left fork

	srand(phil_id);

	while (TRUE)
	{
		int r = rand()%100;
		if(r < 70) { //think 70% of the time
			mutex_lock(&printLock); // lock for printf
			kprintf("Philosopher %d thinking: zzzzzZZZz\n", phil_id);
			mutex_unlock(&printLock); // unlock printf
			think();
		} else { //(attempt to) eat 30% of the time
			mutex_lock(&forks[right]); // try to grab right fork
			if(forks[left]==FALSE) { // try to grab left fork
				mutex_lock(&forks[left]);
				mutex_lock(&printLock); // lock printf
				kprintf("Philosopher %d eating: nom nom nom\n", phil_id);
				mutex_unlock(&printLock); // unlock printf
				mutex_unlock(&forks[left]);
			} // don't grab left fork if already taken
			mutex_unlock(&forks[right]); 
		}
	}
}

int	main(uint32 argc, uint32 *argv)
{
	// int i;
	// for(i=0;i<N;i++) kprintf("i=%d: %d",i,forks[i]);

	//do not change
	ready(create((void*) philosopher, INITSTK, 15, "Ph1", 1, 0), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph2", 1, 1), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph3", 1, 2), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph4", 1, 3), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph5", 1, 4), FALSE);

	return 0;
}
