/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include "../h/conf.h"

/* device independent I/O switch */

struct	devsw	devtab[NDEVS] = {

/*  CONSOLE */
0,
ttyinit6850, ioerr, ioerr,
ttyread, ttywrite6850, ioerr,
ttygetc, ttyputc6850, ttycntl6850,
0x10040, 0x74, 0x74,
slu1int, slu1int, NULLPTR, 0,

/*  TTYA */
1,
ttyinit6850, ioerr, ioerr,
ttyread, ttywrite6850, ioerr,
ttygetc, ttyputc6850, ttycntl6850,
0x10041, 0x78, 0x78,
slu2int, slu2int, NULLPTR, 1,
/*  TTYB--6850 can be added in i/o page 3
    (see p 7-15 and 7-16 of MECB User's Man) */
2,  
/* ttyinit6850, but NYI */ ionull, ioerr, ioerr,
ttyread, ttywrite6850, ioerr,
ttygetc, ttyputc6850, ttycntl6850,
0x30000, 0x70, 0x70,
slu3int, slu3int, NULLPTR, 2,

/*  RING0IN -- hostline 6850 */
3,
dlcinit, ioerr, ioerr,
dlcread, dlcwrite, ioerr,
ioerr, dlcputc, dlccntl,
0x10041, 0x78, 0x78,
dlciin, dlcoin, NULLPTR, 0,

/*  RING0OUT --6850 added to p. 3 */
4,
dlcinit, ioerr, ioerr,
dlcread, dlcwrite, ioerr,
ioerr, dlcputc, dlccntl,
0x30000, 0x70, 0x70,
dlciin, dlcoin, NULLPTR, 1,

/*  LPR */
5,
lpinit, lpopen, lpclose,
ioerr, lpwrite, ioerr,
ioerr, lpputc, lpcntl,
0x10021, 0, 0x110,
ionull, lpoin, NULLPTR, 0
};

/* slu1int calls slu1intc, etc.-- */
slu1intc()
{
  sluint6850(CONSOLE);
}
slu2intc()
{
  sluint6850(TTYB);
}
slu3intc()
{
  sluint6850(TTYC);
}

