#include "slu6850.h"
/* Package to turn the write-only control register of each of up to MAXSLU
   MC6850 SLUs into effectively read-write registers, and also initialize
   the SLU for 8 bits, no parity, 1 stop bit, no interrupts.  To be
   effective, all references to the control registers need to go
   through this package. */

/* space for 3 saved regs-- */
#define MAXSLU 3

static struct {
  struct csr *csrptr;		/* which SLU this is */
  char csrval;			/* its current control reg val */
}soft_control_reg[MAXSLU] = {0};

slu_init6850(csrptr)
struct csr *csrptr;	/* ptr to device csr */
{
  int i;

  csrptr->cstat = SLURESET;	/* initialize the device */
  for (i=0;i<10;i++) ;		/* wait a bit */
  csrptr->cstat = SLUDEFAULT;

  /* lookup csr or set up new spot in array-- */
  for (i=0;i<MAXSLU;i++) {
    if (soft_control_reg[i].csrptr == (struct csr *)0) {
      soft_control_reg[i].csrptr = csrptr; /* new spot */
      break;
    } else if (soft_control_reg[i].csrptr == csrptr)
      break;
  }
  if (i<MAXSLU)
    soft_control_reg[i].csrval = SLUDEFAULT;
  else
    kprintf("Too many SLU devs for soft_control_reg table\n");
}

get_control_reg6850(csrptr)
struct csr *csrptr;	/* ptr to device csr */
{
  int i;

  for (i=0;i<MAXSLU;i++)
    if (soft_control_reg[i].csrptr == csrptr)
      return soft_control_reg[i].csrval;
  kprintf("Bad csrptr or uninitialized SLU\n");
}

/* must be called to change control reg, so that software var is maintained */
set_control_reg6850(csrptr,new_control)
struct csr *csrptr;
char new_control;
{
  int i;

  for (i=0;i<MAXSLU;i++) {
    if (soft_control_reg[i].csrptr == (struct csr *)0) {
      soft_control_reg[i].csrptr = csrptr; /* new spot */
      break;
    } else if (soft_control_reg[i].csrptr == csrptr)
      break;
  }
  if (i==MAXSLU)
    kprintf("Bad csrptr or uninitialized SLU\n");

  csrptr->cstat = new_control;	/* 6850--just one reg */
  return 0;
}

