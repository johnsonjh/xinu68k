/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <deqna.h>
#include <ether.h>
#include <ip.h>
#include <arp.h>

/*------------------------------------------------------------------------
 *  main  --  test ethernet by reading packets
 *------------------------------------------------------------------------
 */
main()
{
	char	buff[1600];
	struct	xx	{
		struct	eheader	ethead;
		struct	arppak	arpout;
	} outpak;
	int	i, iterate, len;
	char	*p, ch;
	struct	arppak	*arpptr;
	struct	epacket	*epkptr;

	/* call buffer pool stuff so 'freebuf' can be mentioned */
	/* without generating a load-time error */
	mkpool(10,10);

	printf("Read %d packets from Ethernet\n", iterate=2);
	epkptr = (struct epacket *)buff;
	while (iterate--) {
	    len = read(ETHER, buff, 1500);
	    epkptr->ep_hdr.e_ptype = netorder(epkptr->ep_hdr.e_ptype);
	    if (len == SYSERR) {
		kprintf("\nError...\n");
	    } else if (epkptr->ep_hdr.e_ptype == EP_ARP) {
		/* do ARP */ kprintf("ARP...\n");
	    } else {
		p = (char *)buff;
		kprintf("\n TYP: %d\n",epkptr->ep_hdr.e_ptype);
		kprintf(" LEN: %d\n  TO: ",len);
		for (i=0 ; i<6 ; i++)
			kprintf("%03o ", 0377& *p++);
		kprintf("\nFROM: ");
		for (i=0 ; i<6 ; i++)
			kprintf("%03o ", 0377& *p++);
		kprintf("\nTYPE: ");
		kprintf("%03o ", 0377 & *p++);
		kprintf("%03o ", 0377 & *p++);
		kprintf("\nDATA: ");
		for (i=0 ; i<60 ; i++) {
			ch = *p++;
			if ( ((ch >= 'a') && (ch <= 'z')) ||
			     ((ch >= 'A') && (ch <= 'Z')) )
				kprintf("%c",ch);
		}
		kprintf("\n");
	    }
	}

	kprintf("Preparing to transmit ARP request %d times...\n", iterate=2);
	/* make up packet */

	for (i=0 ; i<EPADLEN ; i++) {
		outpak.ethead.e_dest[i] = 0377;
	}
	blkcopy(outpak.ethead.e_src, eth[0].etpaddr, EPADLEN);
	outpak.ethead.e_ptype = netorder(EP_ARP);
	outpak.arpout.ar_hrd = netorder(AR_HRD);
	outpak.arpout.ar_prot = netorder(AR_PROT); 
	outpak.arpout.ar_hlen = AR_HLEN;
	outpak.arpout.ar_plen = AR_PLEN;
	outpak.arpout.ar_op = netorder(AR_REQ);
	for (i=0 ; i<EPADLEN ; i++) {
		outpak.arpout.ar_sha[i] = eth[0].etpaddr[i];
		outpak.arpout.ar_tha[i] = (char)0;
	}
	dot2ip(outpak.arpout.ar_spa,192,5,48,30);	/* xinutest1 */
	dot2ip(outpak.arpout.ar_tpa,192,5,48,23);	/* Lancelot*/
	epkptr = (struct epacket *) buff;
	arpptr = (struct arppak *) epkptr->ep_data;
	while (iterate--) {
		if ( (len=sizeof(struct xx)) < EMINPAK)
			len = EMINPAK;
		kprintf("Sending %d byte packet\n", len);
		if (write(ETHER, &outpak,len) == SYSERR)
			kprintf("\nTE\n");
		len = read(ETHER, buff,1500);
		epkptr->ep_hdr.e_ptype = netorder(epkptr->ep_hdr.e_ptype);
		if (len == SYSERR) {
			kprintf("\nRE\n");
		} else if (epkptr->ep_hdr.e_ptype == EP_ARP) {
			kprintf("ARP: len=%d\n",len);
			kprintf(" FR: %o %o %o %o %o %o\n",
				epkptr->ep_hdr.e_src[0] &0377,
				epkptr->ep_hdr.e_src[1] &0377,
				epkptr->ep_hdr.e_src[2] &0377,
				epkptr->ep_hdr.e_src[3] &0377,
				epkptr->ep_hdr.e_src[4] &0377,
				epkptr->ep_hdr.e_src[5] &0377);
			kprintf(" TO: %o %o %o %o %o %o\n",
				epkptr->ep_hdr.e_dest[0] &0377,
				epkptr->ep_hdr.e_dest[1] &0377,
				epkptr->ep_hdr.e_dest[2] &0377,
				epkptr->ep_hdr.e_dest[3] &0377,
				epkptr->ep_hdr.e_dest[4] &0377,
				epkptr->ep_hdr.e_dest[5] &0377);
			kprintf(" OP: %d\n SH: %o %o %o %o %o %o \n",
				netorder(arpptr->ar_op),
				arpptr->ar_sha[0] &0377,
				arpptr->ar_sha[1] &0377,
				arpptr->ar_sha[2] &0377,
				arpptr->ar_sha[3] &0377,
				arpptr->ar_sha[4] &0377,
				arpptr->ar_sha[5] &0377);
			kprintf(" SP: %d.%d.%d.%d\n",
				arpptr->ar_spa[0] &0377,
				arpptr->ar_spa[1] &0377,
				arpptr->ar_spa[2] &0377,
				arpptr->ar_spa[3] &0377);
			kprintf(" TH: %o %o %o %o %o %o\n TP: %d.%d.%d.%d\n",
				arpptr->ar_tha[0] &0377,
				arpptr->ar_tha[1] &0377,
				arpptr->ar_tha[2] &0377,
				arpptr->ar_tha[3] &0377,
				arpptr->ar_tha[4] &0377,
				arpptr->ar_tha[5] &0377,
				arpptr->ar_tpa[0] &0377,
				arpptr->ar_tpa[1] &0377,
				arpptr->ar_tpa[2] &0377,
				arpptr->ar_tpa[3] &0377);
		} else {
			kprintf("Typ=%o\n",netorder(epkptr->ep_hdr.e_ptype));
			iterate++;
		}
	}		
}

dot2ip(ip, b1, b2, b3, b4)
char	*ip;
int	b1, b2, b3, b4;
{
	*ip++ = LOWBYTE & b1;
	*ip++ = LOWBYTE & b2;
	*ip++ = LOWBYTE & b3;
	*ip++ = LOWBYTE & b4;
}


netorder(i)
int	i;
{
	char c;

	return( ((i&0377)<<8) | ((i>>8)&0377) );
}


blkcopy(to, from, bytes)
char	*from;
char	*to;
int	bytes;
{
	while (bytes-- > 0)
		*to++ = *from++;
}
