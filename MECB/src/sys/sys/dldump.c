/* dldump.c - dldump, dldumph */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sleep.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  dldump  --  dump the contents of the dlc control blocks
 *------------------------------------------------------------------------
 */
dldump()
{
	int i;
	struct	dlblk *dptr;

	for (i=0 ; i<Ndlc ; i++) {
		kprintf("\ndlc device %d:\n", i);
		dptr = &dlc[i];
		kprintf("\n");
		kprintf("     csr address=0%o, istate=%d, ostate=%d\n",
			dptr->dioaddr, dptr->distate, dptr->dostate);
		kprintf("     istart=0%o, inext=0%o\n",
			dptr->distart, dptr->dinext);
		kprintf("     icount=%d, imax=%d\n",
			dptr->dicount, dptr->dimax);
		kprintf("     ostart=0%o, onext=0%o\n",
			dptr->dostart, dptr->donext);
		kprintf("     ocount=%d, otot=%d\n",
			dptr->docount, dptr->dotot);
		kprintf("     oproc=%d, iproc=%d, (nonblock proc=%d)\n",
			dptr->doproc, dptr->diproc, dptr->dpid);
		kprintf("     iesc=%d, oesc=%d\n",
			dptr->diesc, dptr->doesc);
		kprintf("     dochar=%c, dochvalid=0%o\n",
			dptr->dochar, dptr->dovalid);
	}
}

/*------------------------------------------------------------------------
 *  dldumph  --  dump the dlc control blocks and halt
 *------------------------------------------------------------------------
 */
dldumph()
{
	dldump();
	kprintf("Dump complete -- type P to continue\n");
	halt();
}
