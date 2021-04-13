/* ttyiin.c ttyiin, ttybreak, erase1, eputc, echoch */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>
#include <zsreg.h>


/*------------------------------------------------------------------------
 *  ttyiin  --  lower-half tty device driver for input interrupts
 *------------------------------------------------------------------------
 */
INTPROC	ttyiin(iptr)
	register struct	tty	*iptr;	/* pointer to tty block		*/
{
	STATWORD ps;    
    
	register struct	zscc_device *zptr;
	register int	ch;
	int	err;
	int	cerr;
	int	ct;

	zptr = iptr->ioaddr;
        ch = zptr->zscc_data;

	/* get the contents of RR1 */
	zptr->zscc_control = 1;
	DELAY(2);
	err = zptr->zscc_control;

	/* check for Parity, Overrun, and Framing errors */
	err &= (ZSRR1_PE | ZSRR1_DO | ZSRR1_FE);

	cerr = 0;
	if (err != 0) {
#ifdef PRINTERRORS	    
	    kprintf("recv error, ch: 0x%x ", (unsigned int) ch);
	    if (err & ZSRR1_PE)
		kprintf(", Parity Error");
	    if (err & ZSRR1_DO)
		kprintf(", Data Overrun");
	    if (err & ZSRR1_PE)
		kprintf(", Framing Error");
	    kprintf("\n");
#endif	    
	    cerr = IOCHERR;
	    zptr->zscc_control = ZSWR0_RESET_ERRORS;	/* reset the error */
	}
#ifdef DEBUG	
	kprintf("\n<%c,%x,%x,%x>	",
		ch,(unsigned int) ch, (unsigned int) err,
		(unsigned int) cerr);
#endif	
	
	if (iptr->imode == IMRAW) {
		if (iptr->icnt >= IBUFLEN){
			return;
		}
		iptr->ibuff[iptr->ihead++] = ch | cerr;
		++iptr->icnt;
		if (iptr->ihead	>= IBUFLEN)	/* wrap buffer pointer	*/
			iptr->ihead = 0;
	        signal(iptr->isem);
	} else {				/* cbreak | cooked mode	*/
		if ( ch	== RETURN && iptr->icrlf )
			ch = NEWLINE;
		if (iptr->iintr && ch == iptr->iintrc) {
			send(iptr->iintpid, INTRMSG);
			eputc(ch, iptr, zptr);
			return;
		}
		if (iptr->oflow) {
			if (ch == iptr->ostart)	{
				iptr->oheld = FALSE;
				ttyostart(iptr);
				return;
			}
			if (ch == iptr->ostop) {
				iptr->oheld = TRUE;
				return;
			}
		} else
		    iptr->oheld = FALSE;
		if (iptr->imode	== IMCBREAK) {		/* cbreak mode	*/
			if (iptr->icnt >= IBUFLEN) {
				eputc(iptr->ifullc,iptr,zptr);
				return;
			}
			iptr->ibuff[iptr->ihead++] = ch	| cerr;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
			if (iptr->iecho)
				echoch(ch,iptr,zptr);
			if (iptr->icnt < IBUFLEN) {
				++iptr->icnt;
				signal(iptr->isem);
			}
		} else {				/* cooked mode	*/
			if (ch == iptr->ikillc && iptr->ikill) {
				iptr->ihead -= iptr->icursor;
				if (iptr->ihead	< 0)
					iptr->ihead += IBUFLEN;
				iptr->icursor =	0;
				eputc(RETURN,iptr,zptr);
				eputc(NEWLINE,iptr,zptr);
				return;
			}
			if (ch == iptr->ierasec	&& iptr->ierase) {
				if (iptr->icursor > 0) {
					iptr->icursor--;
					erase1(iptr,zptr);
				}
				return;
			}
			if (ch == NEWLINE || ch == RETURN ||
				(iptr->ieof && ch == iptr->ieofc)) {
				if (iptr->iecho) {
					echoch(ch,iptr,zptr);
					if (ch == iptr->ieofc)
						echoch(NEWLINE,iptr,zptr);
				}
				iptr->ibuff[iptr->ihead++] = ch | cerr;
				if (iptr->ihead	>= IBUFLEN)
					iptr->ihead = 0;
				ct = iptr->icursor+1; /* +1 for \n or \r*/
				iptr->icursor =	0;
				iptr->icnt += ct;
				signaln(iptr->isem,ct);
				return;
			}
			ct = iptr->icnt;
			ct = ct	< 0 ? 0	: ct;
			if ((ct	+ iptr->icursor) >= IBUFLEN-1) {
				eputc(iptr->ifullc,iptr,zptr);
				return;
			}
			if (iptr->iecho)
				echoch(ch,iptr,zptr);
			iptr->icursor++;
			iptr->ibuff[iptr->ihead++] = ch	| cerr;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
		}
	}
}

/*------------------------------------------------------------------------
 *  erase1  --  erase one character honoring erasing backspace
 *------------------------------------------------------------------------
 */
LOCAL erase1(iptr,zptr)
	struct	tty   *iptr;
	struct	zscc_device	*zptr;
{
	char	ch;

	if (--(iptr->ihead) < 0)
		iptr->ihead += IBUFLEN;
	ch = iptr->ibuff[iptr->ihead];
	if (iptr->iecho) {
		if (ch < BLANK || ch == 0177) {
			if (iptr->evis)	{
				eputc(BACKSP,iptr,zptr);
				if (iptr->ieback) {
					eputc(BLANK,iptr,zptr);
					eputc(BACKSP,iptr,zptr);
				}
			}
			eputc(BACKSP,iptr,zptr);
			if (iptr->ieback) {
				eputc(BLANK,iptr,zptr);
				eputc(BACKSP,iptr,zptr);
			}
		} else {
			eputc(BACKSP,iptr,zptr);
			if (iptr->ieback) {
				eputc(BLANK,iptr,zptr);
				eputc(BACKSP,iptr,zptr);
			}
		}
	} 
        else
            ttyostart(iptr);
}




/*------------------------------------------------------------------------
 *  echoch  --  echo a character with visual and ocrlf options
 *------------------------------------------------------------------------
 */
LOCAL echoch(ch, iptr, zptr)
	char	ch;		/* character to	echo			*/
	struct	tty   *iptr;	/* pointer to I/O block for this devptr	*/
	struct	zscc_device	*zptr;	/* zscc_device address for this devptr		*/
{
	if ((ch==NEWLINE||ch==RETURN)&&iptr->ecrlf) {
		eputc(RETURN,iptr,zptr);
		eputc(NEWLINE,iptr,zptr);
	} else if ((ch<BLANK||ch==0177) && iptr->evis) {
		eputc(UPARROW,iptr,zptr);
		eputc(ch+0100,iptr,zptr);	/* make it printable	*/
	} else {
		eputc(ch,iptr,zptr);
	}
}

/*------------------------------------------------------------------------
 *  eputc - put one character in the echo queue
 *------------------------------------------------------------------------
 */
LOCAL eputc(ch,iptr,zptr)
	char	ch;
	struct	tty   *iptr;
	struct	zscc_device *zptr;
{
	iptr->ebuff[iptr->ehead++] = ch;
	if (iptr->ehead	>= EBUFLEN)
		iptr->ehead = 0;
	ttyostart(iptr);
}


/*------------------------------------------------------------------------
 *  ttybreak -- handle a break received on the serial line
 *------------------------------------------------------------------------
 */
ttybreak()
{
    STATWORD ps;
    
    disable(ps);
    kprintf("\n\nSerial line BREAK detected.\n");
    ret_mon();
    restore(ps);
    return;
}
