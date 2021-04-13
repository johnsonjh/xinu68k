/* test.c - main */

#include "../h/conf.h"
#include "../h/kernel.h"
#include "../h/deqna.h"
#include "../h/ether.h"

/*------------------------------------------------------------------------
 *  main  --  test ethernet by reading packets
 *------------------------------------------------------------------------
 */
main()
{
	char	buff[1600];
	int	i, len;
	char	*p, ch;
	struct	epacket	*epkptr;

	/* call buffer pool stuff so 'freebuf' can be mentioned */
	/* without generating a load-time error */
	mkpool(10,10);

	kprintf("Read from Ethernet\n");
	epkptr = (struct epacket *)buff;
	while (1) {
	    if ( (len=read(ETHER, buff, 1500)) == SYSERR) {
		kprintf("\nError\n");
	    } else if (epkptr->ep_hdr.e_ptype == EP_ARP) {
		/* do ARP */ kprintf("ARP...\n");
	    } else {
		p = (char *)buff;
		kprintf("\n LEN: %d\n  TO: ",len);
		for (i=0 ; i<6 ; i++)
			kprintf("%03o ", 0377& *p++);
		kprintf("\nFROM: ");
		for (i=0 ; i<6 ; i++)
			kprintf("%03o ", 0377& *p++);
		kprintf("\nTYPE: ");
		kprintf("%03o ", 0377 & *p++);
		kprintf("%03o ", 0377 & *p++);
		kprintf("\nDATA: ");
		for (i=0 ; i<60 ; i++)
			if ( printable(ch = *p++) )
				kprintf("%c",ch);
		kprintf("\n");
	    }
	}
}

/*------------------------------------------------------------------------
 *  printable - return TRUE iff characher is printable
 *------------------------------------------------------------------------
 */
printable(ch)
char	ch;
{
	return( (ch >= ' ') && (ch <= '~') );
}
