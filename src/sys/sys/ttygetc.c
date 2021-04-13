/* ttygetc.c - ttygetc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttygetc - read one character from a tty device
 *------------------------------------------------------------------------
 */
ttygetc(devptr)
	struct	devsw	*devptr;
{
	int	ch;
	register struct	tty   *ttyp;

#ifdef	DEBUG
	dotrace("ttygetc", &devptr, 1);
#endif
	ttyp = &tty[devptr->dvminor];
	wait(ttyp->isem);		/* wait	for a character	in buff	*/
	disable();
	ch = ttyp->ibuff[ttyp->itail++];
	if (ttyp->itail	== IBUFLEN)
		ttyp->itail = 0;
	restore();
	return(ch);
}
