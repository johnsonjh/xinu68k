/* ethwstrt.c - ethwstrt */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  ethwstrt - start an ethernet write operation on the LANCE
 *------------------------------------------------------------------------
 */

ethwstrt(etptr, buf, len)
	struct	etblk	*etptr;
	char	*buf;
	int	len;
{
	struct	le_device	*le;
	struct	le_md		*pmdxmit;

#ifdef DEBUG
	kprintf("ethwstrt called\n");
#endif	

	le = etptr->etle;
	pmdxmit = eth->etbrt[0].pmd;

	etptr->etwbuf = buf;
	etptr->etwtry = EXRETRY;

	blkcopy(eth->etbrt[0].buf,buf,len);

	/* free the buffer that we were passed, we'll use a copy */
	freebuf( buf );

	pmdxmit->lmd_bcnt = -len;
	pmdxmit->lmd_mcnt = 0;
	pmdxmit->lmd_flags = LMD_OWN | LMD_STP | LMD_ENP;

        le->le_csr = LE_TDMD | LE_INEA;

}

