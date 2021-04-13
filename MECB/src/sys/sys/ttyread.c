/* ttyread.c - ttyread, readcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttyread - read one or more characters from a tty device
 *------------------------------------------------------------------------
 */
ttyread(devptr, buff, count)
	struct	devsw	*devptr;
	int count;
	char *buff;
{
	register struct	tty	*ttyp;
	int	avail, numread;

#ifdef	DEBUG
	dotrace("ttyread", &devptr, 3);
#endif
	if (count < 0)
		return(SYSERR);
	disable();

	avail = scount( (ttyp= &tty[devptr->dvminor])->isem );
	if ( (count = (count==0 ? avail : count)) == 0) {
		restore();
		return(0);
	}
	numread = count;
	if (avail >= count) {
		readcopy(buff, ttyp, count);
		restore();
		return(numread);
	}
	if (avail > 0) {
		readcopy(buff, ttyp, avail);
		buff += avail;
		count -= avail;
	}
	restore();
	for ( ; count>0 ; count--)
		*buff++ = ttygetc(devptr);
	return(numread);
}

/*------------------------------------------------------------------------
 *  readcopy - high speed copy procedure used by ttyread
 *------------------------------------------------------------------------
 */
LOCAL readcopy(buff,ttyp,count)
register char *buff;
struct tty *ttyp;
int count;
{
	register char *qtail, *qend, *uend;	/* copy	loop variables	*/

	qtail =	&ttyp->ibuff[ttyp->itail];
	qend  =	&ttyp->ibuff[IBUFLEN];
	uend = buff + count;
	while (	buff < uend ) {
		*buff++	= *qtail++;
		if ( qtail >= qend )
			qtail =	ttyp->ibuff;
	}
	ttyp->itail = qtail-ttyp->ibuff;
	sreset(ttyp->isem, scount(ttyp->isem)-count);
}
