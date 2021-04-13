/* remove.c - remove */

#include <conf.h>
#include <kernel.h>
#include <file.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  remove  -  remove a file given its name (key is optional)
 *------------------------------------------------------------------------
 */
SYSCALL	remove(name, key)
char	*name;
int	key;
{
	char	fullnam[NAMLEN];
	struct	devsw	*devptr;
	int	dev;

	if ( (dev=nammap(name, fullnam)) == SYSERR)
		return(SYSERR);
	devptr = &devtab[dev];
	return(	(*devptr->dvcntl)(devptr, FLREMOVE, fullnam, key) );
}
