
/* fgetreq.c - fgetreq */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fgetreq  -  read a request packet from network and return its length
 *------------------------------------------------------------------------
 */
fgetreq(sock, fpacptr, fromptr, fromlen)
int	sock;
struct	fpacket	*fpacptr;
struct	sockaddr *fromptr;
int	*fromlen;
{
	int	len;
	struct	fphdr	*fptr;

	fptr = &fpacptr->fp_h;
	while ( (len=recvfrom(sock, fpacptr, sizeof(*fpacptr), 0,
		fromptr, fromlen)) < 0)
		if (errno != EINTR)
			return(SYSERR);

	/* Convert from network byte order to host byte order */

	fptr->f_pos = ntohl(fptr->f_pos);
	fptr->f_op = ntohs(fptr->f_op);
	fptr->f_count = ntohs(fptr->f_count);

	return(len);
}
