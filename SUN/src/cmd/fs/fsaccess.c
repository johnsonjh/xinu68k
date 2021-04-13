/* fsaccess.c - fsaccess */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsaccess  -  handle access request and prepare reply
 *------------------------------------------------------------------------
 */
fsaccess(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;

	fptr = &fpacptr->fp_h;
	fptr->f_op = FS_ERROR;
	return(FHDRLEN);
}
