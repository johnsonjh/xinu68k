/* arp.h */

/* DARPA Internet Address Resolution Protocol  (see RFCs 826, 920) */

#define	AR_HRD	1		/* Ethernet hardware type code		*/
#define	AR_PROT	0x0800		/* IP protocol address code		*/
#define	AR_HLEN	6		/* Ethernet physical address length	*/
#define	AR_PLEN	4		/* IP Protocol address length		*/

/* Definitions of codes used in operation field of ARP packet */
	
#define	AR_REQ	1		/* arp request to resolve address	*/
#define	AR_RPLY	2		/* reply to a resolve request		*/
#define	AR_RREQ	3		/* reverse ARP request (RARP packets)	*/
#define	AR_RRLY	4		/* reply to a reverse request (RARP ")	*/

struct	arppak	{		/* format of DARPA ARP packet		*/
	short	ar_hrd;		/* type of hardware (Ethernet = 1)	*/
	short	ar_prot;	/* format of proto. address (IP=0x0800)	*/
	char	ar_hlen;	/* hardware address length (6 for Ether)*/
	char	ar_plen;	/* protocol address length (4 for IP)	*/
	short	ar_op;		/* arp operation (see list above)	*/
	Eaddr	ar_sha;		/* sender's physical hardware address	*/
	IPaddr	ar_spa;		/* sender's protocol address (IP addr.)	*/
	Eaddr	ar_tha;		/* target's physical hardware address	*/
	IPaddr	ar_tpa;		/* target's protocol addresss (IP)	*/
};
