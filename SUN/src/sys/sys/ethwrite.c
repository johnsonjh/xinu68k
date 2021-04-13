/* ethwrite.c - ethwrite */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  ethwrite - write a single packet to the ethernet
 *------------------------------------------------------------------------
 */

ethwrite(devptr, buff, len)
struct	devsw	*devptr;
char	*buff;
int	len;
{
	STATWORD ps;    
        struct	etblk	*etptr;

#ifdef DEBUG
	kprintf("ethwrite(devptr,0x%lx,%d) called\n", (unsigned long) buff, len);
#endif	

	if (len > EMAXPAK)
		return(SYSERR);
	if (len < EMINPAK)
		len = EMINPAK;
	
	etptr = (struct etblk *)devptr->dvioblk;
	blkcopy(((struct eheader *)buff)->e_src, etptr->etpaddr, EPADLEN);
	disable(ps);
	wait(etptr->etwsem);
	ethwstrt(etptr, buff, etptr->etlen = len);
	restore(ps);
	return(OK);
}
