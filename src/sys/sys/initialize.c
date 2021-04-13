/* initialize.c - nulluser, sysinit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <tty.h>
#include <q.h>
#include <io.h>
#include <disk.h>

extern	int	main();			/* address of user's main prog	*/

/* Declarations of major kernel variables */

struct	pentry	proctab[NPROC]; /* process table			*/
int	nextproc;		/* next process slot to use in create	*/
struct	sentry	semaph[NSEM];	/* semaphore table			*/
int	nextsem;		/* next sempahore slot to use in screate*/
struct	qent	q[NQENT];	/* q table (see queue.c)		*/
int	nextqueue;		/* next slot in q structure to use	*/
WORD	*maxaddr;		/* max memory address (set by sizmem)	*/
#ifdef	NDEVS
struct	intmap	intmap[NDEVS];	/* interrupt dispatch table		*/
#endif
struct	mblock	memlist;	/* list of free memory blocks		*/
#ifdef	Ntty
struct  tty     tty[Ntty];	/* SLU buffers and mode control		*/
#endif

/* active system status */

int	numproc;		/* number of live user processes	*/
int	currpid;		/* id of currently running process	*/
int	reboot = 0;		/* non-zero after first boot		*/

/* real-time clock variables and sleeping process queue pointers		*/
#ifdef	RTCLOCK
int	count6;			/* counts in 60ths of a second 6-0	*/
int     defclk;			/* non-zero, then deferring clock count */
int     clkdiff;		/* deferred clock ticks			*/
int     slnempty;		/* FALSE if the sleep queue is empty	*/
int     *sltop;			/* address of key part of top entry in	*/
				/* the sleep queue if slnonempty==TRUE	*/
int     clockq;			/* head of queue of sleeping processes  */
int	preempt;		/* preemption counter.	Current process */
				/* is preempted when it reaches zero;	*/
				/* set in resched; counts in ticks	*/
int	clkruns = 1;		/* set TRUE iff clock exists by setclkr	*/
#else
int	clkruns = FALSE;	/* no clock configured; be sure sleep	*/
#endif				/*   doesn't wait forever		*/
int	rdyhead,rdytail;	/* head/tail of ready list (q indicies)	*/


/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED, and      ***/
/***   must eventually be enabled explicitly.  This routine turns     ***/
/***   itself into the null process after initialization.  Because    ***/
/***   the null process must always remain ready to run, it cannot    ***/
/***   execute code that might cause it to be suspended, wait for a   ***/
/***   semaphore, or put to sleep, or exit.  In particular, it must   ***/
/***   not do I/O unless it uses kprintf for polled output.           ***/
/***								      ***/
/************************************************************************/

/*------------------------------------------------------------------------
 *  nulluser  -- initialize system and become the null process (id==0)
 *------------------------------------------------------------------------
 */
nulluser()				/* babysit CPU when no one home */
{
int pid,retval;

#ifdef	DEBUG
	dotrace("nulluser", NULL, 0);
#endif
	kprintf("\n\nXinu Version %s%s\n", VERSION,
		reboot++==0 ? "" : "   (rebooted)");
	sysinit();			/* initialize all of Xinu */
	kprintf("%u real mem\n", (WORD) (maxaddr + 1));
	kprintf("%u avail mem\n",(unsigned)maxaddr-(unsigned)(&end)+(unsigned)sizeof(WORD));
	kprintf("%u end mem\n",(unsigned)(&end));
	setlowpri();

	/* start a process executing the user's main program */

	if ((pid = create(main,INITSTK,INITPRIO,INITNAME,1,0)) == SYSERR) {
		kprintf("Error in initial create call\n");
	}
	if ((retval = resume(pid)) == SYSERR) {
		kprintf("Error in resume of initial user process!\n");
	}
	while (TRUE) {			/* run forever */
		pause();	/* release bus if possible, else loop */
	}
}

/*------------------------------------------------------------------------
 *  sysinit  --  initialize all Xinu data structeres and devices
 *------------------------------------------------------------------------
 */
LOCAL	sysinit()
{
	int	i,j;
	struct	pentry	*pptr;
	struct	sentry	*sptr;
	struct	mblock	*mptr;

#ifdef	DEBUG
	dotrace("sysinit", NULL, 0);
#endif
	numproc = 0;			/* initialize system variables */
	nextproc = NPROC-1;
	nextsem = NSEM-1;
	nextqueue = NPROC;		/* q[0..NPROC-1] are processes */

	memlist.mnext = mptr =		/* initialize free memory list */
	  (struct mblock *) roundew(&end);
	mptr->mnext = (struct mblock *)NULL;
	mptr->mlen = (int) truncew((unsigned)maxaddr-NULLSTK-(unsigned)&end);

	for (i=0 ; i<NPROC ; i++)	/* initialize process table */
		proctab[i].pstate = PRFREE;

	pptr = &proctab[NULLPROC];	/* initialize null process entry */
	pptr->pstate = PRCURR;
	for (j=0; j<7; j++)
		pptr->pname[j] = "prnull"[j];
	pptr->plimit = (WORD)(maxaddr + 1) - NULLSTK;
	pptr->pbase = (WORD) maxaddr;
	pptr->paddr = (WORD) nulluser;
	pptr->pargs = 0;
	pptr->phasps = 0;
	pptr->pprio = 0;
	currpid = NULLPROC;

	for (i=0 ; i<NSEM ; i++) {	/* initialize semaphores */
		(sptr = &semaph[i])->sstate = SFREE;
		newqueue(&(sptr->sqhead), &(sptr->sqtail));
	}

	newqueue(&rdyhead,&rdytail);	/* initialize ready list */

#ifdef	MEMMARK
	_mkinit();			/* initialize memory marking */
#endif
#ifdef	RTCLOCK
	clkinit();		/* init r.t.clock	*/
#endif
#ifdef	Ndsk
	dskdbp= mkpool(DBUFSIZ,NDBUFF);	/* initialize disk buffers */
	dskrbp= mkpool(DREQSIZ,NDREQ);
#endif
#ifdef NDEVS
	for ( i=0 ; i<NDEVS ; i++ ){	/* initialize devices */
		init(i);
	}
#endif
#ifdef	NNETS
	netinit();			/* initialize networks */
#endif
	return(OK);
}
