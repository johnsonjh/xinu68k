
/* 
 * bed.h - Main header file for Distributed Odt/Download programs and routines
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 16:01:48 1988
 *
 * Copyright (c) 1988/* Jim Griffioen
 */

/*
 *---------------------------------------------------------------------------
 * These are site specific constants and should be modified to work with
 * your machines and file system
 *---------------------------------------------------------------------------
 */

#define BED_EXEC_PATH	"/u5/jng/503/network/sunbin/bed"
					/* full bed program name	*/
#define BASEDIR		"/tmp/xinu/"	/* basedir for lock files	*/
#define IN_FILE		"/usr/xinu/database/machines"
					/* machines database name	*/
#define BEDSERVERS	"/usr/xinu/database/bedservers"
					/* list of bedservers on the net*/
#define DEFAULTCLASS	"SUN"		/* default class		*/
#define KNOWN_UDP_PORT	2024		/* port bed will listen on	*/
#define MAXNETPACKET	1500		/* low est of network packet size */
#define APPROXUDPHEAD	200		/* hi est of eth/ip/udp header size */
#define STATTIMEOUT	2		/* status timeout set to 2 seconds */
    
/***** End site specific constants ******/


#define MAXCMDLEN	4		/* max command length		*/
#define MAXUID		16		/* max user id length		*/
#define CURVER		'A'		/* current version character	*/
#define TTYLENGTH	24		/* max tty name length		*/
#define MAXMACHINENAME	32		/* max machine name length	*/
#define LOADLEN		32		/* load average string len	*/
#define CLASSLEN	8		/* len of class string		*/
#define IDLELEN		16		/* len of idle time string	*/
#define MAXDETAIL	(MAXNETPACKET - APPROXUDPHEAD - MAXCMDLEN - MAXUID)
					/* remainer of the packet for	*/
					/* protocols of higher level 	*/
					/* bm (bend machine) protocol	*/
#define MAXBE	(MAXDETAIL-sizeof(struct festatus))/sizeof(struct bestatus)
					/* max # of backend machines 	*/
					/* allowed - due to packet size	*/
#define NOBODY		"nobody"	/* no user			*/
#define	NOWHERE		"nowhere"	/* no machine			*/
#define	RESERVETIME	10		/* minutes a reservation lasts	*/
#define REALLYDIE	3		/* bed return code to really die*/
#define RESTART		4		/* reboot the BED from scratch	*/


/* UDP datagram format for requests and responses to the b.e.d */
struct bm {
	char ver;			/* version number of this structure */
	char cmd[MAXCMDLEN];		/* command/reply code		*/
	char detail[MAXDETAIL];		/* details about the request	*/
    };

# define lbm (sizeof (struct bm))

/* everything is in character format so it is machine independent */
struct festatus {			/* front-end status		*/
	char fename[MAXMACHINENAME];	/* name of front end machine	*/
	char numusers[4];		/* number of users on front end	*/
	char loadav[LOADLEN];		/* load average on front end 	*/
	char numbends[4];		/* number of backends		*/
};

struct bestatus {
	char bename[MAXMACHINENAME];	/* name of backend machine	*/
	char ttyname[TTYLENGTH];	/* name tty it is attached to	*/
	char class[CLASSLEN];		/* class bitmask for machine	*/
	char idle[IDLELEN];		/* amount of idle time		*/
	char user[MAXUID];		/* user who holds the lock	*/
};

struct statresp {			/* status response		*/
	struct festatus	festatus;	/* front end status		*/
	struct bestatus	bestatus[MAXBE]; /* back end status structureS	*/
};

struct lockreq {			/* lock request structure	*/
	char uid[MAXUID];		/* user id of requester		*/
	char hostid[MAXMACHINENAME];	/* host name user is on		*/
	char bename[MAXMACHINENAME];	/* backend machine requested	*/
};

struct genericreq {			/* odt and download request struct */
	char uid[MAXUID];		/* user id of requester		*/
	char hostid[MAXMACHINENAME];	/* host name user is on		*/
	char bename[MAXMACHINENAME];	/* backend machine requested	*/
};

struct option {				/* options used by all programs	*/
	char verbose;			/* verbose option		*/
	char classname[CLASSLEN];	/* class of backend machine	*/
	char bemachname[MAXMACHINENAME]; /* backend machine to use	*/
};

/* request and response codes */
#define RESP_OK			1
#define RESP_ERR		2
#define RESP_PERMISSION		3

#define REQ_STATUS		4

#define REQ_LOCK		5
#define RESP_LOCK_REFUSED	6
#define RESP_MACHINE_UNKNOWN	7

#define REQ_UNLOCK		8

#define REQ_ODT_CONNECT		9
#define RESP_ODT_RUNNING	10

#define REQ_QUIT		11
#define REQ_REBOOT		12

#define REQ_DNLD_CONNECT        13
#define RESP_DNLD_RUNNING       14

/* procedure return status */
#define SYSOK			1
#define	SYSERR			2


/* external function definitions */
extern struct bestatus *getbestatus();	/* parse and return a backend status */

/* external variables */
extern struct option opt;
