/* x_echo.c - x_echo */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  x_echo  -  (command echo) echo arguments separated by blanks
 *------------------------------------------------------------------------
 */
COMMAND	x_echo(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	str[80];
	int	i;

	if (nargs == 1)
		str[0] = NULLCH;
	else {
		for (strcpy(str, args[1]),i=2 ; i<nargs ; i++) {
			strcat(str, " ");
			strcat(str, args[i]);
		}
	}
	strcat(str, "\n");
	write(stdout, str, strlen(str));
	return(OK);
}
