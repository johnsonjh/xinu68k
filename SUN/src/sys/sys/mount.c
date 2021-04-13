/* mount.c - mount */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  mount  -  give meaning to a prefix in the abstract name space
 *------------------------------------------------------------------------
 */
SYSCALL	mount(prefix, dev, replace)
char	*prefix;
char	*replace;
int	dev;
{
	STATWORD ps;    
	struct	nament	*nptr;
	struct	nament	*last;
	int	i;

	if (prefix == NULL)
		prefix == NULLSTR;
	if (replace == NULL)
		replace == NULLSTR;
	if (strlen(prefix) >= NAMPLEN || strlen(replace) >= NAMRLEN)
		return(SYSERR);
	disable(ps);
	for (i=0 ; i<Nam.nnames ; i++) {
		nptr = &Nam.nametab[i];
		if (strcmp(prefix,nptr->npre) == 0) {
			strcpy(nptr->nrepl, replace);
			nptr->ndev = dev;
			restore(ps);
			return(OK);
		}
	}
	if (Nam.nnames >= NNAMES) {
		restore(ps);
		return(SYSERR);
	}
	nptr = last = &Nam.nametab[Nam.nnames++];
	if (Nam.nnames > 1) {	/* preserve last name prefix */
		nptr = last - 1;
		*last = *nptr;
	}
	strcpy(nptr->npre, prefix);
	strcpy(nptr->nrepl, replace);
	nptr->ndev = dev;
	restore(ps);
	return(OK);
}
