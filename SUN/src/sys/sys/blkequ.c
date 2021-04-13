/* blkequ.c - blkequ */

#include <kernel.h>

/*------------------------------------------------------------------------
 *  blkequ  -  return TRUE iff one block of memory is equal to another
 *------------------------------------------------------------------------
 */
blkequ(first, second, nbytes)
register char	*first;
register char	*second;
register int	nbytes;
{
	while (--nbytes >= 0)
		if (*first++ != *second++)
			return(FALSE);
	return(TRUE);
}
