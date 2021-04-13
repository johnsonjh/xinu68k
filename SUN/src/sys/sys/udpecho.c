/* udpecho.c - udpecho */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#define	MAXECHO	600		/* maximum size of echoed datagram	*/
static	char	buff[MAXECHO];	/* here because the stack may be small	*/

/*------------------------------------------------------------------------
 *  udpecho  -  UDP echo server process (runs forever in background)
 *------------------------------------------------------------------------
 */
PROCESS	udpecho()
{
	int	dev, len;

	if ( (dev=open(INTERNET, ANYFPORT, UECHO)) == SYSERR) {
		printf("udpecho: open fails\n");
		return(SYSERR);
	}
	while ( TRUE ) {
		len = read(dev, buff, MAXECHO);
		write(dev, buff, len);
	}
}
