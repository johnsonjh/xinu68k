/* putc.c - putc */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  putc  -  write a single character to a device
 *------------------------------------------------------------------------
 */
SYSCALL	putc(descrp, ch)
int descrp;
char ch;
{
	struct	devsw	*devptr;
	
	if (isbaddev	(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvputc)(devptr,ch) );
}
