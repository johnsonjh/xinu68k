/* ip.h */

/* DARPA Internet Protocol (IP)  Constants and Datagram Format */

#define	IPLEN	4		/* IP address length in bytes (octets)	*/
typedef	char	IPaddr[IPLEN];	/* DARPA internet address		*/
#define	IPTMASK	0300		/* Mask for IP type in top byte of addr	*/
#define	IPATYP	0000		/* IP Class A address (1st 2 bits)	*/
#define	IPBTYP	0200		/* IP Class B address (1st 2 bits)	*/
#define	IPCTYP	0300		/* IP Class C address (1st 2 bits)	*/
#define	IPHLEN	20		/* IP datagram header length in bytes	*/
#define	IMAXLEN	1200		/* Maximum IP datagram length (Notes:	*/
				/*  1) hosts & gateways only required	*/
				/*  to recognize 576 bytes. 2) must be	*/
				/*  less than EMAXPAK for our network	*/
/* Assigned Protocol numbers */

#define	IPRO_ICMP	1	/* protocol type for ICMP packets	*/
#define	IPRO_UDP	17	/* protocol type for UDP packets	*/

struct	ip	{		/* Format of DARPA Internet datagram	*/
	char	i_verlen;	/* IP vers.(0x40) + hdr len in longs (5)*/
	char	i_svctyp;	/* service type (0 => normal service)	*/
	short	i_paclen;	/* packet length in octets (bytes)	*/
	short	i_id;		/* datagram id (to help gateways frag.)	*/
	short	i_fragoff;	/* fragment offset (0 for 1st fragment) */
	char	i_tim2liv;	/* time to live in gateway hops (10)	*/
	char	i_proto;	/* IP protocol (UDP is assigned 17)	*/
	short	i_cksum;	/* 1s compl. of sum of shorts in header	*/
	IPaddr	i_src;		/* IP address of source			*/
	IPaddr	i_dest;		/* IP address of destination		*/
	char	i_data[1];	/* IP datagram data area		*/
};

/* Datagram field constants used by simple IP software */

#define	IVERLEN		0x45	/* current version length field value	*/
#define	ISVCTYP		'\0'	/* service type for normal service	*/
#define	IFRAGOFF	0	/* Fragment offset (0 => no fragment)	*/
#define	ITIM2LIV	10	/* Initial time-to-live value		*/

extern	int	ipackid;	/* Internet datagram id			*/
