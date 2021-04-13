/* getnet.c - getnet */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  getnet  -  obtain the network portion of this system's IP address
 *------------------------------------------------------------------------
 */
SYSCALL	getnet(address)
IPaddr	address;
{
	if (!Net.mavalid && getaddr(address) == SYSERR)
			return(SYSERR);
	blkcopy(address, Net.mynet, IPLEN);
	return(OK);
}
