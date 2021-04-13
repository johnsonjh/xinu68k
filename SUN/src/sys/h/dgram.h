/* dgram.h */

/* Datagram pseudo-device control block */

struct	dgblk	{			/* Datagram device control block*/
	int	dg_dnum;		/* device number of this device	*/
	int	dg_state;		/* whether this device allocated*/
	short	dg_lport;		/* local datagram port number	*/
	short	dg_fport;		/* foreign datagram port number	*/
	IPaddr	dg_faddr;		/* foreign machine IP address	*/
	int	dg_xport;		/* incoming packet queue	*/
	int	dg_netq;		/* index of our netq entry	*/
	int	dg_mode;		/* mode of this interface	*/
};

/* Datagram psuedo-device state constants */

#define	DG_FREE		0		/* this device is available	*/
#define	DG_USED		1		/* this device is in use	*/

#define	DG_TIME		30		/* read timeout (tenths of sec)	*/

/* Constants for dgm pseudo-device control functions */

#define	DGM_GATE	11		/* Set the default gateway	*/

/* Constants for dg pseudo-device control functions */

#define	DG_SETMODE	1		/* Set mode of device		*/
#define	DG_CLEAR	2		/* clear all waiting datagrams	*/

/* Constants for dg pseudo-device mode bits */

#define	DG_NMODE	001		/* normal (datagram) mode	*/
#define	DG_DMODE	002		/* data-only mode		*/
#define	DG_TMODE	040		/* timeout all reads		*/

/* Structure of xinugram as dg interface delivers it to user */

struct	xgram	{			/* Xinu datagram (not UDP)	*/
	IPaddr	xg_faddr;		/* foreign host IP address	*/
	short	xg_fport;		/* foreign UDP port number	*/
	short	xg_lport;		/* local UDP port number	*/
	char	xg_data[UMAXLEN];	/* maximum data to/from UDP	*/
};

#define	XGHLEN	8	/* error in ( (sizeof(struct xgram)) - UMAXLEN)	*/

/* Constants for port specifications on INTERNET open call */

#define	ANYFPORT	(char *)0	/* Accept any foreign UDP port	*/
#define	ANYLPORT	0		/* Assign a fresh local port num*/

extern	struct	dgblk	dgtab[Ndg];
