/* icmp.h */

/* Internet Control Message Protocol Constants and Packet Format */

#define	ICRPECH	0			/* Echo reply			*/
#define	ICDESTU	3			/* Destination unreachable	*/
#define	ICSRCQN	4			/* Source quench		*/
#define	ICREDIR	5			/* Redirect message type	*/
#define	ICRQECH	8			/* Echo request			*/
#define	ICTIMEX	11			/* Time exceeded		*/
#define	ICPARMP	12			/* Parameter Problem		*/
#define	ICTIMST	13			/* Timestamp message		*/
#define	ICTIMRP	14			/* Timestamp reply		*/
#define	ICRQINF	15			/* Information request		*/
#define	ICRPINF	16			/* Information reply		*/

/* ICMP packet format (part of the packet that follows the IP header)	*/

struct	icmp	{			/* ICMP packet			*/
	char	ic_typ;			/* type of message (see above)	*/
	char	ic_code;		/* code (often zero)		*/
	short	ic_cksum;		/* checksum of icmp header+data	*/
	short	ic_id;			/* for echo type, a message id	*/
	short	ic_seq;			/* for echo type, a seq. number	*/
	char	ic_data[1];		/* data area of ICMP message	*/
};
