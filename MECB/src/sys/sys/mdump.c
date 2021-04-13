/* mdump.c - mdump, mdumph */

#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  mdump  --  dump free memory list
 *------------------------------------------------------------------------
 */
mdump()
{
	struct	mblock	*mptr;

	disable();
	kprintf("Dump of free memory block list:\n");
	for( mptr=memlist.mnext ; mptr!=NULL ; mptr=mptr->mnext)
		kprintf("  block at %x, length %u (0x%x)\n",
			mptr,mptr->mlen, mptr->mlen);
	restore();
}

/*------------------------------------------------------------------------
 *  mdumph  --  dump free memory list and halt
 *------------------------------------------------------------------------
 */
mdumph()
{
	struct	mblock	*mptr;

	disable();
	kprintf("Dump of free memory block list:\n");
	for( mptr=memlist.mnext ; mptr!=NULL ; mptr=mptr->mnext)
		kprintf("  block at %o, length %u (0%o)\n",
			mptr,mptr->mlen, mptr->mlen);
	halt();
	restore();
}
