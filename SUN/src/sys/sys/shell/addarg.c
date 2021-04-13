/* addarg.c - addarg */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  addarg  -  copy arguments to area reserved on process stack
 *------------------------------------------------------------------------
 */
addarg(pid, nargs, len)
int	pid;				/* process to receive arguments	*/
int	nargs;				/* number of arguments to copy	*/
int	len;				/* size of arg. area (in bytes)	*/
{
	struct	pentry	*pptr;
	char	**fromarg;
	long	*toarg;
	char	*to;

	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate != PRSUSP)
		return(SYSERR);
	toarg = (long *) ( ((unsigned)pptr->pbase) - (unsigned)len );
	to = (char *) (toarg + (nargs + 2));
	*toarg++ = (long) (toarg + 1);
	for (fromarg=Shl.shtok ; nargs > 0 ; nargs--) {
		*toarg++ = to;
		strcpy(to, *fromarg++);
		to += strlen(to) + 1;		
	}
	*toarg = 0;
	return(OK);
}
