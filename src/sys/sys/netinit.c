/* netinit.c - netinit */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

#ifndef	RING0IN
#define RING0IN		-1
#endif
#ifndef	RING0OUT
#define	RING0OUT -1
#endif
#ifndef	RING1IN
#define RING1IN	 -1
#endif
#ifndef	RING1OUT
#define	RING1OUT -1
#endif
#ifndef	NNETS
#define	NNETS	 0
#endif

#define	MAXNETS	 2		/* maximum nets assigned in netdevs	*/
static	int	 netdevs[][MAXNETS]
			= {RING0IN,RING0OUT,RING1IN,RING1OUT};

/*------------------------------------------------------------------------
 *  netinit  --  iniitalize networks by starting frame level I/O
 *------------------------------------------------------------------------
 */
netinit()
{
	int	netid;

	if (NNETS > MAXNETS)
		panic("netinit - too few devices to netdevs table");
	frbpool = mkpool(FRLEN, NFRAMES);
	for (netid=0 ; netid<NNETS ; netid++)
		frinit(netid, netdevs[netid][0] , netdevs[netid][1]);
	return(OK);
}
