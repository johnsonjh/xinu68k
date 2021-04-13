/* openfile.c - openfile */

#include "fs.h"

/*------------------------------------------------------------------------
 *  openfile  -  open a file given its name and Xinu mode; return UNIX fd
 *------------------------------------------------------------------------
 */
openfile(name, xmode)
char	*name;
int	xmode;
{
	int	umode;
	int	fd;
	char	*subdir;
	char	fname[RNAMLEN];

	umode = fmode(xmode);
	if (xmode & FLOLD)
		return(open(name, umode, FILEMODE));
	for (subdir=index(name+1, '/') ; subdir != NULL ;
		subdir=index(subdir+1,'/')) {
		strncpy(fname, name, subdir - name);
		fname[subdir - name] = NULLCH;
		if (mkdir(fname, DIRMODE) < 0 && errno != EEXIST)
			return(SYSERR);
	}
	if ((fd=open(name, umode, FILEMODE)) >= 0)
		return(fd);
	return( open(name, umode&~O_CREAT, FILEMODE) );
}
