
/* getprio.c    */

#include <conf.h> 
#include <kernel.h>
#include <proc.h>

SYSCALL getprio(pid)
int pid;
{
  struct pentry *pptr;
  
  disable();
  if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE)
  {
   restore();
   return(SYSERR);
  }
  restore();
  return(pptr->pprio);
}

