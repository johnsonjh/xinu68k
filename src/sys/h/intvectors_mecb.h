/* intvectors_mecb.h: header file for interrupt programming on mecb */

typedef int (*intvec) ();	/* an intvec is a ptr to a fn */

/* each vector is at address = 4*(intvec slot #) */
/* for ex. the console is at vec slot 29, at address 4*29=116=0x74 */

#define SPURVEC (*(intvec *)0x60) /* spurious interrupt */
#define SLUVEC1 (*(intvec *)0x74)  /* the SLU1 int vec at addr 0x74: console */
#define SLU1LEV 5		/* interrupt level for SLU1 */
#define SLUVEC2 (*(intvec *)0x78) /* SLU2 int vec: "host line" */
#define SLU2LEV 6		/* int lev for SLU2 */
#define ABORTVEC (*(intvec *)0x7c) /* int vec for abort interrupt */
#define ABORTLEV 7		/* nonmaskable, even at CPU pri 7 */

/* the location of the PI/T timer int vector, in user vector area, 100-3fc-- */
#define CLOCKVECADDRESS ((intvec *)0x100)   /* we choose this, in 100-3fc */

/* the int vector itself--at location we chose-- */
#define CLOCKVEC (*(intvec *)CLOCKVECADDRESS)
#define CLOCKLEV 2

/* The parallel port uses level 3--in block of four vectors,             */
/*   with base address binary XXXXXX0000, XXXXXX from pivr.              */
/*   Then during interrupt cycle, 68230 sets bits 2 and 3 based          */
/*   on specific PI event:  00 for H1 event, 01 for H2, 10 for H3, 11 for H4*/
/*   so we end up with 4 interrupt vectors, one for each handshake line  */
/*   but the MECB has output buffering on H2 and H4, so only H1 and H3   */
/*   are really usable for interrupts                                    */
#define PI_BASEVECADDRESS ((intvec *)0x110)   /* we choose this, in 100-3fc */
#define PI_H1VECADDRESS PI_BASEVECADDRESS   /* H1 is 0'th of the four */
#define PI_H3VECADDRESS (PI_BASEVECADDRESS+2) /* H3 is 2nd of the four */

/* the int vectors themselves--at locations specified above-- */
#define PI_H1VEC (*(intvec *)PI_H1VECADDRESS)
#define PI_H3VEC (*(intvec *)PI_H3VECADDRESS)
#define PILEV 3


/*  Assembly language envelope routines are available in the mecb
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
