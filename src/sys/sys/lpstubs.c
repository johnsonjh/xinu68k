#ifdef ATT7300
#include <intvectors_7300.h>
#include <pi7300.h>
#endif
/* Since ATT7300 parallel interface has interrupts enabled all the time,
   we had better arm them or face system crash on Centronics-pin-10 signal */
lpinit()
{
#ifdef ATT7300
  extern int piint();

  PIVEC = piint;		/* save C scratch regs, then jsr's to lpoin */
#endif
}
lpopen()
{
}
lpclose()
{
}
lpwrite()
{
}
lpputc()
{
}
lpcntl()
{
}
lpoin()
{
#ifdef ATT7300
  *PI_DATA = 0;			/* ack the PI by writing to data reg */
#endif
}
