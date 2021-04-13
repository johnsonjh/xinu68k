/* mark.c - _mkinit, mark */

#include <conf.h>
#include <kernel.h>
#include <mark.h>


#ifdef	MEMMARK
int	*marks[MAXMARK];
int	nmarks;
int	mkmutex;

/*------------------------------------------------------------------------
 *  _mkinit  --  called once at system startup
 *------------------------------------------------------------------------
 */
_mkinit()
{
	mkmutex = screate(1);
	nmarks = 0;
}



/*------------------------------------------------------------------------
 *  mark  --  mark a location if it hasn't been marked
 *------------------------------------------------------------------------
 */
mark(loc)
int *loc;
{

	if ( *loc>=0 && *loc<nmarks && marks[*loc]==loc )
		return(0);
	if (nmarks>=MAXMARK)
		return(SYSERR);
	wait(mkmutex);
	marks[ (*loc) = nmarks++] = loc;
	signal(mkmutex);
	return(OK);
}
#endif
