/* conf.h */

#define	NULLPTR	(char *)0

/* Device table declarations */
struct	devsw	{			/* device table entry */
	int	dvnum;
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
#define	TTYB        1			/* type tty      */
#define TTYC        2		        /* type tty      */
#define	RING0IN     3			/* type dlc      */
#define	RING0OUT    4			/* type dlc      */
#define	LPR         5			/* type lp       */

/* Control block sizes */

#define	Ntty	3
#define	Ndlc	2
#define	Nlp	1

#define	NDEVS	6

/* Declarations of I/O routines referenced */

extern	int	ttyinit();
extern	int	ioerr();
extern	int	ttyread();
extern	int	ttywrite();
extern	int	ttycntl();
extern	int	ttygetc();
extern	int	ttyputc();
extern	int	slu1int();
extern	int	slu2int();
extern  int     slu3int();
extern	int	dlcinit();
extern	int	dlcread();
extern	int	dlcwrite();
extern	int	dlccntl();
extern	int	dlcputc();
extern	int	dlciin();
extern	int	dlcoin();
extern	int	lpinit();
extern	int	lpopen();
extern	int	lpclose();
extern	int	lpwrite();
extern	int	lpcntl();
extern	int	lpputc();
extern	int	ionull();
extern	int	lpoin();
/* for ATT7300 -- 2 SLUs, so two versions of most tty modules-- */
extern	int	ttyinit6850();
extern	int	ttywrite6850();
extern	int	ttycntl6850();
extern	int	ttyputc6850();
extern	int	ttyinit7201();
extern	int	ttywrite7201();
extern	int	ttycntl7201();
extern	int	ttyputc7201();



 /* Configuration and Size Constants */

#define MEMMARK				/* use memory marking		*/
#define NNETS 1	                        /* number of Xinu networks      */
#define	NPROC	10			/* number of user processes	*/
#define	NSEM	50			/* total number of semaphores	*/
#define	RTCLOCK
#define	VERSION	"3.0"    		/* label printed at startup	*/
