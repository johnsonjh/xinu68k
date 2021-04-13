/* x_mem.c - x_mem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>

extern char *etext, *edata;


/*------------------------------------------------------------------------
 *  x_mem  -  (command mem) print memory use and free list information
 *------------------------------------------------------------------------
 */
COMMAND	x_mem(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	i;
	struct	mblock	*mptr;
	char	str[80];
	unsigned free;
	unsigned avail;
	unsigned stkmem;

	/* calculate current size of free memory and stack memory */

	for( free=0,mptr=memlist.mnext ; mptr!=(struct mblock *)NULL ;
	     mptr=mptr->mnext)
		free += mptr->mlen;
	for (stkmem=0,i=0 ; i<NPROC ; i++) {
		if (proctab[i].pstate != PRFREE)
			stkmem += (unsigned)proctab[i].pstklen;
	}
	sprintf(str,
		"Memory: %u bytes real memory, %u text, %u data, %u bss\n",
		1 + (unsigned)maxaddr, (unsigned) &etext,
		(unsigned) &edata - (unsigned) &etext,
		(unsigned) &end - (unsigned) &edata);
	write(stdout, str, strlen(str));
	avail = (unsigned)maxaddr - (unsigned) &end + 1;
	sprintf(str," initially: %5u avail\n", avail);
	write(stdout, str, strlen(str));
	sprintf(str," presently: %5u avail, %5u stack, %5u heap\n",
		free, stkmem, avail - stkmem - free);
	write(stdout, str, strlen(str));
	sprintf(str," free list:\n");
	write(stdout, str, strlen(str));
	for( mptr=memlist.mnext ; mptr!=(struct mblock *)NULL ;
	     mptr=mptr->mnext) {
		sprintf(str,"   block at %6o, length %5u (0x%x)\n",
			mptr, mptr->mlen, mptr->mlen);
		write(stdout, str, strlen(str));
	}
	return(OK);
}

