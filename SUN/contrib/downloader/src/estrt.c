/* ethstrt.c - ethstrt */

#include <tftpload.h>
#include <mmu.h>
#include <lereg.h>
#include <ether.h>
#include <network.h>

extern struct etblk ethblock;
extern struct le_device *le;


/*------------------------------------------------------------------------
 *  ethstrt - start the LANCE ethernet chip
 *------------------------------------------------------------------------
 */
ethstrt(etptr)
	struct	etblk	*etptr;
{
    short	status;
    int		w;
    int		i;

    /* freeze the chip */
    for (i=0; i <= 1; ++i) {   /* twice */
	le->le_rap = LE_CSR0;
	le->le_rdp = LE_STOP;
    }

    /* hand the init block to the chip */
    le->le_rap = LE_CSR1;
    le->le_rdp = low16((long)etptr->etib&DMA_MASK);
    le->le_rap = LE_CSR2;
    le->le_rdp =   hi8((long)etptr->etib&DMA_MASK);
    
    /* set to byte swap	*/
    le->le_rap = LE_CSR3;
    le->le_rdp = LE_BSWP;
    
#ifdef DEBUG
    Printcsr0(le,"before init");
#endif    

    /* initialize the chip */
    le->le_rap = LE_CSR0;
    le->le_rdp = LE_INIT;

#ifdef DEBUG
    Printcsr0(le,"just after init");
#endif    

    /* wait for init to complete */
    le->le_rap = LE_CSR0;
    status = le->le_rdp;
    for (w=10000; (w >= 0) && (!(status & LE_IDON)); --w) {
	if (w==0) {
#ifdef DEBUG		
	    Printcsr0(le,"last condition");
#endif	    
	    panic("Unable to initialize Ethernet chip");
	}
	status = le->le_rdp;
    }

#ifdef DEBUG
    Printcsr0(le,"init finished");
#endif    
    
    /* reset the interrupt */
    le->le_rap = LE_CSR0;
    le->le_csr = LE_IDON;

    /* start the chip */
    le->le_csr = LE_STRT;

    return(OK);
}



#ifdef DEBUG

Printcsr0(le,msg)
     struct le_device *le;
     char *msg;
{
    register short status;

    le->le_rap = LE_CSR0;
    status = le->le_rdp;
  
    kprintf("%s:  (%04b %04b %04b %04b)\n",
	    msg,
	    (int) ((status >>12) & 0x0f),
	    (int) ((status >>8) & 0x0f),
	    (int) ((status >>4) & 0x0f),
	    (int) (status & 0x0f)
	    );

    if (status & LE_ERR	)
	kprintf("    CSR0: BABL | CERR | MISS | MERR\n");
    if (status & LE_INTR)
	kprintf("    CSR0: BABL|MISS|MERR|RINT|TINT|IDON\n");
    if (status & LE_BABL)
	kprintf("    CSR0: transmitted too many bits\n");
    if (status & LE_CERR)
	kprintf("    CSR0: No Heartbeat\n");
    if (status & LE_MISS)
	kprintf("    CSR0: Missed an incoming packet\n");
    if (status & LE_MERR)
	kprintf("    CSR0: Memory Error; no acknowledge\n");
    if (status & LE_RINT)
	kprintf("    CSR0: Received packet Interrupt\n");
    if (status & LE_TINT)
	kprintf("    CSR0: Transmitted packet Interrupt\n");
    if (status & LE_IDON)
	kprintf("    CSR0: Initialization Done\n");
    if (status & LE_INEA)
	kprintf("    CSR0: Interrupt Enable\n");
    if (status & LE_RXON)
	kprintf("    CSR0: Receiver On\n");
    if (status & LE_TXON)
	kprintf("    CSR0: Transmitter On\n");
    if (status & LE_TDMD)
	kprintf("    CSR0: Transmit Demand (send it now)\n");
    if (status & LE_STOP)
	kprintf("    CSR0: Stop\n");
    if (status & LE_STRT)
	kprintf("    CSR0: Start\n");
    if (status & LE_INIT)
	kprintf("    CSR0: Initialize\n");
}

#endif
