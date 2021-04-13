/* rfmkpac.c - rfmkpac */

#include <conf.h>
#include <kernel.h>
#include <network.h>

static	struct	fpacket	packet;

/*------------------------------------------------------------------------
 *  rfmkpac  --  make a remote file request packet and send it
 *------------------------------------------------------------------------
 */
rfmkpac(rop, rname, rpos, buff, len)
int	rop;
char	*rname;
long	*rpos;
char	*buff;
int	len;
{
	struct	fphdr	*fptr;
	int	reqlen, rplylen;

	wait(Rf.rmutex);
	fptr = (struct fphdr *) &packet;
	fptr->f_op = hs2net(rop);
	fptr->f_pos = hl2net(*rpos);
	fptr->f_count = hs2net(len);
	strncpy(fptr->f_name, rname, RNAMLEN);
	reqlen = rplylen = FPHLEN + len;
	switch (rop) {

	    case FS_WRITE:
	    case FS_RENAME:
		if (len > RDATLEN) {
			signal(Rf.rmutex);
			return(SYSERR);
		}
		blkcopy(packet.fpdata, buff, len);
		rplylen = FPHLEN;
		break;

	    case FS_CLOSE:
	    case FS_OPEN:
	    case FS_UNLINK:
	    case FS_MKDIR:
	    case FS_RMDIR:
	    case FS_ACCESS:
		rplylen = FPHLEN;
		/* fall through */
		
	    case FS_READ:
		if (len > RDATLEN) {
			signal(Rf.rmutex);
			return(SYSERR);
		}
		reqlen = FPHLEN;
		break;

	    default:
	    	;
	}
	if (rfsend(fptr, reqlen, rplylen) == SYSERR ||
	    net2hs(fptr->f_op) == FS_ERROR) {
		signal(Rf.rmutex);
		return(SYSERR);
	}
	switch (rop) {

	    case FS_READ:
		blkcopy(buff, packet.fpdata, len);
		/* fall through */

	    case FS_WRITE:
		*rpos = net2hl(fptr->f_pos);
		len = net2hs(fptr->f_count);
		break;

	    default:
		len = OK;
	}
	signal(Rf.rmutex);
	return(len);
}
