/* getc.c - getc */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  getc  -  get one character from a device
 *------------------------------------------------------------------------
 */
SYSCALL	getc(descrp)
int descrp;
{
	struct	devsw	*devptr;

	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
        return( (*devptr->dvgetc)(devptr) );
}
