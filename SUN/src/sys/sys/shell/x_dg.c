/* x_dg.c - x_dg */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  x_dg  -  (command dg) print info for currently open datagram devices
 *------------------------------------------------------------------------
 */
COMMAND	x_dg(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	struct	dgblk	*dgptr;
	char	str[80];
	int	i;

	for (i=0 ; i<Ndg ; i++) {
		dgptr = &dgtab[i];
		if (dgptr->dg_state == DG_FREE)
			continue;
		sprintf(str, "Dev=%2d: lport=%4d, fport=%4d, ",
			dgptr->dg_dnum, dgptr->dg_lport, dgptr->dg_fport);

		sprintf(&str[strlen(str)],
			"mode=%03o, xport=%2d addr=%d.%d.%d.%d\n",
				dgptr->dg_mode, dgptr->dg_xport,
				dgptr->dg_faddr[0] &0377,
				dgptr->dg_faddr[1] &0377,
				dgptr->dg_faddr[2] &0377,
				dgptr->dg_faddr[3] &0377);
		write(stdout, str, strlen(str));
	}
	return(OK);
}
