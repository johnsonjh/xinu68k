#include "slu7201.h"

/*------------------------------------------------------------------------
 *  kgetc  --  read a character from the console (or other SLU)
 *  using polled I/O
 *------------------------------------------------------------------------
 */
kgetc7201(csrptr)
struct csr *csrptr;	/* address of device csr (0x10040 for console)*/
{
  int c;

	while ( (csrptr->cstat & SLURECVREADY) == 0 ) ;	/* poll for char*/
	c = (csrptr->cbuf&SLUCHMASK); /* pick up char from hardware */
        if (c == '\r') c = '\n'; /* convert eol to C standard */
        return c;
}

rawgetc7201(csrptr)
struct csr *csrptr;
{
  int c;

  while ((csrptr->cstat & SLURECVREADY) == 0) ;
  c = (csrptr->cbuf)&0xff;
  return c;
}

kcheckc7201(csrptr)
struct csr *csrptr;
{
   if (csrptr->cstat & SLURECVREADY)
    return (csrptr->cbuf&SLUCHMASK);
   else
    return -1;
}
