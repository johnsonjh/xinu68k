/* proc.h - isbadpid */

/* process table declarations and defined constants			*/

#ifndef	NPROC				/* set the number of processes	*/
#define	NPROC		10		/*  allowed if not already done	*/
#endif

/* process state constants */

#define	PRFREE		'\01'		/* process slot is free		*/
#define	PRCURR		'\02'		/* process is currently running	*/
#define	PRREADY		'\03'		/* process is on ready queue	*/
#define	PRSUSP		'\04'		/* process is suspended		*/
#define	PRRECV		'\05'		/* process waiting for message	*/
#define	PRWAIT		'\06'		/* process is on semaphore queue*/
#define	PRSLEEP		'\07'		/* process is sleeping		*/

/* miscellaneous process definitions */

#ifdef	lsi11
#define	PNREGS		8		/* size of saved register area	*/
#define	PNMLEN		8		/* length of process "name"	*/

#else
#define	PNREGS		19		/* size of saved register area	*/
#define	PNMLEN		16		/* length of process "name"	*/
#endif

#define	NULLPROC	0		/* id of the null process; it	*/
					/*  is always eligible to run	*/

#define	isbadpid(x)	(x<=0 || x>=NPROC)

/* process table entry */

struct	pentry	{
	SAVEDPS	pstatreg;		/* PS at time of call to disable*/
	char	phasps;			/* nonzero iff pstateg is valid	*/
	char	pstate;			/* process state: PRCURR, etc.	*/
	char	pname[PNMLEN];		/* process name			*/
	int	pprio;			/* process priority		*/
	int	psem;			/* semaphore if process waiting	*/
	int	pargs;			/* initial number of arguments	*/
	char	phasmsg;		/* nonzero iff pmsg is valid	*/
	WORD	pmsg;			/* message sent to this process	*/
	WORD	pregs[PNREGS];		/* saved registers PC, and PS	*/
	WORD	paddr;			/* initial code address		*/
	WORD	pbase;			/* base of run time stack	*/
	int	pstklen;		/* stack length			*/
	WORD	plimit;			/* lowest extent of stack	*/
};

extern	struct	pentry proctab[];
extern	int	numproc;		/* currently active processes	*/
extern	int	nextproc;		/* search point for free slot	*/
extern	int	currpid;		/* currently executing process	*/
