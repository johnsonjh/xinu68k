/* fsrmdir.c - fsrmdir */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsrmdir  -  handle rmdir request and prepare reply
 *------------------------------------------------------------------------
 */
fsrmdir(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;

	fptr = &fpacptr->fp_h;
	if (rmdir(fptr->f_name) < 0)
		fptr->f_op = FS_ERROR;
	else
		Uncache(fptr->f_name);
	return(FHDRLEN);
}
