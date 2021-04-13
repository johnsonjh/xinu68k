/* fsndrply.c - fsndrply */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsndrply  -  send a reply packet to a  specified foreign address
 *------------------------------------------------------------------------
 */
fsndrply(s, fptr, len, toaddr, tolen)
int	s;				/* socket on which to write	*/
struct	fpacket	*fptr;			/* data to write		*/
int	len;				/* length of data to write	*/
struct	sockaddr *toaddr;		/* send to this Internet address*/
int	tolen;				/* length of address field	*/
{
	/* Convert inetgers from host byte order to network byte order	*/

	fptr->fp_h.f_op = htons(fptr->fp_h.f_op);
	fptr->fp_h.f_pos = htonl(fptr->fp_h.f_pos);
	fptr->fp_h.f_count = htons(fptr->fp_h.f_count);

	if (sendto(s, fptr, len, 0, toaddr, tolen) < 0)
		return(SYSERR);
	return(OK);
}
