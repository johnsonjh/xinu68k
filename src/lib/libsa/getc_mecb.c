
#include "stdio.h"
#include "sludevs_mecb.h"

extern int console;   /* dev no. of console */
/*-----------------------------------------------------------------------
 *  getc  --  read a character from the console (or other SLU)
 *  using polled I/O
 *------------------------------------------------------------------------
 */
getc(dev)
int dev;			/* device # */
{
  int c;

  switch (dev) 
    {
    case CONSOLE:  c = kgetc6850(SLUCSR1); 
      break;
    case TTYB:  c = kgetc6850(SLUCSR2);
      break;
    default:   return -1;
    }
  if (c == EOFCHAR)
    return EOF;
  else
    {
      if (dev==console) putc(dev,c); /* echo */
      return c;
    }
}


checkc(dev)
int dev;			/* device # */
{
  switch (dev) 
    {
    case CONSOLE:  return kcheckc6850(SLUCSR1); 
      break;
    case TTYB:  return kcheckc6850(SLUCSR2);
      break;
    default:  return -1;
    }
}


rawgetc(dev)
int dev;			/* device # */
{
  switch (dev) 
    {
    case CONSOLE:  return rawgetc6850(SLUCSR1); 
      break;
    case TTYB:  return rawgetc6850(SLUCSR2);
      break;
    default:  return -1;
    }
}

