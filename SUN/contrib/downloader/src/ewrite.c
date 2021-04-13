/* ewrite.c - ewrite */

#include <tftpload.h>
#include <ether.h>

extern struct etblk ethblock;
extern Eaddr myeaddr;


/*
 *------------------------------------------------------------------------
 *  ewrite - write a single packet to the ethernet.  The buffer passed
 *           MUST be in DVMA space.
 *------------------------------------------------------------------------
 */
ewrite(buf, len)
char	*buf;
int	len;
{
    
#ifdef DEBUG
    kprintf("ewrite(buf: 0x%08lx,  len: %d) called\n",
	    (unsigned long) buf, len);
#endif

    if ((int) buf < 0x0ff00000) {
#ifdef DEBUG	
	kprintf("ewrite: buf is not in DVMA space\n");
#endif	
	return(SYSERR);
    }

    if (len > EMAXPAK)
	return(SYSERR);
	
    if (len < EMINPAK)
	len = EMINPAK;

    /* copy in the source ethernet address */
    blkcopy(((struct eheader *)buf)->e_src, myeaddr, EPADLEN);

    while (ewstrt(buf, len) == SYSERR) {
#ifdef PRINTERRORS
	kprintf("ewrite: got error from ewstrt, recalling\n");
#endif
	/* do nothing */;
    }
    
    return(OK);
}
