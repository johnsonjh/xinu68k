/* fstrans.c - fstrans */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fstrans  -  handle transaction request and prepare reply
 *------------------------------------------------------------------------
 */
fstrans(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;

	fptr = &fpacptr->fp_h;
	fptr->f_op = FS_ERROR;		/* presently, no transactions */
	return(FHDRLEN);
}
