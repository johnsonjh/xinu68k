/* unmount.c - unmount */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  unmount  -  remove an entry from the name prefix mapping table
 *------------------------------------------------------------------------
 */
SYSCALL	unmount(prefix)
char	*prefix;
{
	STATWORD ps;    
	struct	nament	*nptr;
	int	i;

	if (prefix == NULL)
		prefix = NULLSTR;
	else if (strlen(prefix) >= NAMPLEN)
		return(SYSERR);
	disable(ps);
	for (i=0 ; i<Nam.nnames ; i++) {
		nptr = &Nam.nametab[i];
		if (strcmp(prefix, nptr->npre) == 0) {
			for(Nam.nnames-- ; i<Nam.nnames ; i++)
				Nam.nametab[i] = Nam.nametab[i+1];
			restore(ps);
			return(OK);
		}
	}
	restore(ps);
	return(SYSERR);
}
