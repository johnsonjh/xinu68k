
#include <stdio.h>
#include "cpu.h"
extern int registers[];
#define NBP 12
#define BPTRAP 0x4e41
static struct bpt {
  int addr;
  short old_instruction;
} bp[NBP];
static int bp_needs_redoing = -1;  /* >=0--one that needs it */
static int was_tracing = 0; /* valid only if bp_needs_redoing >= 0 */

int set_trace()
{
  registers[PS] |= PS_TBIT;
}

int init_bps()
{
  int i;

  for (i=0;i<NBP;i++)
    bp[i].addr = 0;
}

int set_bp(int addr)
{
  int i;

  for (i=0;i<NBP;i++) 
    if (bp[i].addr==0) break;
  if (i==NBP) {
    printf("Breakpoint table is full!\n");
    return -1;
  }
  bp[i].addr = addr;
  bp[i].old_instruction = *(short *)addr;
  *(short *)addr = BPTRAP;
  return 0;
}

int clear_bp(int addr)
{
  int i;

  if (addr==-1) {
    for (i=0;i<NBP;i++)
      if (bp[i].addr>0)
        clear_bp(bp[i].addr); /* clear all bkpts */
    return 0;
  }

  for (i=0;i<NBP;i++)
    if (bp[i].addr==addr) break;
  if (i==NBP) {
    printf("No breakpoint at %x\n",addr);
    return -1;
  }
  bp[i].addr = 0;
  *(short *)addr = bp[i].old_instruction;
  return 0;
}
void show_bp()
{
  int i;

  for (i=0;i<NBP;i++)
   if (bp[i].addr)
     printf("%x ",bp[i].addr);
  printf("\n");
}

void resume(int addr)
{
  int i,new_PC;

  if (addr>0)
    new_PC = addr;
  else
    new_PC = registers[PC];
  for (i=0;i<NBP;i++)
    if (bp[i].addr == new_PC) /* found bkpt that has this trap */
      break;
  if (i==NBP) {  /* no bkpt has this trap */
    registers[PC] = new_PC;  /* assume OK */
    bp_needs_redoing = -1;
    return;   
  } 
  *(short *)new_PC = bp[i].old_instruction;  /* replace instruction */
  registers[PC] = new_PC;  /* do it */
  bp_needs_redoing = i;
  was_tracing = 0;          /* assume go */
  if (registers[PS]&PS_TBIT)
    was_tracing = 1;       /* but was trace cmd */
  else
    registers[PS] |= PS_TBIT;  /* regain control and fix up */
}

/* called from entry to tutor code--chance to fix bp, unset trace bit */
int adjust_bps_on_next_entry()
{
  int i,addr;

  registers[PS] &= ~PS_TBIT;   /* turn off trace bit in SR */

  for (i=0;i<NBP;i++)
    if (registers[PC]==bp[i].addr + 2) {   /* just trapped on this bp */
        registers[PC] -= 2;         /* PC for code execution progress */
        break;                  /* i.e, what user, rest of debugger sees */
      }
  if ((i=bp_needs_redoing)>=0) {
    addr = bp[i].addr;
    *(short *)addr = BPTRAP;
   bp_needs_redoing = -1;
   if (was_tracing)
     return 0;     /* trace cmd--leave in tutor */
   else
     return 1;     /* signal to keep running */
   }
   return 0;     /* normal case--do tutor cmds now */
}
