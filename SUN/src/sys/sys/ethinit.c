/* ethinit.c - ethinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <idprom.h>
#include <mmu.h>
#include <q.h>

struct idprom idprom;

#define	EMAXPAKREC (EMAXPAK+4)	/* maximum size of an ethernet packet 	*/
				/* that can be received from the lance	*/
				/* chip, includes 4 byte CRC		*/
				/* (not visible outside this file)	*/

int Asm_ethint();

/*------------------------------------------------------------------------
 *  ethinit  -  initialize ethernet I/O device and buffers
 *------------------------------------------------------------------------
 */
ethinit(devptr)
	struct	devsw	*devptr;
{
    struct	etblk	*etptr;
    struct	le_device *le;
    struct	le_init_block *ib;
    struct	le_md	(*pmdrecv)[];
    struct	le_md	*pmdxmit;
    struct	le_md	*pmd;
    struct	bre	*pbre;
    int		i;
    char	*pbufr;
    char	*pbuft;

#ifdef DEBUG
    kprintf("ethinit(dev) called\n");
#endif    
    etptr = &eth[devptr->dvminor];

    /* read the ID prom */
    getidprom(&idprom,sizeof (struct idprom));
    if (idprom.id_format != 1) {
	panic("Can't read ID prom\n");
    }

    le = (struct le_device *) devptr->dvcsr;
    ib = (struct le_init_block *) getdmem(sizeof(struct le_init_block));

    /* allocate the ring buffers */
    /* we fudge, because these MUST be on a 4 word boundry */
    pmdrecv = (struct le_md (*)[])
      ((getdmem(7+sizeof(struct le_md)*ENUMRCV)+7) & ~(unsigned long) 7);
    pmdxmit = (struct le_md *)
      ((getdmem(7+sizeof(struct le_md))+7) & ~(unsigned long) 7);

    /* allocate the LANCE buffers */
    pbufr = (char *) getdmem(ENUMRCV*EMAXPAKREC);
    pbuft = (char *) getdmem(EMAXPAK);

    /* zero-out the init block and ring pointers */
    bzero(ib,sizeof(struct le_init_block));
    for (i=0; i<ENUMRCV; ++i)
      bzero(&(*pmdrecv)[i],sizeof(struct le_md));
    bzero(pmdxmit,sizeof(struct le_md));

    /* set the physical address for this machine */
    ib->ib_padr[0] = idprom.id_ether[1]; /* must byte swap */
    ib->ib_padr[1] = idprom.id_ether[0];
    ib->ib_padr[2] = idprom.id_ether[3];
    ib->ib_padr[3] = idprom.id_ether[2];
    ib->ib_padr[4] = idprom.id_ether[5];
    ib->ib_padr[5] = idprom.id_ether[4];

    /* put my physical address into the eth structure */
    for (i=0; i<EPADLEN; ++i) 
      etptr->etpaddr[i] = idprom.id_ether[i];

    /* set up receive ring */
    ib->ib_rdrp.drp_len   = ENUMRCVL2;
    ib->ib_rdrp.drp_laddr = low16((long)pmdrecv&DMA_MASK); 
    ib->ib_rdrp.drp_haddr = hi8((long)pmdrecv&DMA_MASK);
#ifdef DEBUG    
    kprintf("chip: rdrp_laddr: 0x%04x\n",low16((long)pmdrecv&DMA_MASK));
    kprintf("chip: rdrp_haddr:   0x%02x\n",hi8((long)pmdrecv&DMA_MASK));
#endif	    
    
    /* set up xmit ring */
    ib->ib_tdrp.drp_len   = 0;	/* 0 means 2^^0 == 1 buffer in ring	*/
    ib->ib_tdrp.drp_laddr = low16((long)pmdxmit&DMA_MASK);
    ib->ib_tdrp.drp_haddr = hi8((long)pmdxmit&DMA_MASK);
#ifdef DEBUG    
    kprintf("chip: tdrp_laddr: 0x%04x\n",low16((long)pmdxmit&DMA_MASK));
    kprintf("chip: tdrp_haddr:   0x%02x\n",hi8((long)pmdxmit&DMA_MASK));
#endif	    

