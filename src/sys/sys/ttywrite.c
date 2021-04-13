/* ttywrite.c - ttywrite, writcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define TTYWRITE ttywrite6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define TTYWRITE ttywrite7201, etc.
   if SLUCHIP undef.,#include slu.h, #define TTYWRITE ttywrite  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>

/*------------------------------------------------------------------------
 *  ttywrite - write one or more characters to a tty device
 *------------------------------------------------------------------------
 */
TTYWRITE(devptr,buff,count)
	register struct	devsw	*devptr;
	register char	*buff;
	register int	count;
{
	register struct csr *cptr;
	register struct tty *ttyp;
	int avail;

	ttyp = &tty[devptr->dvminor];
	cptr = (struct csr *)devptr->dvcsr;

	disable();
	if ( (avail=scount(ttyp->osem)) > count) {
		writcopy(buff, ttyp, count);
	} else {
		writcopy(buff, ttyp, avail);
		buff += avail;
		for (count-=avail ; count>0 ; count--)
			TTYPUTC(devptr, *buff++);
	}
	slutenable(cptr); /* enable transmitter ints */
	restore();
	return(OK);
}

/*------------------------------------------------------------------------
 *  writcopy - high-speed copy from user's buffer into system buffer
 *------------------------------------------------------------------------
 */
LOCAL writcopy(buff, ttyp, count)
	register char	*buff;
	register struct	tty *ttyp;
	register int	count;
{
	register char	*qhead, *qend, *uend;

	qhead = &ttyp->obuff[ttyp->ohead];
	qend  = &ttyp->obuff[OBUFLEN];
	uend  = buff + count;
	while (buff < uend) {
		*qhead++ = *buff++;
		if ( qhead >= qend )
			qhead = ttyp->obuff;
	}
	ttyp->ohead = qhead - ttyp->obuff;
	sreset(ttyp->osem, scount(ttyp->osem)-count);
}
