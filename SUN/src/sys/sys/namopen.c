/* namopen.c - namopen */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  namopen  -  open an object (e.g., remote file) based on the name
 *------------------------------------------------------------------------
 */
namopen(devptr, filenam, mode)
struct	devsw	*devptr;
char	*filenam;
char	*mode;
{
	int	dev;
	char	newname[NAMLEN];

	if ( (dev=nammap(filenam, newname)) == SYSERR)
		return(SYSERR);
	return( open(dev, newname, mode) );
}
