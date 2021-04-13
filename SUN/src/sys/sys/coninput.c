/* coninput.c - congetc conread */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  congetc -- read a character from the SUN physical keyboard
 *------------------------------------------------------------------------
 */
congetc(devptr )
struct	devsw	*devptr;
{
}


/*------------------------------------------------------------------------
 *  conread -- read from the SUN physical keyboard
 *------------------------------------------------------------------------
 */
conread(devptr, buff, count )
struct	devsw	*devptr;
char *buff;
int count;
{
}

