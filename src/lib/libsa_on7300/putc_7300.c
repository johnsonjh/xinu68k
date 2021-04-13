#include <stdio.h>
#include "sludevs_7300.h"
/*------------------------------------------------------------------------
 *  putc  --  write a character on the console (or other SLU)
 *  using polled I/O--for standalone programs
 *------------------------------------------------------------------------
 */
putc(dev,c)
int dev;			/* device number */
register char c;		/* character to print   */
{
  switch (dev)
    {
    case CONSOLE:   kputc7201(SLUCSR1,c);
      break;
    case TTYB:  kputc7201(SLUCSR2,c);
      break;
    case TTYC:  kputc6850(SLUCSR3,c);
      break;
    default: return -1;
    }
  return 0;
}

rawputc(dev,c)
int dev;			/* device number */
register char c;		/* character to print   */
{
  switch (dev)
    {
    case CONSOLE:   rawputc7201(SLUCSR1,c);
      break;
    case TTYB:  rawputc7201(SLUCSR2,c);
      break;
    case TTYC:  rawputc6850(SLUCSR3,c);
      break;
    default: return -1;
    }
  return 0;
}

