/* Support routines to allow kprintf or other polling code */
/* to be used in the case that */
/* the calling program itself uses slu interrupts */
/* the calling program is required to call set_control_reg to manipulate */
/* the SLU control register, so that kprintf knows what to put it back to */

/* default to counter divide 16, no interrupts, 8 bits, no parity, 1 stop bit*/
/* should reinit this from startup */
#ifdef SLU7201
#include "slu7201.h"
static char soft_control_reg[2][6] = {{0,SLUCREG1DEF,SLUCREG2DEF,
					 SLUCREG3DEF,SLUCREG4DEF,SLUCREG5DEF},
					{0,SLUCREG1DEF,SLUCREG2DEF,
					SLUCREG3DEF,SLUCREG4DEF,SLUCREG5DEF}};
#else
#include "slu.h"
static char soft_control_reg[2] = {SLUDEFAULT,SLUDEFAULT};
#endif

/* call this to get current control reg contents-- */
#ifdef SLU7201
get_control_reg(csrptr,reg)
#else
get_control_reg(csrptr)
#endif
struct csr *csrptr;	/* ptr to device csr */
{
	int port01 = (short *)csrptr - (short *)SLUCSR1; /* port# as 0 or 1, not 1 or 2 */
	if (port01<0||port01>1)
	  {
	    kprintf("get_control_reg: Unknown control reg %x\n",(int)csrptr);
	    return -1;
	  }
	else
#ifdef SLU7201	  
	  return soft_control_reg[port01][reg];
#else
	  return soft_control_reg[port01];
#endif
}

/* must be called to change control reg, so that software var is maintained */
#ifdef SLU7201
set_control_reg(csrptr,new_control,reg)
#else
set_control_reg(csrptr,new_control)
#endif
struct csr *csrptr;
char new_control;
{
  int port01 = (short *)csrptr - (short *)SLUCSR1; /* port# as 0 or 1, not 1 or 2 */
  if (port01<0||port01>1) {
    kprintf("set_control_reg: Unknown control reg %x\n",(int)csrptr);
    return -1;
  }
#ifdef SLU7201
  if (reg==0) {
    if (new_control&SLUREGMASK) { /* better not set another reg! */
      kprintf("set_control_reg: Bad reg 0 command %x\n",
	      new_control);
      return -1;
    }
    else			/* OK, do it */
      csrptr->cstat = new_control;
  }
  else {			/* reg>0--select reg, do it, reselect reg 0 */
    csrptr->cstat = reg;	/* select reg */
    csrptr->cstat = new_control;
    csrptr->cstat = 0;		/*  back to reg 0 */
  }
  soft_control_reg[port01][reg] = new_control;
#else
  csrptr->cstat = new_control;	/* 6850--just one reg */
  soft_control_reg[port01] = new_control;
#endif  
  return 0;
}

