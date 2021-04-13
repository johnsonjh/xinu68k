/* netout.c - netout */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#define  MNAMLEN  24		/* maximum size of this machine's name	*/

/*------------------------------------------------------------------------
 *  netout  -  start network by finding address and forward IP packets
 *------------------------------------------------------------------------
 */
PROCESS	netout(userpid, icmpp)
int	userpid;
int	icmpp;
{
	struct	epacket	*packet;
	struct	ip	*ipptr;
	long	tim;
	int	len;
	char	nam[MNAMLEN];
	IPaddr	addr;

	getaddr(addr);
	gettime(&tim);
	getname(nam);

#ifdef PRINTADDRESS	
	kprintf("Phys. address:  %02x:%02x:%02x:%02x:%02x:%02x\n",
		eth[0].etpaddr[0]&0xff,
		eth[0].etpaddr[1]&0xff,
		eth[0].etpaddr[2]&0xff,
		eth[0].etpaddr[3]&0xff,
		eth[0].etpaddr[4]&0xff,
		eth[0].etpaddr[5]&0xff);
	
	kprintf("IP address %d.%d.%d.%d\n",
		(unsigned short) Net.myaddr[0]&0xff,
		(unsigned short) Net.myaddr[1]&0xff,
		(unsigned short) Net.myaddr[2]&0xff,
		(unsigned short) Net.myaddr[3]&0xff);
#endif
	resume(userpid);
	while (TRUE) {
		packet = (struct epacket *) preceive(icmpp);
		ipptr = (struct ip *) packet->ep_data;
		blkcopy (addr, ipptr->i_dest, IPLEN);
		len = net2hs(ipptr->i_paclen) - IPHLEN;
		ipsend(addr, packet, len);
	}
}
