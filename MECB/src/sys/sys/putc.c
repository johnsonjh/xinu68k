/* putc.c - putc */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  putc  -  write a single character to a device
 *------------------------------------------------------------------------
 */
putc(descrp, ch)
int descrp;
char ch;
{
	struct	devsw	*devptr;

#ifdef	DEBUG
	dotrace("putc", &descrp, 2);
#endif
	if (isbaddev	(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvputc)(devptr,ch) );
}
