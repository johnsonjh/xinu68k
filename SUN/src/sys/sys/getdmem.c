/* getdmem.c - getdmem initdmamem */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <mmu.h>
#include <mem.h>
#include <network.h>

long nextdma;
long dmaend;

MARKER dmamark;


/*------------------------------------------------------------------------
 * getdmem  --  allocate space from the DVMA address space that can be
 *              used to communicate with DVMA devices.  To simplify life,
 *              it always returns a block that starts on a longword boundary.
 *------------------------------------------------------------------------
 */
SYSCALL getdmem(nbytes)
    long nbytes;
{
    long ret;
    
#ifdef DEBUG
    kprintf("getdmem(%ld) called\n",nbytes);
#endif

    if (unmarked(dmamark)) {
	mark(dmamark);
	initdmamem(DMA_NBYTES);
        /* round the beginning up to an even word boundary */
        nextdma = (long) roundmb(nextdma);
    }

    /* round the request up to an even word boundary */
    nbytes = (long) roundmb(nbytes);

    if ((nextdma + nbytes) <= dmaend) {
	ret = nextdma;
	nextdma += nbytes;
#ifdef DEBUG
	kprintf("getdmem(%ld) returns 0x%08lx\n",nbytes,ret);
#endif    
	return(ret);
    }
    else {
	panic("No more memory for DMA buffers");
	return(SYSERR);
    }
}


/*------------------------------------------------------------------------
 * initdmamem -- initialize the Xinu DVMA address space.  Allocate nbytes
 *               from the Xinu heap.  Then steal the page frames used for
 *               those pages and use them to map addresses in the DVMA range.
 *------------------------------------------------------------------------
 */
initdmamem(dmasize)
     long dmasize;
{
    long heapmem;
    long dmamem;
    union pte pte;
    union pte dma_pte;

#ifdef DEBUG
    kprintf("initdmamem(%ld) called\n",dmasize);
#endif

    /* On the Sun3, DMA space must be in the virtual range		*/
    /* 0ff00000	to 0fffffff.  That memory is already mapped, but there	*/
    /* are no memory frames associated with it.  We will allocate space	*/
    /* from the Xinu heap, and then use those frames for the DMA space.	*/

    /* allocate enough heap, plus extra to page allign */
    heapmem = getmem(dmasize + PGSIZ);
#ifdef DEBUG
    kprintf("Getmem returns 0x%lx\n",(unsigned long) heapmem);
#endif

    /* bump the space received up to the next page boundary */
    heapmem = uptopage(heapmem);
#ifdef DEBUG
    kprintf("heapmem rounded up to 0x%lx\n",(unsigned long) heapmem);
#endif

    /* set up the default protection for the DMA pages */
    dma_pte.bits.hvalid   = 1;	/* the page is valid			*/
    dma_pte.bits.prot     = 2;	/* writeable by all			*/
    dma_pte.bits.cache    = 1;	/* don't cache DMA pages!		*/
    dma_pte.bits.type     = ONBOARD_MEM;

    /* step through each page to be mapped, and make the DMA space 	*/
    /* use the frame we just took from the heap.			*/
    for (dmamem = DMA_VM_START; dmamem < DMA_VM_START + dmasize;
	 dmamem += PGSIZ, heapmem += PGSIZ) {
	pte.value = GetPageMap(heapmem);
	dma_pte.bits.pfn = pte.bits.pfn;
#ifdef DEBUG
	kprintf("SetPageMap(0x%08lx, 0x%08lx)\n",dmamem,dma_pte.value);
#endif	
	SetPageMap(dmamem,dma_pte.value);
    }
    
    dmaend  = DMA_VM_START + dmasize - 1;
    nextdma = DMA_VM_START;
    return(OK);
}
