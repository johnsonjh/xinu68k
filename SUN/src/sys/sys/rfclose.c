/* rfclose.c - rfclose */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rfclose  --  close a remote file by deallocating pseudo device
 *------------------------------------------------------------------------
 */
rfclose(devptr)
struct	devsw	*devptr;
{
	struct	rfblk	*rfptr;
	long	junk;		/* argument to rfmkpac; not really used	*/

	rfptr = (struct rfblk *)devptr->dvioblk;
	if (rfptr->rf_state == RFREE)
		return(SYSERR);
	wait(rfptr->rf_mutex);
	junk = 0L; /* 0L is long zero constant */
	rfmkpac(FS_CLOSE,rfptr->rf_name,&junk,(char *)&junk,0);
	rfptr->rf_state = RFREE;
	signal(rfptr->rf_mutex);
	return(OK);
}
