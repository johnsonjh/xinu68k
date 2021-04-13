/* rfgetc.c - rfgetc */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rfgetc  --  get a character from a remote file
 *------------------------------------------------------------------------
 */
rfgetc(devptr)
struct	devsw	*devptr;
{
	char	ch;
	int	retcode;

	if ( (retcode=read(devptr->dvnum, &ch, 1)) == 1)
		return(ch);
	else if (retcode != EOF)
		retcode = SYSERR;
	return(retcode);
}
