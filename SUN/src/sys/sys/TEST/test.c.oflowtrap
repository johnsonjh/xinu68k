/* test.c - main */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  main  --  current test routines
 *------------------------------------------------------------------------
 */
main()
{
	int i;

	kprintf("Testing integer overflow trap\n");
	for (i=0x7ffffffc; i != 0x80000000; i++)
		kprintf ("trying %d\n", i);
	kprintf("No overflow trap!!!!\n");
}
