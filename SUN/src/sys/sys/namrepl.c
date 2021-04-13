/* namrepl.c - namrepl */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  namrepl  -  using namespace, replace name with (newname,device)
 *------------------------------------------------------------------------
 */
SYSCALL	namrepl(name, newname)
char	*name;
char	*newname;
{
	STATWORD ps;    
	register struct	nament	*nptr;
	register struct	nament	*nlast;
	int	plen, rlen;

	disable(ps);
	nlast = &Nam.nametab[Nam.nnames];
	for (nptr= Nam.nametab ; nptr<nlast ; nptr++) {
		plen = strlen(nptr->npre);
		if (strncmp(nptr->npre, name, plen) == 0) {
			rlen = strlen(nptr->nrepl);
			if ((rlen+strlen(name)-plen) >= NAMLEN)
				break;
			strcpy(newname, nptr->nrepl);
			strcat(newname, name + plen);
			restore(ps);
			return(nptr->ndev);
		}
	}
	strcpy(newname, "");
	restore(ps);
	return(SYSERR);
}
