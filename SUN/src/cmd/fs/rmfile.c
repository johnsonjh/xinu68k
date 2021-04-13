/* rmfile.c - rmfile */

#include "fs.h"

/*------------------------------------------------------------------------
 *  rmfile  -  remove a file given its name and delete cache entry
 *------------------------------------------------------------------------
 */
rmfile(name)
char	*name;
{
	char	fname[RNAMLEN];
	char	*subdir;

	if (unlink(name) < 0)
		return(SYSERR);
	strncpy(fname, name, sizeof(fname));
	fname[sizeof(fname)-1] = NULLCH;
	while ( (subdir=rindex(fname,'/')) != NULL && subdir != fname) {
		*subdir = NULLCH;
		if (rmdir(fname) < 0)
			return(OK);
	}
	return(OK);
}
