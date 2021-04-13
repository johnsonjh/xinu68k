/* einit.c - einit */

#include <tftpload.h>
#include <idprom.h>
#include <mmu.h>
#include <ether.h>
#include <lereg.h>
#include <network.h>

struct idprom idprom;

struct etblk ethblock;
struct le_device *le;

extern Eaddr	myeaddr;

/*------------------------------------------------------------------------
 *  einit  -  initialize ethernet I/O device and buffers
 *------------------------------------------------------------------------
 */
einit()
{
    struct	etblk	*etptr;
    struct	le_init_block *ib;
    struct	le_md	(*pmdrecv)[];
    struct	le_md	*pmdxmit;
    struct	le_md	*pmd;
    struct	bre	*pbre;
    int		i;

    etptr = &ethblock;
    le    = (struct le_device *) 0x0fe10000;

    /* read the ID prom */
    getidprom(&idprom,sizeof (struct idprom));
    if (idprom.id_format != 1) {
	panic("Can't read ID prom\n");
    }

    ib = (struct le_init_block *) getdmem(sizeof(struct le_init_block));

    /* allocate the ring buffers */
    /* we fudge, because these MUST be on a 4 word boundry */
    pmdrecv = (struct le_md (*)[])
      ((getdmem(7+sizeof(struct le_md)*ENUMRCV)+7) & ~(unsigned long) 7);
    pmdxmit = (struct le_md *)
      ((getdmem(7+sizeof(struct le_md))+7) & ~(unsigned long) 7);

    /* allocate the LANCE receive buffers */
    for (i=0; i < ENUMRCV; ++i) {
	etptr->etbrr[i].buf = (char *) getdmem(EMAXPAK);
    }
    
    /* allocate the alternate receive buffer */
    etptr->etbrralt = (char *) getdmem(EMAXPAK);

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
	myeaddr[i] = idprom.id_ether[i];

    /* set up receive ring */
    ib->ib_rdrp.drp_len   = ENUMRCVL2;
    ib->ib_rdrp.drp_laddr = low16((long)pmdrecv&DMA_MASK); 
    ib->ib_rdrp.drp_haddr =   hi8((long)pmdrecv&DMA_MASK);
#ifdef DEBUG    
    kprintf("chip: receive ring (low): 0x%04x\n",low16((long)pmdrecv&DMA_MASK));
    kprintf("chip: receive ring (hi):    0x%02x\n",hi8((long)pmdrecv&DMA_MASK));
#endif	    
    
    /* set up xmit ring */
    ib->ib_tdrp.drp_len   = 0;	/* 0 means 2^^0 == 1 buffer in ring	*/
    ib->ib_tdrp.drp_laddr = low16((long)pmdxmit&DMA_MASK);
    ib->ib_tdrp.drp_haddr =   hi8((long)pmdxmit&DMA_MASK);
#ifdef DEBUG    
    kprintf("chip: xmit ring (low): 0x%04x\n",low16((long)pmdxmit&DMA_MASK));
    kprintf("chip: xmit ring (hi):    0x%02x\n",hi8((long)pmdxmit&DMA_MASK));
#endif	    

    /* set up the receive ring buffers */
    /* set up the LANCE info */
    for (i=0; i<ENUMRCV; ++i) {
	/* set up the LANCE info */
	pmd = &(*pmdrecv)[i];
	pbre = &(etptr->etbrr[i]);
	
	pmd->lmd_ladr = low16((long)pbre->buf&DMA_MASK);
	pmd->lmd_hadr =   hi8((long)pbre->buf&DMA_MASK);
	pmd->lmd_bcnt = -EMAXPAK;
	pmd->lmd_mcnt = 0;
	pmd->lmd_flags = LMD_OWN; /* belongs to the LANCE */
	
	/* set up the XINU info */
	pbre->pmd = pmd;
	pbre->flags = 0;
    }
    
    /* set up the xmit ring buffers */
    /* set up the LANCE info */
    pmd = pmdxmit;
    pmd->lmd_ladr = 0;
    pmd->lmd_hadr = 0;
    pmd->lmd_bcnt = -EMAXPAK;
    pmd->lmd_mcnt = 0;
    pmd->lmd_flags = 0;

    /* set up the XINU info */
    pbre = &etptr->etbrt[0];
    pbre->pmd = pmd;
    pbre->buf = 0;
    pbre->flags = 0;
	
    
#ifdef DEBUG
    kprintf("Lance device at 0x%08lx\n",(unsigned long) le);
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

    /* print each recv buffer */
    for (i=0; i<ENUMRCV; ++i) {
	kprintf("rbuf[%d]  pdm: 0x%08lx  buf: 0x%08lx-0x%08lx\n",
		i,
		etptr->etbrr[i].pmd,
		etptr->etbrr[i].buf,
		(long)etptr->etbrr[i].buf+EMAXPAK-1);
	pmd = &(*pmdrecv)[i];
	kprintf("    ladr:0x%04x  hadr:0x%02x   bcnt:0x%x  mcnt:0x%x flags:%d\n",
		pmd->lmd_ladr,
		pmd->lmd_hadr,
		pmd->lmd_bcnt,
		pmd->lmd_mcnt,
		pmd->lmd_flags);
    }

    /* print out the alternate receive buffer */
    kprintf(" alternate rbuf  buf: 0x%08lx-0x%08lx\n",
	    etptr->etbrralt,
	    (long)etptr->etbrralt+EMAXPAK-1);
    
    /* print out the xmit buffer */
    kprintf("tbuf[0]  pdm: 0x%08lx  buf: 0x%08lx-0x%08lx   flags: %d\n",
	    etptr->etbrt[0].pmd,
	    etptr->etbrt[0].buf,
	    (long)etptr->etbrr[i].buf+EMAXPAK-1,
	    etptr->etbrt[0].flags);
    
    pmd = pmdxmit;
    kprintf("    ladr:0x%04x  hadr:0x%02x   bcnt:0x%x  mcnt:0x%x flags:%d\n",
	    pmd->lmd_ladr,
	    pmd->lmd_hadr,
	    pmd->lmd_bcnt,
	    pmd->lmd_mcnt,
	    pmd->lmd_flags);
    
    kprintf("\n");
#endif

    etptr->etle   = le;
    etptr->etib   = ib;
    
    ethstrt(etptr);

    return(OK);
}
