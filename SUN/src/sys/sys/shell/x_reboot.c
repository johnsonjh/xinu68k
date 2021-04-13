/* x_reboot.c - x_reboot */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_reboot  -  (builtin command reboot) restart the system from scratch
 *------------------------------------------------------------------------
 */
BUILTIN	x_reboot(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	ret_mon();	/* warning! This terminates everything. */
}
