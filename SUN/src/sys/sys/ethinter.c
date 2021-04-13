/* ethinter.c - ethinter */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <q.h>


/*------------------------------------------------------------------------
 *  ethinter - ethernet interrupt processing procedure
 *
 *   Interrupts are disabled before this routine is called
 *------------------------------------------------------------------------
 */
INTPROC	ethinter(etptr)
     struct	etblk	*etptr;
{
    register struct le_device *le;
    int i;
    struct	le_md	*pmd;
    struct	bre	*pbre;
    short	csr;
    int	pid;
    int j;
    Bool doresch;

    le = etptr->etle;
    doresch = FALSE;

    le->le_rap = LE_CSR0;

#ifdef DEBUG
    printcsr0(le,"LEINTER");
#endif

    /* see if a hardware error has occurred */
    if ( ((csr = le->le_rdp)&LE_ERR) != 0) {
	if (csr&LE_BABL) {
#ifdef PRINTERRORS
	    printcsr0(le,"LEerr: Babbling xmitter");
#endif			
	    ++Net.nerror;
	}
	if (csr&LE_CERR) {
#ifdef PRINTERRORS
	    printcsr0(le,"LEerr: Collision error");
#endif			
	    ++Net.nerror;
	}
	if (csr&LE_MISS) {
#ifdef PRINTERRORS
	    printcsr0(le,"LEerr: Missed packet");
#endif			
	    ++Net.nmiss;
	}
	if (csr&LE_MERR) {
#ifdef PRINTERRORS
	    printcsr0(le,"LEerr: Memory error");
#endif			
	    ++Net.nerror;
	}
	/* reset the interrupt */
	le->le_csr = LE_BABL|LE_CERR|LE_MISS|LE_MERR|LE_INEA;
	++Net.ndrop;
    }

    /* check to see if a receive has completed */
    if ( (le->le_rdp & LE_RINT) != 0) {
	/* the chip won't tell us which buffer it filled in */
	/* But, etnextbuf always points to the next buf after */
	/* the last buffer we examined */

	pbre = &etptr->etbrr[etptr->etnextbuf];
	pmd  = pbre->pmd;
	for (;
	     ((pmd->lmd_flags&LMD_OWN) == 0) && (pbre->flags == 0);
	     etptr->etnextbuf = (etptr->etnextbuf + 1) % ENUMRCV,
	     pbre = &etptr->etbrr[etptr->etnextbuf],
	     pmd  = pbre->pmd) {

	    /* reset the interrupt */
	    le->le_csr = LE_RINT | LE_INEA;

	    if ((pmd->lmd_flags&LMD_ERR) == 0) {
		pbre->flags = 1;

#ifdef DEBUG
		kprintf("recv'd %d\n",etptr->etnextbuf);
#endif

		/* put this new packet at the end of the packet */
		/* receive queue */
		etptr->etrqnext[etptr->etnextbuf]  = EMPTY;
		if (etptr->etrqhead == EMPTY) {  /* only one */
		    etptr->etrqhead = etptr->etnextbuf;
		    etptr->etrqtail = etptr->etnextbuf;
		} else {                     /* put at the end */
		    etptr->etrqnext[etptr->etrqtail] = etptr->etnextbuf;
		    etptr->etrqtail = etptr->etnextbuf;
		}

		/* wake up the reading process, if there is one */
		if (! isbadpid(etptr->etrpid)) {
		    if (proctab[etptr->etrpid].pstate == PRSUSP) {
			ready(etptr->etrpid, RESCHNO);
			etptr->etrpid = BADPID;
#ifdef DEBUG
			kprintf("ethinter: readying proc %d\n",
				etptr->etrpid);
#endif
			doresch = TRUE;
		    }
		}
		
	    }
	    else {	/* error, so retry operation */
#ifdef PRINTERRORS
		kprintf("ethinter: flags: 0x%x, retrying read\n",
			pmd->lmd_flags);
#endif				    
		pmd->lmd_flags = LMD_OWN;
		pmd->lmd_bcnt  = 0;
		break;
	    }
	}
    }

    if ( ((csr = le->le_rdp) & LE_TINT) != 0) {
	/* reset the interrupt */
	le->le_csr = LE_TINT|LE_INEA;
	
	pbre = &etptr->etbrt[0];
	pmd  = pbre->pmd;
	if ( (pmd->lmd_flags&LMD_ERR) == 0) {
	    etptr->etlen = 0;
	    doresch = FALSE;
	    signal(etptr->etwsem);
	} else if (etptr->etwtry-- > 0) { /* retry on error */
#ifdef PRINTERRORS
	    kprintf("ethinter: flags: 0x%x, retrying write\n",
		    pmd->lmd_flags);
	    kprintf("ethinter: flags3: 0x%x, retrying write\n",
		    pmd->lmd_flags3);
#endif			    
	    pmd->lmd_flags = LMD_OWN;
	    pmd->lmd_bcnt  = 0;
	    pmd->lmd_flags = LMD_OWN | LMD_STP | LMD_ENP;
	    le->le_csr = LE_TDMD | LE_INEA;
	} else {
#ifdef PRINTERRORS
	    kprintf("ethinter: too many retries\n");
#endif		
	    doresch = FALSE;
	    signal(etptr->etwsem);
	}
    }

    if (doresch) {
#ifdef DEBUG
	kprintf("ethinter: calling resched()\n");
#endif
	resched();
    }
}
