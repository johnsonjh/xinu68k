/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <deqna.h>
#include <ether.h>
#include <ip.h>
#include <dgram.h>
#include <net.h>


/*------------------------------------------------------------------------
 *  main  --  UDP echo server that listens on port 7 and 
 *------------------------------------------------------------------------
 */
main()
{
	int	echodev;
	int	len;
	struct	xdgram	buff;

	printf("UDP Echo server at work...\n");

	echodev = open(NETWORK, ANYFPORT, UECHO);

	while (TRUE) {
		len = read(echodev, &buff, sizeof(struct xdgram));
		if (len == SYSERR)
			continue;
		write(echodev, &buff, len);
	}
}
