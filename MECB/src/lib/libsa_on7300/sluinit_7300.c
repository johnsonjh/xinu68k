/* SLU initialization by device number */
#include <stdio.h>
#include "sludevs_7300.h"

int console = 0;  /* dev being used as console */

slu_set_console(int condev)
{
  console = condev;
}

slu_init(dev)
int dev;
{
  switch (dev) {
   case CONSOLE: slu_init7201(SLUCSR1);
           break;
   case TTYB: slu_init7201(SLUCSR2);
           break;
   case TTYC: slu_init6850(SLUCSR3);
           break;
   default:  kprintf("slu_init: Unknown dev %d\n",dev);
  }
}

slu_test(dev)
int dev;
{
  int i,c;

  for (i=0;i<10;i++)
   putc(dev,'0'+i);
  for (i=0;i<10;i++) {
    c = getc(dev);
    if (c>=' ')
     putc(dev,c);
    else {
     putc(dev,'.');
     putc(dev,' '+c);
    }
   putc(dev,'0'+i);
  }
}

     
  
