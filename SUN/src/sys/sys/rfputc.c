/* rfputc.c - rfputc */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rfputc  --  put a single character into a remote file
 *------------------------------------------------------------------------
 */
rfputc(devptr, ch)
struct	devsw	*devptr;
char	ch;
{
	char	outch;

	outch = ch;
	if ( write(devptr->dvnum, &outch, 1) == 1)
		return(OK);
	else
		return(SYSERR);
}
