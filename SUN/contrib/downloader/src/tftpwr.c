/* tftpwr.c - tftpwr */

#include <ether.h>
#include <ip.h>
#include <udp.h>
#include <tftp.h>
#include <network.h>


/*
 *=========================================================================
 *tftpwr() - this procedure writes out tftp packets.  It will write out any
 *           one of the following 3 types of tftp packets: RRQ, ACK, ERROR
 *=========================================================================
 */

tftpwr(op, blkorerr, fname, modeormsg, wpkt)
short	op;				/* tftp opcode			*/
short	blkorerr;			/* block # or error code	*/
char	*fname;				/* filename to request		*/
char	*modeormsg;			/* mode or message string	*/
struct	epacket	*wpkt;			/* write  bufs			*/
{
	struct ip *ipptr;
	struct udp *udpptr;
	struct tftp *tftpptr;
	char *str;
	long packlen;

	ipptr   = (struct ip *) wpkt->ep_data;
	udpptr  = (struct udp *) ipptr->i_data;
	tftpptr = (struct tftp *) udpptr->u_data;

	switch (op) {
	case TRRQ:
		tftpptr->op = hs2net(op);
		str = (char *) &(tftpptr->info.fname);
		while ((*str++ = *fname++) != '\0')
			/* empty */ ;
		while ((*str++ = *modeormsg++) != '\0')
			/* empty */ ;
		packlen = EHLEN + IPHLEN + UHLEN +
		          ((long) str - (long) tftpptr);
		udpipwr(wpkt, packlen);
		break;
	case TACK:
		tftpptr->op = hs2net(op);
		tftpptr->info.block = hs2net(blkorerr);
		packlen = EHLEN + IPHLEN + UHLEN + TFTPHDR;
		udpipwr(wpkt, packlen);
		break;
	case TERROR:
		tftpptr->op = hs2net(op);
		tftpptr->info.errcode = hs2net(blkorerr);
		str = (char *) tftpptr->data.errmsg;
		while ((*str++ = *modeormsg++) != '\0')
			/* empty */ ;
		packlen = EHLEN + IPHLEN + UHLEN + ((long)str - (long)tftpptr);
		udpipwr(wpkt, packlen);
		break;
	}
}


