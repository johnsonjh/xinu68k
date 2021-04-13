/* fmode.c - fmode */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fmode  -  convert Xinu file protection mode to analogous UNIX mode
 *------------------------------------------------------------------------
 */
fmode(xmode)
int	xmode;
{
	int	umode;			/* UNIX file mode bits		*/

	if ((xmode&FLREAD) && (xmode&FLWRITE)) /* Xinu Read + Write	*/
		umode = O_RDWR;
	else if (xmode & FLREAD)	/* Xinu read only		*/
		umode = O_RDONLY;
	else if (xmode & FLWRITE)	/* Xinu write only		*/
		umode = O_WRONLY;
	else				/* Xinu neither of RW specified	*/
		umode = O_RDWR;

	if (xmode & FLNEW)		/* Xinu says a new file OK	*/
		umode |= O_CREAT | O_EXCL;
	else if (! (xmode & FLOLD))
		umode |= O_CREAT;	/* Xinu says create if needed	*/
	return(umode);
}
