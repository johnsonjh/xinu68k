/* conoutput.c - conputc conwrite */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>    
#include <sunromvec.h>    

#define prputchar(x) (romp->v_fwritechar)((unsigned char) x)

/*------------------------------------------------------------------------
 *  conputc - write one character to the SUN physical monitor
 *------------------------------------------------------------------------
 */
conputc(devptr, c )
	struct	devsw	*devptr;
     	register unsigned char c;
{
	if ( c == 0 )
		return;
	if ( c > 127 ) {
	    prputchar('M');		/* do what "cat -v" does */
	    prputchar('-');		/* prom routine dies otherwise */
	    prputchar((char) (c & 0x007f) );
	    return;
	}

	if (c==NEWLINE)
	    prputchar(RETURN);

	prputchar(c);
}


/*------------------------------------------------------------------------
 *  conwrite - write to the SUN physical monitor
 *------------------------------------------------------------------------
 */
conwrite(devptr, buff, count)
	struct	devsw	*devptr;
	char *buff;
	int count;
{

    if (count < 0)
	return(SYSERR);
    if (count == 0)
	return(OK);

    for (; count>0 ; count--)
	conputc(devptr, *buff++);

    return(OK);
}




