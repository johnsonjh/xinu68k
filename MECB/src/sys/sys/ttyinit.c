/* ttyinit.c - ttyinit */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define TTYINIT ttyinit6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define TTYINIT ttyinit7201, etc.
   if SLUCHIP undef.,#include slu.h, #define TTYINIT ttyinit  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>

/*------------------------------------------------------------------------
 *  ttyinit - initialize buffers and modes for a tty line
 *  (if SLUCHIP is nontrivial, this defines ttyinit6850 or ttyinit7201)
 *------------------------------------------------------------------------
 */
TTYINIT(devptr)
	struct	devsw	*devptr;
{
	register struct	tty *ttyp;
	struct csr *cptr;
	int	junk, isconsole;

#ifdef	debug
	dotrace("ttyinit", &devptr, 1);
#endif

	ttyp = &tty[devptr->dvminor];

/*	iosetvec(devptr->dvnum, ttyp, ttyp); */
/* The "map" trick for interrupt dispatching doesn't work for 68000 */
/* so iosetvec is obsolete--simpler but device-dependent method is used */
/* here we just write the interrupt vector(s) in low memory--
     (for 6850 and 7201, both of these do the same thing)  */
	*(int *)(devptr->dvivec) = (int)devptr->dviint;/* set up int. vec(s)*/
	*(int *)(devptr->dvovec) = (int)devptr->dvoint;

	devptr->dvioblk = (char *) ttyp;	/* fill tty control blk	*/
	isconsole = (devptr->dvnum == CONSOLE);	/* make console cooked	*/
	ttyp->ioaddr = (struct csr *)devptr->dvcsr; /* copy csr address */
	ttyp->ihead = ttyp->itail = 0;		/* empty input queue	*/
	ttyp->isem = screate(0);		/* chars. read so far=0	*/
	ttyp->osem = screate(OBUFLEN);		/* buffer available=all */
	ttyp->odsend = 0;			/* sends delayed so far	*/
	ttyp->ohead = ttyp->otail = 0;		/* output queue empty	*/
	ttyp->ehead = ttyp->etail = 0;		/* echo queue empty	*/
	ttyp->imode = (isconsole ? IMCOOKED : IMRAW);
	ttyp->iecho = ttyp->evis = isconsole;	/* echo console input	*/
	ttyp->ierase = ttyp->ieback = isconsole;/* console honors erase	*/
	ttyp->ierasec = BACKSP;			/*  using ^h		*/
	ttyp->ecrlf = ttyp->icrlf = isconsole;	/* map RETURN on input	*/
	ttyp->ocrlf = ttyp->oflow = isconsole;
	ttyp->ikill = isconsole;		/* set line kill == @	*/
	ttyp->ikillc = ATSIGN;
	ttyp->oheld = FALSE;
	ttyp->ostart = STRTCH;
	ttyp->ostop = STOPCH;
	ttyp->icursor = 0;
	ttyp->ifullc = TFULLC;

	cptr = (struct csr *)devptr->dvcsr;	/* CSR of this SLU	*/
	sluinitcsr(cptr);	                /* set dataformat, etc. */
	junk = slugetch(cptr);			/* Clear receiver	*/
	slutdisable(cptr);         		/* Disable trans. ints	*/
	slurenable(cptr);			/* Enable receiver ints	*/
}
