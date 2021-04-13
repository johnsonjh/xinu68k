/* ttyiin.c ttyiin, erase1, eputc, echoch */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define TTYIIN ttyiin6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define TTYIIN ttyiin7201, etc.
   if SLUCHIP undef.,#include slu.h, #define TTYIIN ttyiin  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>

/*------------------------------------------------------------------------
 *  ttyiin  --  lower-half tty device driver for input interrupts
 *------------------------------------------------------------------------
 */
INTPROC TTYIIN(ttyp, cptr, error)
	register struct	tty	*ttyp;	/* pointer to tty block		*/
	register struct	csr	*cptr;	/* pointer to cont. & stat. reg	*/
	register int	error;
{
	register int	ch;
	Bool	cerr;
	int	ct;

#ifdef	DEBUG
/*	kprintf("ttyiin(%x, %x)\n", ttyp, cptr);
 */
#endif
	ch = slugetch(cptr);
	if (ttyp->imode == IMRAW) {
		if (scount(ttyp->isem) >= IBUFLEN)
			return;
		if (error) 			/* character error 	*/
			ttyp->ibuff[ttyp->ihead++] = ch | IOCHERR;
		else
			ttyp->ibuff[ttyp->ihead++] = ch;
		if (ttyp->ihead	>= IBUFLEN)	/* wrap buffer pointer	*/
			ttyp->ihead = 0;
	        signal(ttyp->isem);
	} else {				/* cbreak | cooked mode	*/
		cerr = error ? IOCHERR : 0;
		ch &= SLUCHMASK;
		if ( ch == RETURN && ttyp->icrlf )
			ch = NEWLINE;
		if (ttyp->oflow) {
			if (ch == ttyp->ostart)	{
				ttyp->oheld = FALSE;
				return;
			}
			if (ch == ttyp->ostop) {
				ttyp->oheld = TRUE;
				return;
			}
		}
		ttyp->oheld = FALSE;
		if (ttyp->imode	== IMCBREAK) {		/* cbreak mode	*/
			if (scount(ttyp->isem) >= IBUFLEN) {
				eputc(ttyp->ifullc,ttyp,cptr);
				return;
			}
			ttyp->ibuff[ttyp->ihead++] = ch	| cerr;
			if (ttyp->ihead	>= IBUFLEN)
				ttyp->ihead = 0;
			if (ttyp->iecho)
				echoch(ch,ttyp,cptr);
			if (scount(ttyp->isem) < IBUFLEN)
				signal(ttyp->isem);
		} else {				/* cooked mode	*/
			if (ch == ttyp->ikillc && ttyp->ikill) {
				ttyp->ihead -= ttyp->icursor;
				if (ttyp->ihead	< 0)
					ttyp->ihead += IBUFLEN;
				ttyp->icursor =	0;
				eputc(RETURN,ttyp,cptr);
				eputc(NEWLINE,ttyp,cptr);
				return;
			}
			if (ch == ttyp->ierasec	&& ttyp->ierase) {
				if (ttyp->icursor > 0) {
					ttyp->icursor--;
					erase1(ttyp,cptr);
				}
				return;
			}
			if (ch == NEWLINE || ch == RETURN) {
				if (ttyp->iecho)
					echoch(ch,ttyp,cptr);
				ttyp->ibuff[ttyp->ihead++] = ch	| cerr;
				if (ttyp->ihead	>= IBUFLEN)
					ttyp->ihead = 0;
				ct = ttyp->icursor+1; /* +1 for \n or \r*/
				ttyp->icursor =	0;
				signaln(ttyp->isem,ct);
				return;
			}
			ct = scount(ttyp->isem);
			ct = ct	< 0 ? 0	: ct;
			if ((ct	+ ttyp->icursor) >= IBUFLEN-1) {
				eputc(ttyp->ifullc,ttyp,cptr);
				return;
			}
			if (ttyp->iecho)
				echoch(ch,ttyp,cptr);
			ttyp->icursor++;
			ttyp->ibuff[ttyp->ihead++] = ch	| cerr;
			if (ttyp->ihead	>= IBUFLEN)
				ttyp->ihead = 0;
		}
	}
}

/*------------------------------------------------------------------------
 *  erase1  --  erase one character honoring erasing backspace
 *------------------------------------------------------------------------
 */
LOCAL erase1(ttyp,cptr)
	register struct	tty	*ttyp;
	register struct	csr	*cptr;
{
	register char	ch;

#ifdef	DEBUG
	kprintf("erase1(%x, %x)\n", ttyp, cptr);
#endif

	if (--(ttyp->ihead) < 0)
		ttyp->ihead += IBUFLEN;
	ch = ttyp->ibuff[ttyp->ihead];
	if (ttyp->iecho) {
		if (ch < BLANK || ch == 0177) {
			if (ttyp->evis)	{
				eputc(BACKSP,ttyp,cptr);
				if (ttyp->ieback) {
					eputc(BLANK,ttyp,cptr);
					eputc(BACKSP,ttyp,cptr);
				}
			}
			eputc(BACKSP,ttyp,cptr);
			if (ttyp->ieback) {
				eputc(BLANK,ttyp,cptr);
				eputc(BACKSP,ttyp,cptr);
			}
		} else {
			eputc(BACKSP,ttyp,cptr);
			if (ttyp->ieback) {
				eputc(BLANK,ttyp,cptr);
				eputc(BACKSP,ttyp,cptr);
			}
		}
	} else {
	       slutenable(cptr);	/* restart output */
	}
}

/*------------------------------------------------------------------------
 *  echoch  --  echo a character with visual and ocrlf options
 *------------------------------------------------------------------------
 */
LOCAL echoch(ch, ttyp, cptr)
	register char	ch;		/* character to	echo		*/
	register struct	tty	*ttyp;	/* pointer to I/O block for dev	*/
	register struct	csr	*cptr;	/* csr address for this devptr	*/
{
#ifdef	DEBUG
/*	kprintf("echoch(%c,%x,%x)\n", ch, ttyp, cptr); */
#endif
	if ((ch==NEWLINE||ch==RETURN)&&ttyp->ecrlf) {
		eputc(RETURN,ttyp,cptr);
		eputc(NEWLINE,ttyp,cptr);
	} else if ((ch < BLANK || ch == 0177) && ttyp->evis) {
		eputc(UPARROW,ttyp,cptr);
		eputc(ch+0100,ttyp,cptr);	/* make it printable	*/
	} else {
		eputc(ch,ttyp,cptr);
	}
}

/*------------------------------------------------------------------------
 *  eputc - put one character in the echo queue
 *------------------------------------------------------------------------
 */
LOCAL eputc(ch,ttyp,cptr)
	register char	ch;
	register struct	tty	*ttyp;
	register struct	csr	*cptr;
{
#ifdef	DEBUG
/*	kprintf("eputc(%c,%x,%x)\n", ch, ttyp, cptr); */
#endif
	ttyp->ebuff[ttyp->ehead++] = ch;
	if (ttyp->ehead	>= EBUFLEN)
		ttyp->ehead = 0;
	slutenable(cptr);	/* restart output */
}
