/* x_rf.c - x_rf */

#include <conf.h>
#include <kernel.h>
#include <fserver.h>
#include <rfile.h>

/*------------------------------------------------------------------------
 *  x_rf  -  (command rf) format and print remote file status
 *------------------------------------------------------------------------
 */
COMMAND	x_rf(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	struct	rfblk	*rfptr;
	char	str[80];
	int	i;

	sprintf(str, "Remote files: server on dev=%d, server mutex=%d\n",
			Rf.device, Rf.rmutex);
	write(stdout, str, strlen(str));
	for (i=0 ; i<Nrf; i++) {
		if ((rfptr = &Rf.rftab[i])->rf_state == RFREE)
			continue;
		sprintf(str,
			" %2d. name=%-20s, pos=%6D, mode=%03o, sem=%d\n",
				rfptr->rf_dnum, rfptr->rf_name,
				rfptr->rf_pos,  rfptr->rf_mode,
				rfptr->rf_mutex);
		write(stdout, str, strlen(str));
	}
	return(OK);
}
