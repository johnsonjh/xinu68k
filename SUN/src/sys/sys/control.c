/* control.c - control */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  control  -  control a device (e.g., set the mode)
 *------------------------------------------------------------------------
 */
SYSCALL	control(descrp, func, addr, addr2)
int descrp, func;
char *addr,*addr2;
{
	struct	devsw	*devptr;

	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvcntl)(devptr, func, addr, addr2) );
}
