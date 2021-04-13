/* x_snap.c - x_snap */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  x_snap  -  (command snap) write snapshot of memory to a core file
 *------------------------------------------------------------------------
 */
COMMAND	x_snap(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
    fprintf(stderr,"snap: snap will be rewritten when a future release\n");
    fprintf(stderr,"      determines the format of the core file\n");
    return(OK);
}
