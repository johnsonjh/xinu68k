/* xdone.c - xdone */
#include <kernel.h>

/*------------------------------------------------------------------------
 * xdone  --  print system completion message as last process exits
 *------------------------------------------------------------------------
 */
xdone()
{
    kprintf("\n\nAll user processes have completed.\n\n");

#ifdef RET_WHEN_DONE    
	
    /* loop in case we <C>ontinue from the monitor */
    while (TRUE) {
	ret_mon();
        kprintf("\n\nNo Xinu user processes remaining.\n\n");
    }
#endif    
}
