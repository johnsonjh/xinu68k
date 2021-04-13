/*------------------------------------------------------------------------
 *	panic  --  panic and abort XINU
 *------------------------------------------------------------------------
 */


/* the monitor prom gives the address of the routine we should use to	*/
/* return to the prom monitor						*/   
/* sorry about the Warning, I can't make a cast work here :-(  */
int (**exit_to_mon)() = 0x0fef00c4;

panic (msg)
char *msg;
{
    kprintf("Panic: %s\n", msg);
    (**exit_to_mon)();
}

stopnow()
{
    (**exit_to_mon)();
}
