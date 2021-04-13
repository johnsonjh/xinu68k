/* fsunlink.c - fsunlink */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsunlink  -  handle unlink request and prepare reply
 *------------------------------------------------------------------------
 */
fsunlink(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;

	fptr = &fpacptr->fp_h;
	if (rmfile(fptr->f_name) < 0) {
		fptr->f_op = FS_ERROR;
	} else {
		Uncache(fptr->f_name);
	}
	return(flen);
}
