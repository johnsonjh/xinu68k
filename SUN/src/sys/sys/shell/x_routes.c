/* x_routes.c - x_routes */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

LOCAL	char	st[] = "F?GD";

/*------------------------------------------------------------------------
 *  x_routes  -  (command routes) format and print routing cache entries
 *------------------------------------------------------------------------
 */
COMMAND	x_routes(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	arindex;
	char	str[80];
	struct	arpent	*atabptr;

	if (nargs > 1) {
		Arp.atabsiz = Arp.atabnxt = 0;
		return(OK);
	}
	sprintf(str, "Routing cache: size=%d, next=%d\n",
		Arp.atabsiz, Arp.atabnxt);
	write(stdout, str, strlen(str));
	for (arindex=0; arindex<Arp.atabsiz; arindex++) {
		atabptr = &Arp.arptab[arindex];
		if (atabptr->arp_state == AR_FREE)
			continue;
		sprintf(str, " %2d. Route=%c, Dev=%2d",
			arindex,st[atabptr->arp_state],atabptr->arp_dev);
		sprintf(&str[strlen(str)],
			" IPaddr=%03d.%03d.%03d.%03d, ",
				atabptr->arp_Iad[0] & 0377,
				atabptr->arp_Iad[1] & 0377,
				atabptr->arp_Iad[2] & 0377,
				atabptr->arp_Iad[3] & 0377);
		sprintf(&str[strlen(str)],
			"Ether addr=%02x%02x.%02x%02x.%02x%02x\n",
				atabptr->arp_Ead[0]&0377,
				atabptr->arp_Ead[1]&0377,
				atabptr->arp_Ead[2]&0377,
				atabptr->arp_Ead[3]&0377,
				atabptr->arp_Ead[4]&0377,
				atabptr->arp_Ead[5]&0377 );
		write(stdout, str, strlen(str));
	}
	return(OK);
}
