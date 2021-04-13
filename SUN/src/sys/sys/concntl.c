/* concntl.c - concntl */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  concntl  -  control a tty device by setting modes
 *------------------------------------------------------------------------
 */
concntl(devptr, func, addr)
struct	devsw	*devptr;
int func;
char *addr;
{
    return(SYSERR);
}
