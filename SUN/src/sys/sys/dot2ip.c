/* dot2ip.c - dot2ip */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dot2ip  -  convert network address from "decimal dot" form to IPaddr
 *------------------------------------------------------------------------
 */
dot2ip(ip, nad1, nad2, nad3, nad4)
char	*ip;
int	nad1, nad2, nad3, nad4;
{
	*ip++ = LOWBYTE & nad1;
	*ip++ = LOWBYTE & nad2;
	*ip++ = LOWBYTE & nad3;
	*ip++ = LOWBYTE & nad4;
}
