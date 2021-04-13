/* write.c - write */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  write  -  write 1 or more bytes to a device
 *------------------------------------------------------------------------
 */
write(descrp, buff, count)
	int descrp, count;
	char *buff;
{
	struct	devsw	*devptr;

#ifdef	DEBUG
	dotrace("write", &descrp, 3);
#endif
	if (isbaddev(descrp) )
		return(SYSERR);
	devptr = &devtab[descrp];
	return(	(*devptr->dvwrite)(devptr,buff,count) );
}