    /* set up the receive ring buffers */
    for (i=0; i<ENUMRCV; ++i) {
	/* set up the LANCE info */
	pmd = &(*pmdrecv)[i];
	pmd->lmd_ladr = low16((long)pbufr&DMA_MASK);
	pmd->lmd_hadr = hi8((long)pbufr&DMA_MASK);
	pmd->lmd_bcnt = -EMAXPAKREC;
	pmd->lmd_mcnt = 0;
	pmd->lmd_flags = LMD_OWN;
	
	/* set up the XINU info */
	pbre = &(etptr->etbrr[i]);
	pbre->pmd = pmd;
	pbre->buf = pbufr;
	pbre->flags = 0;
	
	pbufr += EMAXPAKREC;
    }
    
    /* set up the xmit ring buffers */
    /* set up the LANCE info */
    pmd = pmdxmit;
    pmd->lmd_ladr = low16((long)pbuft&DMA_MASK);
    pmd->lmd_hadr = hi8((long)pbuft&DMA_MASK);
    pmd->lmd_bcnt = -EMAXPAK;
    pmd->lmd_mcnt = 0;
    pmd->lmd_flags = 0;

    /* set up the XINU info */
    pbre = &etptr->etbrt[0];
    pbre->pmd = pmd;
    pbre->buf = pbuft;
    pbre->flags = 0;
	
    
#ifdef DEBUG
    kprintf("Lance device at address 0x%08lx\n",(unsigned long) le);
    kprintf("Interrupt vector: 0x%08lx\n",devptr->dvivec);
    kprintf("Init block at 0x%08lx\n",(unsigned long) ib);
    kprintf("  rladdr:0x%08lx   rhaddr:0x%08lx   len: %d\n",
	    ib->ib_rdrp.drp_laddr,
	    ib->ib_rdrp.drp_haddr,
	    ib->ib_rdrp.drp_len);
    kprintf("  tladdr:0x%08lx   thaddr:0x%08lx   len: %d\n",
	    ib->ib_tdrp.drp_laddr,
	    ib->ib_tdrp.drp_haddr,
	    ib->ib_tdrp.drp_len);
    kprintf("recv ring at 0x%08lx\n",(unsigned long) pmdrecv);
    kprintf("xmit ring at 0x%08lx\n",(unsigned long) pmdxmit);
    for (i=0; i<ENUMRCV; ++i) {
      kprintf("buf %d  pdm: 0x%08lx  buf: 0x%08lx-0x%08lx   flags: %d\n",
	      i,
	      etptr->etbrr[i].pmd,
	      etptr->etbrr[i].buf,
	      (long)etptr->etbrr[i].buf+EMAXPAKREC-1,
	      etptr->etbrr[i].flags);
	pmd = &(*pmdrecv)[i];
	kprintf("  ladr:0x%04x  hadr:0x%02x   bcnt:0x%x  mcnt:0x%x flags:%d\n",
		pmd->lmd_ladr,
		pmd->lmd_hadr,
		pmd->lmd_bcnt,
		pmd->lmd_mcnt,
		pmd->lmd_flags);
    }
    kprintf(" tbuf  pdm: 0x%08lx  buf: 0x%08lx-0x%08lx   flags: %d\n",
	    etptr->etbrt[0].pmd,
	    etptr->etbrt[0].buf,
	    (long)etptr->etbrr[i].buf+EMAXPAK-1,
	    etptr->etbrt[0].flags);
    
	pmd = pmdxmit;
	kprintf("  ladr:0x%04x  hadr:0x%02x   bcnt:0x%x  mcnt:0x%x flags:%d\n",
		pmd->lmd_ladr,
		pmd->lmd_hadr,
		pmd->lmd_bcnt,
		pmd->lmd_mcnt,
		pmd->lmd_flags);
    
    kprintf("\n");
#endif

    /* set up global information */
    devptr->dvioblk = (char *) etptr;
    etptr->etle   = le;
    etptr->etib   = ib;
    etptr->etdev  = devptr;
    etptr->etnextbuf = 0;
    etptr->etrsem = screate(1);
    etptr->etwsem = screate(1);
    etptr->etrpid = 0;

    /* init the shared queue */
    etptr->etrqhead = EMPTY;
    etptr->etrqtail = EMPTY;

    ethstrt(etptr);

    return(OK);
}

#ifdef	Neth
struct	etblk	eth[Neth];
#endif


printcsr0(le,msg)
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

#ifdef PRINT_CSR0_DETAILS
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
#endif    
}
