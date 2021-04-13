/* rfdump.c - rfdump */

#include <conf.h>
#include <kernel.h>
#include <fserver.h>
#include <rfile.h>

/*------------------------------------------------------------------------
 *  rfdump  --  dump the contents of the remote file device blocks
 *------------------------------------------------------------------------
 */
rfdump()
{
	struct	rfblk	*rfptr;
	int	i;

	kprintf("Remote files: server on dev=%d, server mutex=%d\n",
			Rf.device, Rf.rmutex);
	for (i=0 ; i<Nrf; i++) {
		rfptr = &Rf.rftab[i];
		if (rfptr->rf_state == RFREE)
			continue;
		kprintf(" %2d. ", rfptr->rf_dnum);
		kprintf("mode=%03o, sem=%2d, pos=%6D, file name=%s\n",
				rfptr->rf_mode,	rfptr->rf_mutex,
				rfptr->rf_pos, rfptr->rf_name);
	}
}
