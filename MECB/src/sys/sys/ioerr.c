/* ioerr.c - ioerr */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  ioerr  -  return an error (used for "error" entries in devtab)
 *------------------------------------------------------------------------
 */
ioerr()
{
	return(SYSERR);
}
