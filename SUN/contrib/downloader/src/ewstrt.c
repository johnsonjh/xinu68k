/* ewstrt.c - ewstrt */

#include <tftpload.h>
#include <ether.h>
#include <lereg.h>
#include <mmu.h>
#include <network.h>

extern struct etblk ethblock;
extern struct le_device *le;


/*
 *------------------------------------------------------------------------
 *  ewstrt - start an ethernet write operation on the LANCE
 *------------------------------------------------------------------------
 */

ewstrt(buf, len)
	char	*buf;
	int	len;
{
    struct	bre	*pbre;
    struct	le_md	*pmd;
    short	csr;

    /* tell the LANCE chip about the buffer */
    pbre = &ethblock.etbrt[0];
    pmd  = pbre->pmd;
    pmd->lmd_ladr = low16((long)buf&DMA_MASK);
    pmd->lmd_hadr =   hi8((long)buf&DMA_MASK);
    pmd->lmd_bcnt = -len;
    pmd->lmd_mcnt = 0;
    pmd->lmd_flags = LMD_OWN | LMD_STP | LMD_ENP;

    /* tell the chip to write! */
    le->le_csr = LE_TDMD;

    /* wait until the packet has been sent */
    while ( ((csr = le->le_rdp) & LE_TINT) == 0) {
	/* do nothing */;
    }

    /* reset the interrupt */
    le->le_csr = LE_TINT;
	    
    if ( (pmd->lmd_flags&LMD_ERR) != 0) {
#ifdef PRINTERRORS
	kprintf("ethinter: flags: 0x%x, retrying write\n",
		pmd->lmd_flags);
	kprintf("ethinter: flags3: 0x%x, retrying write\n",
		pmd->lmd_flags3);
#endif
	return(SYSERR);
    }

    return(OK);
}
