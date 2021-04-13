/* rfinit.c - rfinit */

#include <conf.h>
#include <kernel.h>
#include <fserver.h>
#include <rfile.h>

/*------------------------------------------------------------------------
 *  rfinit  --  initialize remote file pseudo devices
 *------------------------------------------------------------------------
 */
rfinit(devptr)
struct	devsw	*devptr;
{
	struct	rfblk	*rfptr;

	devptr->dvioblk = (char *) (rfptr = &Rf.rftab[devptr->dvminor]);
	rfptr->rf_dnum = devptr->dvnum;
	rfptr->rf_name[0] = NULLCH;
	rfptr->rf_state = RFREE;
	rfptr->rf_mutex = screate(1);
	rfptr->rf_pos = 0L;
	if (devptr->dvminor == 0) {	/* done just once */
		Rf.device = RCLOSED;
		Rf.rmutex = screate(1);
	}
}

struct	rfinfo	Rf;
