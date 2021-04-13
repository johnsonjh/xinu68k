/* ethread.c - ethread */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <q.h>

/*------------------------------------------------------------------------
 *  ethread - read a single packet from the ethernet
 *------------------------------------------------------------------------
 */
ethread(devptr, buff, len)
struct	devsw	*devptr;
char	*buff;
int	len;
{
    STATWORD ps;    
    char	*wbuff;
    struct	etblk	*etptr;
    struct	le_device	*le;
    struct	le_md	*pmd;
    struct	bre	*pbre;
    int errs;
    int readlen;
    int packet;

#ifdef DEBUG
    kprintf("ethread() called\n");
#endif    

    etptr = (struct etblk *) devptr->dvioblk;
    le = etptr->etle;

    /* wait for exclusive read access */
    wait(etptr->etrsem);
    
    disable(ps);

    /* wait for a packet to arrive */
    while (etptr->etrqhead == EMPTY) {
#ifdef DEBUG
        kprintf("ethread: no packet available, calling suspend\n");
#endif    
        etptr->etrpid = currpid;
        suspend(currpid);
        etptr->etrpid = BADPID;
#ifdef DEBUG	
        if (etptr->etrqhead == EMPTY)
          kprintf("ethread: OOPS! head still 0!!!\n");
#endif	
    }

    /* remove the first packet on the queue */
    packet = etptr->etrqhead;
    etptr->etrqhead = etptr->etrqnext[etptr->etrqhead];

#ifdef DEBUG
    kprintf("ethread: received packet %d\n",packet);
#endif    

    pbre = &etptr->etbrr[packet];
    pmd = pbre->pmd;
    errs = pmd->lmd_flags & (RMD_FRAM|RMD_OFLO|RMD_CRC|RMD_BUFF);
    if ( errs != 0) {
#ifdef PRINTERRORS	
	if (errs & RMD_FRAM)
	    kprintf("ethread: framing error\n");
	if (errs & RMD_OFLO)
	    kprintf("ethread: buffer overflow\n");
	if (errs & RMD_CRC)
	    kprintf("ethread: CRC error\n");
	if (errs & RMD_BUFF)
	    kprintf("ethread: don't own next buffer\n");
#endif	
	len = SYSERR;
    } else {
	readlen = pmd->lmd_mcnt;
	readlen -= 4;      /* subtract the 4 byte CRC at the end */
	if (readlen > len)
	    len = SYSERR;
	else {
	    blkcopy(buff,pbre->buf,readlen);
	    len = readlen;
	}
    }

    /* give this buffer back to the ethernet chip */
    pmd->lmd_flags = LMD_OWN;
    pbre->flags = 0;

    signal(etptr->etrsem);
    
    restore(ps);
    return(len);
}
