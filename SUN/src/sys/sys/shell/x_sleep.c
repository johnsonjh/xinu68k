/* x_sleep.c - x_sleep */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_sleep  -  (command sleep) delay for a given number of seconds
 *------------------------------------------------------------------------
 */
COMMAND	x_sleep(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	if (nargs != 2) {
	        fprintf(stderr, "You gave me %d args\n",nargs);
		fprintf(stderr, "usage: sleep delay\n");
		return(SYSERR);
	}
	return( sleep( atoi(args[1]) ) );
}
