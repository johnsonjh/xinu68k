/* x_rm.c - x_rm */

#include <conf.h>
#include <kernel.h>
#include <file.h>

/*------------------------------------------------------------------------
 *  x_rm  -  (command rm) remove a file given its name
 *------------------------------------------------------------------------
 */
COMMAND	x_rm(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	if (nargs != 2) {
		fprintf(stderr, "usage: rm file\n");
		return(SYSERR);
	}
	if ( remove(args[1]) == SYSERR ) {
		fprintf(stderr, "Cannot remove %s\n", args[1]);
		return(SYSERR);
	}
	return(OK);
}
