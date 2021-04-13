/* fsmkdir.c - fsmkdir */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsmkdir  -  handle mkdir request and prepare reply
 *------------------------------------------------------------------------
 */
fsmkdir(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;

	if (mkdir(fptr->f_name, fptr->f_count) < 0)
		fptr->f_op = FS_ERROR;
	else
		Uncache(fptr->f_name);
	return(FHDRLEN);
}
