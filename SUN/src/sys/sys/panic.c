/*------------------------------------------------------------------------
 *	panic  --  panic and abort XINU
 *------------------------------------------------------------------------
 */

#include <kernel.h>    

panic (msg)
char *msg;
{
	STATWORD ps;    
	disable(ps);
	kprintf("Panic: %s\n", msg);
	ret_mon();
	restore(ps);
}

