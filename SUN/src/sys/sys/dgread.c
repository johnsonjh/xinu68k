/* dgread.c - dgread */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgread  -  read one datagram from a datagram protocol pseudo-device
 *------------------------------------------------------------------------
 */
dgread(devptr, buff, len)
struct	devsw	*devptr;
struct	xgram	*buff;
int	len;
{
	STATWORD ps;    
	struct	dgblk	*dgptr;
	struct	epacket	*packet;
	struct	udp	*udpptr;
	struct	ip	*ipptr;
	struct	netq	*nqptr;
	int	datalen;

	disable(ps);
	dgptr = (struct dgblk *)devptr->dvioblk;
	if (dgptr->dg_mode & DG_TMODE) {
		nqptr = &Net.netqs[dgptr->dg_netq];
		if ( !isbadpid(nqptr->pid) ) {
			restore(ps);
			return(SYSERR);
		}
		if (pcount(dgptr->dg_xport) <= 0) {
			nqptr->pid = getpid();
			if (recvtim(DG_TIME) == TIMEOUT) {
				nqptr->pid = BADPID;
				restore(ps);
				return(TIMEOUT);
			}
		}
	}
	packet = (struct epacket *) preceive(dgptr->dg_xport);

	/* copy data into user's buffer & set length */

	ipptr = (struct ip *) packet->ep_data;
	udpptr = (struct udp *)ipptr->i_data;
	datalen = net2hs(udpptr->u_udplen) - UHLEN;
	if (dgptr->dg_mode & DG_NMODE) {
		if ( (datalen+XGHLEN) > len) {
			freebuf(packet);
			restore(ps);
			return(SYSERR);
		}
		blkcopy(buff->xg_faddr, ipptr->i_src, IPLEN);
		buff->xg_fport = net2hs(udpptr->u_sport);
		buff->xg_lport = dgptr->dg_lport;
		blkcopy(buff->xg_data, udpptr->u_data, datalen);
	} else {
		if (datalen > len)
			datalen = len;
		blkcopy(buff, udpptr->u_data, datalen);
	}
	freebuf(packet);
	restore(ps);
	return(datalen);
}
