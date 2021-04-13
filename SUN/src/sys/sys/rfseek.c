/* rfseek.c - rfseek */

#include <conf.h>
#include <kernel.h>
#include <fserver.h>
#include <rfile.h>

/*------------------------------------------------------------------------
 *  rfseek  --  seek to a specified position of a remote file
 *------------------------------------------------------------------------
 */
rfseek(devptr, offset)
struct	devsw	*devptr;
long	offset;
{
	struct	rfblk	*rfptr;

	rfptr = (struct rfblk *)devptr->dvioblk;
	wait(rfptr->rf_mutex);
	rfptr->rf_pos = offset;
	signal(rfptr->rf_mutex);
	return(OK);
}
