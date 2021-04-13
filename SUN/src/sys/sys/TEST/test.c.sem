/* test.c - main */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  main  --  current test routines
 *------------------------------------------------------------------------
 */
main()
{
	int prhello(), waiter(), signaler();
	int isem, i;

	if ((isem  = screate (0)) == SYSERR)
		kprintf ("waiter: cannot get semaphore\n");
	else {
		for (i=0; i<NPROC-2; i++) {
			resume (
				create (waiter, 12000, 20, "waiter", 2, isem, i)
				);
			if (!(i % 5)) {
				resume (
				    create (signaler,12000,20,"sigler",2,isem,i)
					);
				}
			}
		for (i=0; i<NPROC-2; i++)
			if (i % 5)
				resume (
				    create (signaler,12000,20,"sigler",2,isem,i)
					);
	}
}

prhello()
{
	kprintf("hello world\n");
}

waiter(isem, iwait)
int isem, iwait;
{
	kprintf ("waiter #%d: waiting for signal\n", iwait);
	wait (isem);
	kprintf ("waiter #%d: my semaphorer was signalled\n", iwait);
}

signaler(isem, iwait)
int isem, iwait;
{
	kprintf ("signaler #%d: about to signal and die\n", iwait);
	signal (isem);
}
