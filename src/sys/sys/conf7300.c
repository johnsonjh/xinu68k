/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include "../h/conf.h"

/* device independent I/O switch */

struct	devsw	devtab[NDEVS] = {

/*  CONSOLE */
0,
ttyinit7201, ioerr, ioerr,
ttyread, ttywrite7201, ioerr,
ttygetc, ttyputc7201, ttycntl7201,
0xe50000, 0x70, 0x70,
slu1int, slu1int, NULLPTR, 0,
/*  TTYB -- NYI, so init is null */
1,
ionull, ioerr, ioerr,
ttyread, ttywrite7201, ioerr,
ttygetc, ttyputc7201, ttycntl7201,
0xe50002, 0x00, 0x00,
slu2int, slu2int, NULLPTR, 1,
/*  TTYC--keyboard interface */
2,
/*ttyinit6850*/ionull, ioerr, ioerr,
ttyread, ttywrite6850, ioerr,
ttygetc, ttyputc6850, ttycntl6850,
0xe70000, 0x6c, 0x6c,
slu3int, slu3int, NULLPTR, 1,

/*  RING0IN--NYI */
3,
/* dlcinit*/ ionull, ioerr, ioerr,
dlcread, dlcwrite, ioerr,
ioerr, dlcputc, dlccntl,
0xe50002,0x00,0x00,
dlciin, dlcoin, NULLPTR, 0,

/*  RING0OUT */
4,
/* dlcinit */ ionull, ioerr, ioerr,
dlcread, dlcwrite, ioerr,
ioerr, dlcputc, dlccntl,
0xe70000, 0x6c, 0x6c,
dlciin, dlcoin, NULLPTR, 1,

/*  LPR */
/* Note that on the ATT7300, the parallel interface for the LP device has
   interrupts enabled all the time, so we need at least a near-trivial
   driver that just acknowledges PI interrupts that happen
*/

5,
lpinit, lpopen, lpclose,
ioerr, lpwrite, ioerr,
ioerr, lpputc, lpcntl,
0x470000, 0000, 0x68,
ionull, lpoin, NULLPTR, 0
};

/* slu1int saves regs, calls slu1intc, etc.-- */
slu1intc()
{
  sluint7201(CONSOLE);
}
slu2intc()
{
  sluint7201(TTYB);
}
slu3intc()
{
  sluint6850(TTYC);
}

