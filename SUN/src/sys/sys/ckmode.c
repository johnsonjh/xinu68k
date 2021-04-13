/* ckmode.c - ckmode */

#include <conf.h>
#include <kernel.h>
#include <file.h>

/*------------------------------------------------------------------------
 *  ckmode  --  parse open's mode argument and generate actual mode bits
 *------------------------------------------------------------------------
 */
ckmode(mode)
char	*mode;
{
	int	mbits;
	char	ch;

	mbits = 0;
	while (ch = *mode++)
		switch (ch) {

		    case FLRMODE:
				if (mbits&FLREAD) return(SYSERR);
				mbits |= FLREAD;
				break;

		    case FLWMODE:
				if (mbits&FLWRITE) return(SYSERR);
				mbits |= FLWRITE;
				break;

		    case FLOMODE:
				if (mbits&FLOLD || mbits&FLNEW)
					return(SYSERR);
				mbits |= FLOLD;
				break;

		    case FLNMODE:
				if (mbits&FLOLD || mbits&FLNEW)
					return(SYSERR);
				mbits |= FLNEW;
				break;

		    default:	return(SYSERR);
		}
	if ((mbits&FLREAD) == (mbits&FLWRITE))	/* default: allow R + W	*/
		mbits |= FLRW;
	return(mbits);
}
