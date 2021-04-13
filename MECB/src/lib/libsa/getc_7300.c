#include <stdio.h>
#include "sludevs_7300.h"

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
    case CONSOLE:  c = kgetc7201(SLUCSR1); 
      break;
    case TTYB:  c = kgetc7201(SLUCSR2);
      break;
    case TTYC:  c = kgetc6850(SLUCSR3);
      break;
    default: return -1;
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
  int c;

  switch (dev) 
    {
    case CONSOLE:  return kcheckc7201(SLUCSR1); 
      break;
    case TTYB:  return kcheckc7201(SLUCSR2);
      break;
    case TTYC:  return kcheckc6850(SLUCSR3);
      break;
    default: return -1;
    }
}


rawgetc(dev)
int dev;			/* device # */
{
  switch (dev) 
    {
    case CONSOLE:  return rawgetc7201(SLUCSR1); 
      break;
    case TTYB:  return rawgetc7201(SLUCSR2);
      break;
    case TTYC:  return rawgetc6850(SLUCSR3);
      break;
    default:    return -1;
    }
}

