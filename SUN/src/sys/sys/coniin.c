/* coniin.c coniin, erase1, eputc, echoch */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <slu.h>
#include <zsreg.h>
#include <sunromvec.h>

#define prgetkey() (romp->v_getkey)()

/*------------------------------------------------------------------------
 *  coniin  --  lower-half con device driver for input interrupts
 *------------------------------------------------------------------------
 */
INTPROC	coniin()
{
    
	register struct	zscc_device *zptr;
	unsigned char	ch;

	ch = ((struct zscc_device *) 0xfe00004)->zscc_data;

	if (ch != 0x7f)
	    kprintf("<%x>\n", (unsigned int) ch);
	
	return;
}
