/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <deqna.h>
#include <ether.h>
#include <ip.h>
#include <net.h>
#include <arp.h>
#include <ctype.h>
#include <hosts.h>

#define	UDPECHO		7		/* UDP echo port number		*/
#define	LPORT		2000		/* local udp port for reply	*/
#define	NMSGS		6		/* number of possible messages	*/

IPaddr	gateway;
IPaddr	defaddr;

char	*msgs[NMSGS] =
		{"1 - Send echo packet\n",
		"2 - Clear address cache\n",
		"3 - Dump address cache\n",
		"4 - Change default address\n",
		"5 - Change gateway address\n",
		"6 - Toggle RWHO packet printing\n"};
int	prrwho;

/*------------------------------------------------------------------------
 *  main  --  Try to reach echo servers on remote machines
 *------------------------------------------------------------------------
 */
main()
{
	char	ch;
	int	i;
	int	cindex;

	dtoIP(&gateway, MORDRED);
	dtoIP(&defaddr, MERLIN);
	control(NETWORK, NC_SETGW, &gateway);
	printf("\nUDP test.  Gateway = %d.%d.%d.%d  Dest = %d.%d.%d.%d\n",
		gateway[0] & 0377, gateway[1] & 0377,
		gateway[2] & 0377, gateway[3] & 0377,
		defaddr[0] & 0377, defaddr[1] & 0377,
		defaddr[2] & 0377, defaddr[3] & 0377);
	while (TRUE) {
		printf("\nSelect one of the following\n");
		for (i=0 ; i<NMSGS ; i++)
			printf("  %s",msgs[i]);
		while ( (ch=getc(CONSOLE)) == ' ')
			;
		cindex = ch - '0';
		while (ch != '\n')
			ch = getc(CONSOLE);
		if ( cindex<=0 || cindex>NMSGS)
			continue;

		switch (cindex) {

		 case 1:
			sndecho();
			break;

		 case 2: 
			ARP.atabnxt = ARP.atabsiz = 0;

		 case 3:
		 	adump();
			break;

		 case 4:
			getaddr(defaddr);
			break;

		 case 5:
			getaddr(gateway);
			control(NETWORK, NC_SETGW, &gateway);
			break;

		 case 6:
		 	if (prrwho) {
				printf("printing turned off\n");
				prrwho = FALSE;
			} else {
				printf("printing turned on\n");
				prrwho = TRUE;
			}
			break;

		 default:
		 	panic("test - can't happen");
		}
	}
}

/*------------------------------------------------------------------------
 *  sndecho  -  send an echo packet
 *------------------------------------------------------------------------
 */
sndecho()
{
	char	ch;
	char	*cptr;
	int	len;
	IPaddr	faddr;
	struct	epacket	*packet;

	/* set up specific gateway address */

	packet = (struct epacket *) getbuf(Net.netpool);
	blkcopy(faddr, defaddr, IPLEN);
	getaddr(faddr);
	cptr = (char *) (( (struct udpip *)packet->ep_data)->u_data);
	printf("\nType a one-line message to send out: ");
	for (len=1 ; (ch=getc(CONSOLE)) != '\n' ; len++)
		*cptr++ = ch;
	*cptr = '\n';
	printf("Sending datagram with %d data bytes\n", len);
	udpsend(faddr, UDPECHO, LPORT, packet, len);
	return(OK);
}

/*------------------------------------------------------------------------
 *  getaddr  -  get network address (dot notation) from CONSOLE input
 *------------------------------------------------------------------------
 */
getaddr(ip)
char	*ip;
{
	int	i, byte;
	char	ch;
	IPaddr	tmp;
	char	*tp;

	printf("\nType destination net address like %d.%d.%d.%d: ",
		ip[0] & 0377, ip[1] & 0377,
		ip[2] & 0377, ip[3] & 0377);
	if ( (ch = getc(CONSOLE) ) == '\n' || !isdigit(ch)) {
		printf("Using address %d.%d.%d.%d\n",
			ip[0]&0377, ip[1]&0377,ip[2]&0377, ip[3]&0377);
		return(OK);
	}
	byte = ch - '0';
	tp = (char *)tmp;
	for (i=0 ; i<4 ; byte=0,i++) {
		while ( isdigit(ch=getc(CONSOLE)) )
			byte = 10*byte + (ch - '0');
		if (byte > 256 || (i<3 && ch!='.') ) {
			printf("Using address %d.%d.%d.%d\n",
			    ip[0]&0377, ip[1]&0377,ip[2]&0377, ip[3]&0377);
			return(OK);
		}
		*tp++ = (char)byte;
	}
	if (ch == '\n')
		blkcopy(ip, tmp, IPLEN);
	return(OK);
}
