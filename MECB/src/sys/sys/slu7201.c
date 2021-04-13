#include <slu7201.h>

/* Package to turn the write-only control registers of each of up to MAXSLU/2
   NEC7201 SLUs into effectively read-write registers, and also initialize
   the SLU for 8 bits, no parity, 1 stop bit, 9600 baud, no interrupts.
   To be effective, all references to the control registers need to go
   through this package. */

/* space for 2 sets of saved regs, for each channel of one 7201-- */
#define MAXSLU 2

static struct {
  struct csr *csrptr;		/* which SLU this is */
  char vals[6];			/* its 6 control reg vals */
}soft_control_reg[MAXSLU] = {0};

/* initialize UART regs to do async 8 bits, no parity, 1 stop bit, no ints */
/* save control reg settings in soft_control_reg for later access */
int slu_init7201(struct csr *csrptr)
{
  int i;

  /* lookup csr or set up new spot in array-- */
  for (i=0;i<MAXSLU;i++) {
    if (soft_control_reg[i].csrptr == (struct csr *)0) {
      soft_control_reg[i].csrptr = csrptr; /* new spot */
      break;
    } else if (soft_control_reg[i].csrptr == csrptr)
      break;
  }
  if (i==MAXSLU)
    panic("Too many SLU devs for soft_control_reg table");

  csrptr->cstat = 0;		/* select reg 0 */
  csrptr->cstat = 0;		/* for sure */
  csrptr->cstat = SLURESET;
  for (i=0;i<10;i++);		/* little delay */
  soft_control_reg[i].vals[0] = 0;
  csrptr->cstat = 2;
  csrptr->cstat = SLUCREG2DEF;
  soft_control_reg[i].vals[2] = SLUCREG2DEF;
  csrptr->cstat = 4;
  csrptr->cstat = SLUCREG4DEF;
  soft_control_reg[i].vals[4] = SLUCREG4DEF;
  csrptr->cstat = 1;
  csrptr->cstat = SLUCREG1DEF;
  soft_control_reg[i].vals[1] = SLUCREG1DEF;
  csrptr->cstat = 3;
  csrptr->cstat = SLUCREG3DEF;
  soft_control_reg[i].vals[3] = SLUCREG3DEF;
  csrptr->cstat = 5;
  csrptr->cstat = SLUCREG5DEF|SLUCREG5DTR;
  soft_control_reg[i].vals[5] = SLUCREG5DEF|SLUCREG5DTR;
  csrptr->cstat = 0;		/* leave with reg 0 selected */
  *SLUSELECT9600 = 0;		/* write to magic addr for 9600 baud */
  delay7201();			/* don't use right away */
  return 0;
}

set_baudrate(int baudrate)
{
  switch (baudrate) {
  case 1200: *SLUSELECT1200 = 0;
    break;
  case 2400: *SLUSELECT2400 = 0;
    break;
  case 4800: *SLUSELECT4800 = 0;
    break;
  case 9600: *SLUSELECT9600 = 0;
    break;
  case 19200: *SLUSELECT19200 = 0;
    break;
  case 38400: *SLUSELECT38400 = 0;
    break;
  default:  return -1;
  }
  return 0;
}

delay7201()
{
  int i;

  for (i=0;i<10000;i++)
	;
}

/* call this to get current control reg contents-- */
get_control_reg7201(csrptr,reg)
struct csr *csrptr;	/* ptr to device csr */
{
  int i;

  for (i=0;i<MAXSLU;i++)
    if (soft_control_reg[i].csrptr == csrptr)
      return soft_control_reg[i].vals[reg];
  panic("Bad csrptr or uninitialized SLU");
}

/* must be called to change control reg, so that software var is maintained */
set_control_reg7201(csrptr,new_control,reg)
struct csr *csrptr;
char new_control;
{
  int i;

  for (i=0;i<MAXSLU;i++)
    if (soft_control_reg[i].csrptr == csrptr)
      break;
  if (i==MAXSLU)
    panic("Bad csrptr or uninitialized SLU");

  if (reg==0) {
    if (new_control&SLUREGMASK) /* better not set another reg! */
      panic("Inconsistent use of set_control_reg7201");
    else			/* OK, do it */
      csrptr->cstat = new_control;
  }
  else {			/* reg>0--select reg, do it, reselect reg 0 */
    csrptr->cstat = reg;	/* select reg */
    csrptr->cstat = new_control;
    csrptr->cstat = 0;		/*  back to reg 0 */
  }
  delay7201();			/* let 7201 have time to process cmd */
  soft_control_reg[i].vals[reg] = new_control;
  return 0;
}
/* used by slutenable macro--reenable transmit ints if necessary,
   start output with null char */
slu_tstart7201(csrptr)
struct csr *csrptr;
{
  short savesr,creg1;
  int i;

  for (i=0;i<MAXSLU;i++)
    if (soft_control_reg[i].csrptr == csrptr)
      break;
  if (i==MAXSLU)
    panic("Bad csrptr or uninitialized SLU");

  _disable(&savesr);		/* make sure test,set of int bit is atomic  */
  if ((creg1 = soft_control_reg[i].vals[1])&SLUTRANSIE) {
    _restore(&savesr);
    return;			/* already going */
  }
  while ((csrptr->cstat&SLUTRANSREADY)==0) ; /* make sure transmitter idle */
  set_control_reg7201(csrptr,creg1|SLUTRANSIE,1); /* set int. bit */
  csrptr->cbuf = 0;		/* start output */
  _restore(&savesr);
}


  
