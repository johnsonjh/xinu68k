/* control.c - control */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  control  -  control a device (e.g., set the mode)
 *------------------------------------------------------------------------
 */
control(descrp, func, addr, addr2)
int descrp, func;
char *addr,*addr2;
{
	struct	devsw	*devptr;

#ifdef	DEBUG
	dotrace("control", &descrp, 4);
#endif
	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvcntl)(devptr, func, addr, addr2) );
}
