/* x_exit.c - x_exit */

#include <conf.h>
#include <kernel.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  x_exit  -  (builtin command exit) exit from the shell
 *------------------------------------------------------------------------
 */
BUILTIN	x_exit(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	return(SHEXIT);
}
