/* rwho.h */

/* Data structures and constants for RWHO packet processing */

#define	RMACLEN		16		/* length of machine name	*/
#define	RWNLOAD		3		/* number of "load averages"	*/
#define	RWNLEN		8		/* name length in rwho packet	*/
#define	RWCSIZ		50		/* size of rwho cache		*/
#define	RWMAXP		600		/* max packet length accepted	*/
#define	RWMINP		60		/* size of rwho pac, no users	*/
#define	RWDELAY		180		/* delay for output in seconds	*/
#define	RWCDOWN		200		/* consider down if this old	*/
#define	RWMAXDT		(60L*60L*24L*7L) /* maximum down time before	*/
					/* machine removed from cache	*/
#define	RWIN		rwhoind		/* Rwho input process code	*/
#define	RWISTK		500		/* Rwho input process stack	*/
#define	RWIPRIO		20		/* Rwho input process priority	*/
#define	RWINAM		"rwho-in"	/* Rwho input process name	*/
#define	RWIARGS		0		/* Rwho input process arguments	*/
#define	RWOUT		rwhod		/* Rwho output process code	*/
#define	RWOSTK		500		/* Rwho output process stack	*/
#define	RWOPRIO		20		/* Rwho output process priority	*/
#define	RWONAM		"rwhod"		/* Rwho output process name	*/
#define	RWOARGS		0		/* Rwho output process arguments*/

extern	int	RWIN(), RWOUT();

struct	rwent	{			/* rwho cache entry		*/
	char	rwmach[RMACLEN];	/* Name of machine that is up	*/
	long	rwboot;			/* when machine was booted	*/
	long	rwlast;			/* Local time last packet recvd	*/
	long	rwslast;		/* Sender's time in last packet	*/
	int	rwload[3];		/* Load averages as in uptime	*/
	int	rwusers;		/* Number of users logged in	*/
};

struct	rwinfo	{			/* all rwho information		*/
	struct	rwent	rwcache[RWCSIZ];/* cache of received info	*/
	int	rwnent;			/* number of valid cache entries*/
	Bool	rwsend;			/* send out rwho packets?	*/
	long	rwbtime;		/* time I was booted		*/
	char	rbuf[RWMAXP];		/* to hold input packets	*/
};

extern	struct	rwinfo	Rwho;

/* Declarations that describe the format of rwho packets on the net	*/

struct	rwhopac	{			/* format of rwho packet	*/
	char	rw_vers;		/* protocol version number	*/
	char	rw_type;		/* packet type			*/
	char	rw_pad[2];
	long	rw_sndtim;		/* sender's time stamp		*/
	long	rw_rtim;		/* receiver's time stamp	*/
	char	rw_host[32];		/* sending host's name		*/
	long	rw_load[3];		/* load averages		*/
	long	rw_btim;		/* boot time of sender		*/
	struct	rw_who	{
		char	rw_tty[RWNLEN];	/* UNIX tty name		*/
		char	rw_nam[RWNLEN];	/* user's name			*/
		long	rw_ton;		/* time user logged on		*/
		long	rw_idle;	/* user's idle time		*/
	} rw_rww[1024 / sizeof(struct rw_who)];
};

#define	RWVERSION	1		/* protocol version number	*/
#define	RWSTATUS	1		/* host status			*/
