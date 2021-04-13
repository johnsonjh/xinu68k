/* x_date.c - x_date */

#include <conf.h>
#include <kernel.h>
#include <sleep.h>
#include <date.h>

/*------------------------------------------------------------------------
 *  x_date  -  (command date) print the date and time
 *------------------------------------------------------------------------
 */
COMMAND	x_date(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	long	now;
	char	str[80];

	if (nargs == 2)
		clktime = 0L;
	gettime(&now);
	ascdate(now, str);
	strcat(str, "\n");
	write(stdout, str, strlen(str));
	return(OK);
}
