/* eread.c - eread */

#include <tftpload.h>

/*------------------------------------------------------------------------
 *  eread - synchronous read from the ethernet -- return a buffer addr
 *------------------------------------------------------------------------
 */
eread()
{
    long packet;

    while ((packet = erstrt()) == SYSERR) {
#ifdef PRINTERRORS
	kprintf("eread: got error from erstrt, recalling\n");
#endif
	/* do nothing */;
    }

    return((int) packet);
}
