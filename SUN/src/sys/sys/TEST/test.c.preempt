/* test.c - main */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  main  --  current test routines
 *------------------------------------------------------------------------
 */
main()
{
	int prhello(), proc();
	int i;

	prhello();
		for (i=0; i<10; i++) {
			resume (
				create (proc, 12000, 19, "proc", 1, i)
				);
			}
	kprintf ("created 10 processes...\n");
}

prhello()
{
	kprintf("hello world\n");
}

proc(num)
int num;
{
	int i;

	for (i=0;i >= 0; i = (i+1)%3000)
		if (i == 0)
			kprintf ("process # %d: executing\n", num);
}
