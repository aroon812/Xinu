/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

void	bigargs(int32 argc, int32 *argv)
{
	int a = argv[0];
	int b = argv[1];
	int c = argv[2];
	int d = argv[3];
	int e = argv[4];
	int f = argv[5];
	kprintf("bigargs(%d, %d, %d, %d, %d, %d) == %d\r\n", a, b, c, d, e, f, a+b+c+d+e+f);
}


void	printpid(int32 argc, int32 *argv)
{
	int i;
	for (i=0; i<argv[0]; i++)
	{
		kprintf("This is process %d (%s)\r\n", currpid, proctab[currpid].prname);
		resched();
	}
}

void longProcess(){
	int i;
	for (i = 0; i < 1000; i++)
	{
		kprintf("i: %u\n", i);
		resched();
	}
}

int	main(uint32 argc, uint32 *argv)
{
	static uint32 args[] = {1, 2, 3};
	static uint32 *args1 = args;
	static uint32 args2[] = {10, 20, 30, 40, 50, 60};

	kprintf("Hello XINU WORLD!\r\n");
	ready(create((void*) printpid, INITSTK, 10, "1", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "2", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "3", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "4", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "5", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "6", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "7", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "8", 2, 1, args1++), FALSE);
	ready(create((void*) printpid, INITSTK, 10, "9", 2, 1, args1++), FALSE);
	ready(create((void*) bigargs, INITSTK, 5, "BIGARGS", 2, 6, args2), FALSE);

	return 0;
}
