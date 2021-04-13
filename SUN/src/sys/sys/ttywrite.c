/* ttywrite.c - ttywrite, writcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>

/*------------------------------------------------------------------------
 *  ttywrite - write one or more characters to a tty device
 *------------------------------------------------------------------------
 */
ttywrite(devptr, buff, count)
struct	devsw	*devptr;
char	*buff;
int	count;
{
	STATWORD ps;    
	register struct tty *ttyp;
	int ncopied;

	if (count < 0)
		return(SYSERR);
	if (count == 0)
		return(OK);
	disable(ps);
	ttyp = &tty[devptr->dvminor];

	count -= (ncopied = writcopy(buff, ttyp, count));
	buff += ncopied;

	for ( ; count>0 ; count--) {
		ttyputc(devptr, *buff++);
	}
	restore(ps);
	return(OK);
}

/*------------------------------------------------------------------------
 *  writcopy - high-speed copy from user's buffer into system buffer
 *------------------------------------------------------------------------
 */
LOCAL writcopy(buff, ttyp, count)
char	*buff;
struct	tty *ttyp;
int	count;
{
	register int	avail;
	register char	*cp, *qhead, *qend, *uend;

	avail = OBUFLEN - ttyp->ocnt;
	qhead = &ttyp->obuff[ttyp->ohead];
	qend  = &ttyp->obuff[OBUFLEN];
	cp    = buff;
	uend  = buff + count;
	while (avail-- > 1 && cp < uend) {
		if (*cp == NEWLINE && ttyp->ocrlf) {
			*qhead++ = RETURN;
			--avail;
			if ( qhead >= qend )
				qhead = ttyp->obuff;
		}
		*qhead++ = *cp++;
		if ( qhead >= qend )
			qhead = ttyp->obuff;
	}					/* avail decremented one*/
	ttyp->ohead = qhead - ttyp->obuff;	/* extra time when loop	*/
	ttyp->ocnt = OBUFLEN - (avail+1);
	sreset(ttyp->osem, ++avail);		/* condition fails.	*/
	ttyostart(ttyp);
	return(cp - buff);
}
