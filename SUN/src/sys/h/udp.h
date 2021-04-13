/* udp.h */

/* DARPA User Datagram Protocol (UDP) constants and formats */

#define	UHLEN	8		/* UDP header length in bytes		*/
#define	UMAXLEN	IMAXLEN-IPHLEN-UHLEN /* Maximum data in UDP packet	*/

struct	udp	{		/* Message format of DARPA UDP		*/
	short	u_sport;	/* Source UDP port number		*/
	short	u_dport;	/* Destination UDP port number		*/
	short	u_udplen;	/* Length of UDP data			*/
	short	u_ucksum;	/* UDP checksum (0 => no checksum)	*/
	char	u_data[UMAXLEN];/* Data in UDP message			*/
};

/* UDP constants */

#define	UPROTO		(char)17/* IP protocol type (17 => UDP)		*/
#define	ULPORT		2050	/* Initial UDP local "port" number	*/

/* Assigned UDP port numbers */

#define	UECHO		7	/* echo server				*/
#define	UDISCARD	9	/* discard packet			*/
#define	UUSERS		11	/* users server				*/
#define	UDAYTIME	13	/* day and time server			*/
#define	UQOTD		17	/* quote of the day server		*/
#define	UCHARGEN	19	/* character generator			*/
#define	UTIME		37	/* time server				*/
#define	UWHOIS		43	/* who is server (user information)	*/
#define	UDNAME		53	/* darpa domain name server		*/
#define	UTFTP		69	/* trivial file transfer protocol server*/
#define	URWHO		513	/* remote who server (ruptime)		*/
