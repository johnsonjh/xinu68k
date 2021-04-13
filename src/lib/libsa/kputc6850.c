#include "slu6850.h"

#define NEWLINE '\n'
#define RETURN '\r'

/*------------------------------------------------------------------------
 *  kputc  --  write a character on the console (or other SLU)
 *  using polled I/O
 *------------------------------------------------------------------------
 */
kputc6850(csrptr,c)
struct csr *csrptr;	/* address of device csr (0x10040 for console)*/
register char c;	/* character to print from _doprnt	*/
{
/*T*/	int	slowdown;	/* added to delay output because VAX	*/
/*T*/				/* can't take it at 9600 baud		*/
	if ( c == 0 )
		return;
	if ( c == NEWLINE )
		kputc6850(csrptr, RETURN );
	while ( (csrptr->cstat & SLUTRANSREADY) == 0 ) ;  /* poll for idle*/
	csrptr->cbuf = c;
/*T*/	for(slowdown=0;slowdown<300;slowdown++) ;
	while ( (csrptr->cstat & SLUTRANSREADY) == 0 ) ;  /* poll for idle*/
}
rawputc6850(csrptr,c)
struct csr *csrptr;
char c;
{
	while ((csrptr->cstat & SLUTRANSREADY) == 0) ;
        csrptr->cbuf = c;
}

