/* dgparse.c - dgparse */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ctype.h>

/*------------------------------------------------------------------------
 *  dgparse  -  parse foreign address specification; get IP and port #s
 *------------------------------------------------------------------------
 */
dgparse(dgptr, fspec)
struct	dgblk	*dgptr;
char	*fspec;
{
	int	i, byte;
	char	ch;
	char	*ipptr;

	if (fspec == ANYFPORT) {
		dgptr->dg_fport = 0;
		return(OK);
	}

	/* parse forms like 192.5.48.30:3 into (ip-address,udp-port) */

	ipptr = (char *) dgptr->dg_faddr;
	for (i=0 ; i<4 ; i++) {
		byte = 0;
		while ( isdigit(ch = *fspec++) )
			byte = 10*byte + (ch - '0');
		if (byte > 256 || (i<3 && ch!='.') )
			return(SYSERR);
		*ipptr++ = (char)byte;
	}
	if (ch != ':')
		return(SYSERR);
	i = 0;
	while ( isdigit(ch = *fspec++) )
		i = 10*i + (ch - '0');
	if (i==0 || ch!='\0')
		return(SYSERR);
	dgptr->dg_fport = i;
	return(OK);
}
