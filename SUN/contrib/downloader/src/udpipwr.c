/* udpipwr.c -- udpipwr */

#include <ether.h>
#include <ip.h>
#include <udp.h>
#include <tftpload.h>
#include <network.h>

/*---------------------------------------------------------------------------
 * udpipwr -- write a udp packet over the ethernet
 *---------------------------------------------------------------------------
*/
udpipwr(wpkt,  wpktlen)
struct	epacket	*wpkt;
int	wpktlen;
{
	register struct	ip *ipptr;
	struct udp *udpptr;

	ipptr = (struct ip *) wpkt->ep_data;
	udpptr = (struct udp *) ipptr->i_data;
	udpptr->u_udplen = hs2net(wpktlen-EHLEN-IPHLEN);
	ipptr->i_paclen = hs2net(wpktlen-EHLEN);
	ipptr->i_cksum = 0;
	ipptr->i_cksum = cksum(ipptr, (int) (IPHLEN / 2));
	ewrite(wpkt, wpktlen);
}
