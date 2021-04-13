/* ttyputc.c - ttyputc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>
#include <zsreg.h>

extern char *foob2;

/*------------------------------------------------------------------------
 *  ttyputc - write one character to a tty device
 *------------------------------------------------------------------------
 */
ttyputc(devptr, ch )
struct	devsw	*devptr;
char	ch;
{
	STATWORD ps;    
	struct	tty   *iptr;

	iptr = &tty[devptr->dvminor];
        if ( ch==NEWLINE && iptr->ocrlf )
                ttyputc(devptr,RETURN);
	disable(ps);
	wait(iptr->osem);		/* wait	for space in queue	*/
	/**foob2++ = ch;*/
	iptr->obuff[iptr->ohead++] = ch;
	++iptr->ocnt;
	if (iptr->ohead	>= OBUFLEN)
		iptr->ohead = 0;
	ttyostart(iptr);
	restore(ps);
	return(OK);
}



