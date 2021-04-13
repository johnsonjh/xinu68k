/* sluint.c - sluint */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define SLUINT sluint6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define SLUINT sluint7201, etc.
   if SLUCHIP undef.,#include slu.h, #define SLUINT sluint  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>

/*------------------------------------------------------------------------
 *  sluint  --  interrupt dispatcher 
 *  Xinu assumes separate int vecs for receiver and transmitter,
 *  but MC6850/NEC7201 has only one common one, so need this.
 *------------------------------------------------------------------------
 */
INTPROC SLUINT(descrp)
	int descrp;
{
	register struct	tty	*ttyp;
	register struct	csr	*cptr;

#ifdef DEBUG
	dotrace ("sluint",NULL,0);
#endif
	ttyp = (struct tty *) devtab[descrp].dvioblk ;
	cptr = (struct csr *) ttyp->ioaddr ;
	if (slurecvready(cptr)) {
	  TTYIIN(ttyp, cptr, FALSE);
	  return(OK);
	}
	if (slutransready(cptr)) {
	  TTYOIN(ttyp, cptr);
	  return(OK);
	}
	/* should not get here */
	panic("SLU interrupt but device not ready");
}
