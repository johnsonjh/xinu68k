/* dgwrite.c - dgwrite */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgwrite  -  write one datagram to a datagram protocol pseudo-device
 *------------------------------------------------------------------------
 */
dgwrite(devptr, buff, len)
struct	devsw	*devptr;
struct	xgram	*buff;
int	len;
{
	struct	epacket	*packet;
	struct	ip	*ipptr;
	struct	udp	*udpptr;
	struct	dgblk	*dgptr;
	int	dstport;
	char	*dstIP;

	if (len < 0 || len > UMAXLEN)
		return(SYSERR);
	dgptr = (struct dgblk *) devptr->dvioblk;
	packet = (struct epacket *) getbuf(Net.netpool);
	ipptr = (struct ip *) packet->ep_data;
	udpptr = (struct udp *) ipptr->i_data;
	dstport = dgptr->dg_fport;
	dstIP = (char *) dgptr->dg_faddr;
	if ( (dgptr->dg_mode & DG_NMODE) != 0) {
		if (dstport == 0) {
			dstport = buff->xg_fport;
			dstIP = (char *) buff->xg_faddr;
		}
		blkcopy(udpptr->u_data, buff->xg_data, len);
	} else {
		if ( dstport == 0) {
			freebuf(packet);
			return(SYSERR);
		}
		blkcopy(udpptr->u_data, buff, len);
	}
	return( udpsend(dstIP, dstport,	dgptr->dg_lport, packet, len) );
}
