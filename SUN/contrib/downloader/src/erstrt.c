/* erstrt.c - erstrt */

#include <tftpload.h>
#include <ether.h>
#include <lereg.h>
#include <mmu.h>
#include <network.h>


extern struct etblk ethblock;
extern struct le_device *le;

int lastbuffer = 0;

/*------------------------------------------------------------------------
 *  erstrt - synchronous read from the ethernet -- return a buffer addr
 *------------------------------------------------------------------------
 */
erstrt()
{
    struct	etblk	*etptr;
    struct	le_md	*pmd;
    struct	bre	*pbre;
    int		errs;
    short	csr;
    int		j;
    int		filledbuffer;
    char	*tmp;

    
    etptr = &ethblock;

    /* select Control reg 0 */
    le->le_rap = LE_CSR0;

    /* wait for a packet to arrive */
    while ( ((csr = le->le_rdp)&LE_RINT) == 0) {
	/* see if a hardware error has occurred */
	if ( (csr&LE_ERR) != 0) {
#ifdef PRINTERRORS
	    Printcsr0(le,"Erstrt error");
#endif			
	    /* reset the interrupt */
	    le->le_csr = LE_BABL|LE_CERR|LE_MISS|LE_MERR;
	}
    }
    
    /* reset the interrupt */
    le->le_csr = LE_RINT;

    /* look for oldest filled buffer */
    filledbuffer = SYSERR;
    for(j=0; j<ENUMRCV; ++j) {
	lastbuffer = (lastbuffer + 1) % ENUMRCV;
	pbre = &etptr->etbrr[lastbuffer];
	pmd  = pbre->pmd;
	if ((pmd->lmd_flags&LMD_OWN) == 0) {
	    if ((pmd->lmd_flags&LMD_ERR) == 0) {
		filledbuffer = lastbuffer;
#ifdef DEBUG
		kprintf("erstrt: buffer %d is filled\n",filledbuffer);
#endif		
		break;
	    }
	    else {		/* error, so retry operation */
#ifdef PRINTERRORS
		kprintf("ethinter: flags: 0x%x, retrying read\n",
			pmd->lmd_flags);
#endif				    
		pmd->lmd_flags = LMD_OWN;
		pmd->lmd_bcnt  = 0;
		return(SYSERR);
	    }
	}
    }

#ifdef DEBUG    
    if (filledbuffer == SYSERR) {
	panic("Received RINT, but found no packet");
    }
#endif    
    
    /* reset the interrupt */
    le->le_csr = LE_RINT|LE_INEA;

    /* remember a few pointers */
    pbre = &etptr->etbrr[filledbuffer];
    pmd  = pbre->pmd;
    
    /* check the status of the packet that came in */
    errs = pmd->lmd_flags & (RMD_FRAM|RMD_OFLO|RMD_CRC|RMD_BUFF);
    if ( errs != 0) {
#ifdef PRINTERRORS	
	if (errs & RMD_FRAM)
	    kprintf("erstrt: framing error\n");
	if (errs & RMD_OFLO)
	    kprintf("erstrt: buffer overflow\n");
	if (errs & RMD_CRC)
	    kprintf("erstrt: CRC error\n");
	if (errs & RMD_BUFF)
	    kprintf("erstrt: don't own next buffer\n");
#endif	
	return(SYSERR);
    }
    
    
#ifdef DEBUG
    kprintf("exchanging alt buffer: 0x%08lx for buffer %d, 0x%08lx\n",
	    etptr->etbrralt, filledbuffer,  pbre->buf);
#endif
	    
    /* exchange the alternate buffer for the one on the chip */
    tmp              = etptr->etbrralt;
    etptr->etbrralt  = pbre->buf;
    pbre->buf        = tmp;

    /* give the alternate buffer to the chip to use */
    pmd->lmd_flags = LMD_OWN;
    pmd->lmd_ladr  = low16((long)(pbre->buf)&DMA_MASK);
    pmd->lmd_hadr  =   hi8((long)(pbre->buf)&DMA_MASK);


#ifdef DEBUG
    kprintf("eread() returns packet 0x%08lx\n", etptr->etbrralt);
#endif    

    return((int) etptr->etbrralt);
}
