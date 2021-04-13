/* conf.h (GENERATED FILE; DO NOT EDIT) */

#define	NULLPTR	(char *)0

/* Device table declarations */
struct	devsw	{			/* device table entry */
	int	dvnum;
	char	*dvname;
	int	(*dvinit)();
	int	(*dvopen)();
	int	(*dvclose)();
	int	(*dvread)();
	int	(*dvwrite)();
	int	(*dvseek)();
	int	(*dvgetc)();
	int	(*dvputc)();
	int	(*dvcntl)();
	int	dvcsr;
	int	dvivec;
	int	dvovec;
	int	(*dviint)();
	int	(*dvoint)();
	char	*dvioblk;
	int	dvminor;
	};

extern	struct	devsw devtab[];		/* one entry per device */


/* Device name definitions */

#define	CONSOLE     0			/* type tty      */
#define	ETHER       1			/* type eth      */
#define	INTERNET    2			/* type dgm      */
#define	DGRAM1      3			/* type dg       */
#define	DGRAM2      4			/* type dg       */
#define	DGRAM3      5			/* type dg       */
#define	DGRAM4      6			/* type dg       */
#define	RFILSYS     7			/* type rfm      */
#define	RFILE1      8			/* type rf       */
#define	RFILE2      9			/* type rf       */
#define	RFILE3      10			/* type rf       */
#define	RFILE4      11			/* type rf       */
#define	NAMESPACE   12			/* type nam      */

/* Control block sizes */

#define	Ntty	1
#define	Neth	1
#define	Ndgm	1
#define	Ndg	4
#define	Nrfm	1
#define	Nrf	4
#define	Nnam	1

#define	NDEVS	13

/* Declarations of I/O routines referenced */

extern	int	ttyinit();
extern	int	ionull();
extern	int	ttyread();
extern	int	ttywrite();
extern	int	ioerr();
extern	int	ttycntl();
extern	int	ttygetc();
extern	int	ttyputc();
extern	int	ethinit();
extern	int	ethread();
extern	int	ethwrite();
extern	int	ethcntl();
extern	int	ethinter();
extern	int	dgmopen();
extern	int	dgmcntl();
extern	int	dginit();
extern	int	dgclose();
extern	int	dgread();
extern	int	dgwrite();
extern	int	dgcntl();
extern	int	rfopen();
extern	int	rfcntl();
extern	int	rfinit();
extern	int	rfclose();
extern	int	rfread();
extern	int	rfwrite();
extern	int	rfseek();
extern	int	rfgetc();
extern	int	rfputc();
extern	int	naminit();
extern	int	namopen();


/* Configuration and Size Constants */

#define	NPROC	    30			/* number of user processes	*/
#define	NSEM	    50			/* number of semaphores		*/
#define	MEMMARK				/* define if memory marking used*/
#define	RTCLOCK				/* now have RTC support		*/
#define	STKCHK				/* resched checks stack overflow*/
#define	NETDAEMON			/* Ethernet network daemon runs	*/
#define	GATEWAY	     128,10,3,8		/* Gateway address		*/
#define	TSERVER	    "128.10.3.20:37"	/* Time server address		*/
#define	RSERVER	    "128.10.3.20:2001"	/* Remote file server address	*/
#define	NSERVER	    "128.10.3.20:53"	/* Domain Name server address	*/
#define	VERSION	    "7.2.2 SUN3 (Wed May 27, 1992)"/* label printed at startup	*/
