/* x_who.c - x_who */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  x_who  -  (command who) print name user is logged in under
 *------------------------------------------------------------------------
 */
COMMAND	x_who(stdin, stdout, stderr)
int	stdin, stdout, stderr;
{
	char	machine[32];
	char	str[80];

	if (marked(Shl.shmark) && Shl.shused) {
		getname(machine);
		sprintf(str, "%10s!%-10s Console : ", machine, Shl.shuser);
		write(stdout, str, strlen(str));
		ascdate(ut2ltim(Shl.shlogon), str);
		strcat(str, "\n");
		write(stdout, str, strlen(str));
	}
	return(OK);
}
