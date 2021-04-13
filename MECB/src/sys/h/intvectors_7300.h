/* intvectors.h: header file for interrupt programming on att7300 */

typedef int (*intvec) ();	/* an intvec is a ptr to a fn */

/* each vector is at address = 4*(intvec slot #) */
/* for ex. the console is at vec slot 29, at address 4*29=116=0x74 */
/* All interrupts on the ATT7300 are "autovectored", so they have */
/* fixed vector locations as follows--                            */

#define CLOCKVEC (*(intvec *)0x78)  /* clock interrupt */
#define CLOCKLEV 6
#define SLUVEC1 (*(intvec *)0x70)  /* the SLU1 int vec at addr 0x70 */
#define SLU1LEV 4		/* interrupt level for SLU1 */
#define SLUVEC3 (*(intvec *)0x6c) /* SLU3 int vec: keyboard */
#define SLU3LEV 3		/* int lev for SLU3 */
#define PIVEC (*(intvec *)0x68)  /* the parallel port, also floppy */
#define PILEV 2

/*  Assembly language envelope routines are available in the att7300
library for the interrupt handlers needed for each of the vectors
defined above.   For example, for the console, SLU1, device, there
is:
.globl _slu1int			| make _slu1int extern
 
 _slu1int: movel a0,sp@-		| save C scratch regs
	movel a1,sp@-
	movel d0,sp@-
	movel d1,sp@-		| (or use movem for better performance)
	jsr _slu1intc		| C will save other regs on entry to routine
	movel sp@+,d1
	movel sp@+,d0
	movel sp@+,a1
	movel sp@+,a0
	rte			| return from interrupt

Similar routines are available for:
slu2int, calling slu2intc
clockint, calling clockintc
spurint, calling spurintc
*/
