/* ethstrt.c - ethstrt */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <idprom.h>
#include <mmu.h>

int Asm_ethint();


/*------------------------------------------------------------------------
 *  ethstrt - start the LANCE ethernet chip
 *------------------------------------------------------------------------
 */
ethstrt(etptr)
	struct	etblk	*etptr;
{
    struct	le_device	*le;
    struct	devsw		*devptr;
    short	status;
    int		w;

    le = etptr->etle;
    devptr = etptr->etdev;
    
    /* freeze the chip */
    le->le_rap = LE_CSR0;
    le->le_rdp = LE_STOP;

    /* set the interrupt vector */
    set_evec( devptr->dvivec, Asm_ethint);

    /* freeze the chip */
    le->le_rap = LE_CSR0;
    le->le_rdp = LE_STOP;
	
    /* hand the init block to the chip */
    le->le_rap = LE_CSR1;
    le->le_rdp = low16((long)etptr->etib&DMA_MASK);
    le->le_rap = LE_CSR2;
    le->le_rdp = hi8((long)etptr->etib&DMA_MASK);
    
    le->le_rap = LE_CSR3;
    le->le_rdp = LE_BSWP;	/* set to byte swap	*/
    
#ifdef DEBUG
    printcsr0(le,"before init");
#endif    

    /* initialize the chip */
    le->le_rap = LE_CSR0;
    le->le_rdp = LE_INIT;

#ifdef DEBUG
    printcsr0(le,"just after init");
#endif    

    /* wait for init to complete */
    le->le_rap = LE_CSR0;
    status = le->le_rdp;
    for (w=10000; (w >= 0) && (!(status & LE_IDON)); --w) {
	if (w==0) {
	    printcsr0(le,"last condition");
	    panic("Unable to initialize Ethernet chip");
	}
	status = le->le_rdp;
    }

#ifdef DEBUG
    printcsr0(le,"init finished");
#endif    
    
    /* reset the interrupt */
    le->le_rap = LE_CSR0;
    le->le_csr = LE_IDON;

    /* start the chip */
    le->le_csr = LE_STRT | LE_INEA;

    return(OK);
}
