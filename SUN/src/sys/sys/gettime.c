/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(timvar)
long	*timvar;
{
	long	now;

	if (getutim(&now) == SYSERR)
		return(SYSERR);
	*timvar = ut2ltim(now);			/* adjust for timezone	*/
	return(OK);
}
