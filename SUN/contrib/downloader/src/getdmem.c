/* getdmem.c - getdmem, initdmamem */

#include <tftpload.h>
#include <mmu.h>
#include <network.h>

long nextdma;
long dmaend;

/*------------------------------------------------------------------------
 * getdmem  --  allocate space from the DVMA address space that can be
 *              used to communicate with DVMA devices.  To simplify life,
 *              it always returns a block that starts on a longword boundary.
 *------------------------------------------------------------------------
 */
getdmem(nbytes)
    long nbytes;
{
    long ret;
    
    /* round the request up to an even word boundary */
    nbytes = (long) roundew(nbytes);

    if ((nextdma + nbytes) <= dmaend) {
	ret = nextdma;
	nextdma += nbytes;
	return(ret);
    }
    else {
	panic("No more memory for DMA buffers");
	return(SYSERR);
    }
}


/*------------------------------------------------------------------------
 * initdmamem -- initialize the Xinu DVMA address space.  Actually, we'll
 *               just use the monitor's DVMA memory.  This limits us to
 *               8k of DVMA space, but that's currently plenty.
 *------------------------------------------------------------------------
 */
initdmamem()
{
    dmaend  = 0x0ff01fff;
    nextdma = 0x0ff00000;
    return(OK);
}
