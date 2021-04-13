/* read.c - read */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  read  -  read 1 or more bytes from a device
 *------------------------------------------------------------------------
 */
read(descrp, buff, count)
int descrp, count;
char *buff;
{
	struct	devsw	*devptr;

#ifdef	DEBUG
	dotrace("read", &descrp, 3);
#endif
	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvread)(devptr,buff,count) );
}
