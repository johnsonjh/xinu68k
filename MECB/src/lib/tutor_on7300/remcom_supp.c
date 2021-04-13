/* put jsr's to handlers at 0x400 instead of very top of mem-- */
#define HANDLER_XFER ((char *)0x400)
#define JSR ((short)0x4eb9)
putDebugChar(ch)
char ch;
{
  rawputc(1,ch);
  return 1;
}

int getDebugChar()
{
  return rawgetc(1);
}
typedef void (*ExceptionHook)(int);   /* pointer to function with int parm */
typedef void (*Function)();           /* pointer to a function */

ExceptionHook exceptionHook;
exceptionHandler(chan,handler)
int chan;
Function handler;
{
  int jsr_addr;

#ifdef m68010
  *((int *) (chan<<2)) = (int)handler; /* direct xfer */
#else				/* need table kludge to get except. id */
  jsr_addr = (int)HANDLER_XFER+chan*6;
  *((int *) (chan<<2)) = jsr_addr;
  *(short *)jsr_addr = JSR;	/* jsr, abs long addressing */
  *(Function *)(jsr_addr+2) = handler;
#endif
}
