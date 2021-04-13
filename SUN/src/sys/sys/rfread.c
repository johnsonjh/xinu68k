/* rfread.c - rfread */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rfread  --  read one or more bytes from a remote file
 *------------------------------------------------------------------------
 */
rfread(devptr, buff, len)
struct	devsw	*devptr;
char	*buff;
int	len;
{
	return( rfio(devptr, FS_READ, buff, min(len,RDATLEN) ) );
}
