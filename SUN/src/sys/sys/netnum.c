/* netnum.c - netnum */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  netnum  -  obtain the network portion of a given IP address
 *------------------------------------------------------------------------
 */
netnum(netpart, address)
IPaddr	netpart;
IPaddr	address;
{
	blkcopy(netpart, address, IPLEN);
	switch (netpart[0] & IPTMASK) {
		case IPATYP: netpart[1] = '\0';	/* fall through */
		case IPBTYP: netpart[2] = '\0';	/* fall through */
		case IPCTYP: netpart[3] = '\0';
	}
	return(OK);
}
