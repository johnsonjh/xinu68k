/* create.c - create, newpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
SYSCALL create(procaddr,ssize,priority,name,nargs,args)
	int	*procaddr;		/* procedure address		*/
	int	ssize;			/* stack size in words		*/
	int	priority;		/* process priority >= 0	*/
	char	*name;			/* name (for debugging)		*/
	int	nargs;			/* number of args that follow	*/
	WORD	args;			/* arguments (treated like an	*/
					/* array in the code)		*/
{
	int	pid;			/* stores new process id	*/
	struct	pentry	*pptr;		/* pointer to proc. table entry */
	int	i;
	WORD	*a;			/* points to list of args	*/
	WORD	*saddr;			/* stack address		*/
	int	INITRET();

	disable();
	ssize = (int) roundew(ssize);
	if ( ssize < MINSTK || ((saddr = getstk(ssize)) == SYSERR ) ||
		(pid=newpid()) == SYSERR || isodd(procaddr) ||
		priority < 1 ) {
		restore();
		return(SYSERR);
	}
	numproc++;
	pptr = &proctab[pid];
	pptr->pstate = PRSUSP;
	for (i=0 ; i<PNMLEN && (int)(pptr->pname[i]=name[i])!=0 ; i++)
		;
	pptr->pprio = priority;
	pptr->pbase = (WORD) saddr;
	pptr->pstklen = ssize;
	pptr->plimit = pptr->pbase - ssize + sizeof (WORD);	
		/* Bottom of stack */
	pptr->psem = 0;
	pptr->pargs = nargs;
	pptr->phasps = 0;
	pptr->phasmsg = 0;
	for (i=0 ; i<PNREGS ; i++)
		pptr->pregs[i]=INITREG;
	pptr->pregs[PC] = pptr->paddr = (WORD)procaddr;
	pptr->pregs[PS] = INITPS;
	a = (&args) + (nargs-1);	/* point to last argument	*/
	for ( ; nargs > 0 ; nargs--) {	/* machine dependent; copy args	*/
		*saddr-- = *a--;
	}
					/* onto created process' stack	*/
	*saddr = (WORD)INITRET;	/* push on return address	*/
#ifdef	lsi11
	pptr->pregs[SP] = (WORD)saddr;
#else
	pptr->pregs[USP] = (WORD)saddr;
	pptr->pregs[SSP] = (WORD)saddr;
#endif
	restore();
	return(pid);
}

/*------------------------------------------------------------------------
 * newpid  --  obtain a new (free) process id
 *------------------------------------------------------------------------
 */
LOCAL	newpid()
{
	int	pid;			/* process id to return		*/
	int	i;

	for (i=0 ; i<NPROC ; i++) {	/* check all NPROC slots	*/
		if ( (pid=nextproc--) <= 0)
			nextproc = NPROC-1;
		if (proctab[pid].pstate == PRFREE)
			return(pid);
	}
	return(SYSERR);
}
