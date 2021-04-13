#include "stdio.h"
#include "sludevs_mecb.h"
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
    case CONSOLE:   kputc6850(SLUCSR1,c);
      break;
    case TTYB:  kputc6850(SLUCSR2,c);
      break;
    default:  return -1;
    }
  return 0;
}

rawputc(dev,c)
int dev;			/* device number */
register char c;		/* character to print   */
{
  switch (dev)
    {
    case CONSOLE:   rawputc6850(SLUCSR1,c);
      break;
    case TTYB:  rawputc6850(SLUCSR2,c);
      break;
    default: return -1;
    }
  return 0;
}

