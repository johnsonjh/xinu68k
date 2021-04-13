/* rename.c - rename */

#include <conf.h>
#include <kernel.h>
#include <file.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  rename  -  rename a file (key is optional)
 *------------------------------------------------------------------------
 */
SYSCALL	rename(old, new)
char	*old;
char	*new;
{
	char	fullold[NAMLEN];
	char	fullnew[NAMLEN];
	struct	devsw	*devptr;
	int	dev, dev2;

	/* map names through namespace and restrict to single device */

	if ( (dev = nammap(old, fullold)) == SYSERR ||
	     (dev2= nammap(new, fullnew)) == SYSERR ||
	      dev != dev2)
			return(SYSERR);
	devptr = &devtab[dev];
	return(	(*devptr->dvcntl)(devptr,FLRENAME,fullold,fullnew) );
}
