/* ether.h */

/* Ethernet definitions and constants */

#define	EMINPAK	64		/* minimum packet length		*/
#define	EMAXPAK	1536		/* maximum size of an ethernet packet	*/
#define	EHLEN	14		/* size of Ethernet packet header	*/
#define	EDLEN	EMAXPAK-EHLEN	/* length of data field in ether packet	*/
#define	EPADLEN	6		/* number of octets in physical address	*/
typedef	char	Eaddr[EPADLEN];	/* length of physical address (48 bits)	*/
#define	EXRETRY	3		/* number of times to retry xmit errors	*/
#define	EBCAST	"\377\377\377\377\377\377"/* Ethernet broadcast address	*/
#define ETH_RTO 300		/* time out in seconds for reads	*/

#define ENUMRCV 8		/* number LANCE recv buffers (power 2)	*/
#define ENUMRCVL2 3		/* log2 of the number of buffers	*/     
     
struct	eheader	{		/* format of header in Ethernet packet	*/
	Eaddr	e_dest;		/* destination host address		*/
	Eaddr	e_src;		/* source host address			*/
	short	e_ptype;	/* Ethernet packet type (see below)	*/
};

#define	EP_LOOP	0x0060		/* packet type: Loopback		*/
#define	EP_ECHO	0x0200		/* packet type: Echo			*/
#define	EP_PUP	0x0400		/* packet type: Xerox PUP protocol	*/
#define	EP_IP	0x0800		/* packet type: DARPA Internet protocol	*/
#define	EP_ARP	0x0806		/* packet type: Address resolution  "	*/
#define	EP_RARP	0x8035		/* packet type: reverse  "  "       "	*/

struct	epacket	{		/* complete structure of Ethernet packet*/
	struct	eheader	ep_hdr;	/* packet header			*/
	char	ep_data[EDLEN];	/* data in the packet			*/
};

/* structure for handling a buffer ring entry for the LANCE */
struct bre {
    struct le_md *pmd;		/* pointer to the message descriptor	*/
    char *buf;			/* pointer to the buffer for this desc	*/
    int flags;			/* status of the buffer			*/
};


/* Ethernet control block descriptions */

struct	etblk	{
	struct	le_device *etle;/* address of device csr		*/
	struct	le_init_block *etib;/* initialization block		*/
	struct	bre etbrt[1];	/* ring descriptor for transmits	*/
	struct	bre etbrr[ENUMRCV];/* ring descriptor for receives	*/
	struct	devsw	*etdev;	/* Address of device switch table entry	*/
	Eaddr	etpaddr;	/* Ethernet physical device address	*/
	int	etrpid;		/* id of process reading from the ether	*/
	int	etwpid;		/* id of process writing to the ether	*/
	int	etrsem;		/* mutex for reading from the ethernet	*/
	int	etwsem;		/* mutex for writing to the ethernet	*/
	int	etlen;		/* length of packet being written or 0	*/
	int	etwtry;		/* num. of times to retry xmit errors	*/
	char	*etwbuf;	/* pointer to current transmit buffer	*/
	short	etnextbuf;	/* for checking buffers round robin	*/
	short	etrqnext[ENUMRCV];
	short	etrqhead;
	short	etrqtail;
};


/* ethernet function codes */

#define	ETHPROMON	1		/* turn on promiscuous mode	*/
#define	ETHPROMOFF	2		/* turn off promiscuous mode	*/

extern	struct	etblk	eth[];
