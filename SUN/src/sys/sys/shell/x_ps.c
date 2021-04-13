/* x_ps.c - x_ps */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

LOCAL	char	hd1[] =
	"pid   name   state prio  stack range  stack length   sem message\n";
LOCAL	char	hd2[] =
	"--- -------- ----- ---- ------------- ------------   --- -------\n";
LOCAL	char	*pstnams[] = {"curr ","free ","ready","recv ",
			    "sleep","susp ","wait ","rtim "};
LOCAL	int	psavsp;

/*------------------------------------------------------------------------
 *  x_ps  -  (command ps) format and print process table information
 *------------------------------------------------------------------------
 */
COMMAND	x_ps(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	i;
	char	str[80];
	struct	pentry	*pptr;
	unsigned long currstk;

	asm("movl sp,_psavsp");	/* capture current stack pointer */
	proctab[currpid].pregs[SSP] = psavsp;
	write(stdout, hd1, strlen(hd1));
	write(stdout, hd2, strlen(hd2));
	for (i=0 ; i<NPROC ; i++) {
		if ((pptr = &proctab[i])->pstate == PRFREE)
			continue;
		sprintf(str, "%3d %8s %s ", i, pptr->pname,
			pstnams[pptr->pstate-1]);
		write(stdout, str, strlen(str));
		sprintf(str, "%4d %06x-%06x ", pptr->pprio,  pptr->plimit,
			(unsigned)pptr->pbase + 1);
		write(stdout, str, strlen(str));
		currstk = pptr->pregs[SSP];
		if (currstk < pptr->plimit || currstk > pptr->pbase)
			sprintf(str, " OVERFLOWED     ");
		else
			sprintf(str, "%5d /%5d    ", pptr->pbase - currstk,
				pptr->pbase - pptr->plimit + sizeof(int));
		write(stdout, str, strlen(str));
		if (pptr->pstate == PRWAIT)
			sprintf(str, "%2d", pptr->psem);
		else
			sprintf(str, "- ");
		write(stdout, str, strlen(str));
		if (pptr->phasmsg)
			sprintf(str, "%06x\n", pptr->pmsg);
		else
			sprintf(str, "   -\n");
		write(stdout, str, strlen(str));
	}
}




