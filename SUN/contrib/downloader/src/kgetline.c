/* kgetline.c - kgetline */

#include <sunromvec.h>

#define prgetline()  (romp->v_getline)()
#define pribuf       ((char *) (romp->v_linebuf))


/*------------------------------------------------------------------------
 *  kgetline  --  return a pointer to a line of text from the selected
 *                device.  Uses a ROM routine.
 *------------------------------------------------------------------------
 */
kgetline()
{
	prgetline();
	return((int) pribuf);
}

