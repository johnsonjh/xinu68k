/* makepack.c */

/* This procedure fills in a packet once we know all the info	*/
/* that has to go in it.  It really only makes 2 types of	*/
/* packets.							*/

#include "tftpload.h"
#include "ether.h"
#include "ip.h"
#include "udp.h"
#include "network.h"


makepack(newpack, myeaddr, myaddr, myport, desteaddr, destaddr,
	 destport, packid)
     char *newpack;			/* ack packet to be filled in	*/
     Eaddr myeaddr;			/* my ethernet address		*/
     IPaddr myaddr;			/* my IP address		*/
     short myport;			/* my udp port (src udp port)	*/
     Eaddr desteaddr;			/* eth address to send to	*/
     IPaddr destaddr;			/* IP address to send to	*/
     short destport;			/* udp port to send ack to	*/
     short packid;			/* packet id			*/
{
	struct epacket *packet;		/* ptr to ack packet		*/
	struct ip *ipptr;		/* ptr to ip packet portion	*/
	struct udp *udpptr;		/* ptr to udp packet portion	*/

	packet = (struct epacket *) newpack;
	ipptr = (struct ip *)packet->ep_data;
	udpptr = (struct udp *)ipptr->i_data;

	udpptr->u_sport = hs2net(myport);
	udpptr->u_dport = hs2net(destport);
	udpptr->u_ucksum = 0;

	ipptr->i_proto = IPRO_UDP;
	ipptr->i_verlen = IVERLEN;
	ipptr->i_svctyp = ISVCTYP;
	ipptr->i_id = hs2net(packid);
	ipptr->i_fragoff = hs2net(IFRAGOFF);
	ipptr->i_tim2liv = ITIM2LIV;

	blkcopy(ipptr->i_src, myaddr, IPLEN);
	blkcopy(ipptr->i_dest, destaddr, IPLEN);

	packet->ep_hdr.e_ptype = hs2net(EP_IP);
	blkcopy(packet->ep_hdr.e_dest, desteaddr, EPADLEN);
	blkcopy(packet->ep_hdr.e_src, myeaddr, EPADLEN);
}
