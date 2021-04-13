/* nammap.c - nammap */

#include <conf.h>
#include <kernel.h>
#include <name.h>

/*------------------------------------------------------------------------
 *  nammap  -  using namespace, iteratively map name onto (newname,device) 
 *------------------------------------------------------------------------
 */
SYSCALL	nammap(name, newname)
char	*name;
char	*newname;
{
	STATWORD ps;    
	int	dev;
	char	tmpnam[NAMLEN];

	disable(ps);
	dev = namrepl(name, newname);
	while (dev == NAMESPACE) {
		strcpy(tmpnam, newname);
		dev = namrepl(tmpnam, newname);
	}
	restore(ps);
	return(dev);
}
