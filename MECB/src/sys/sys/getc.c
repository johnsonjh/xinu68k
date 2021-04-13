/* getc.c - getc */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  getc  -  get one character from a device
 *------------------------------------------------------------------------
 */
getc(descrp)
int descrp;
{
	struct	devsw	*devptr;

#ifdef	DEBUG
	dotrace("getc", &descrp, 1);
#endif
	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
        return( (*devptr->dvgetc)(devptr) );
}
