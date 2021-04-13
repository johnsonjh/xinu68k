/* test.c - main */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  main  --  current test routines
 *------------------------------------------------------------------------
 */
main()
{
	int prhello();
	int i;

	prhello();
	for (i=0; i<10; i++) {
		kprintf ("sleeping for %d seconds...\n",i);
		sleep (i);
		}
}
prhello()
{
	kprintf ("Hello, world\n");
}
