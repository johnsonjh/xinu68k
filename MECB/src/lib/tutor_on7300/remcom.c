
/****************************************************************************

		THIS SOFTWARE IS NOT COPYRIGHTED  
   
   HP offers the following for use in the public domain.  HP makes no
   warranty with regard to the software or it's performance and the 
   user accepts the software "AS IS" with all faults.

   HP DISCLAIMS ANY WARRANTIES, EXPRESS OR IMPLIED, WITH REGARD
   TO THIS SOFTWARE INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

****************************************************************************/

/****************************************************************************
 *  $Header: remcom.c,v 1.25 89/05/16 14:34:00 glenne Exp $                   
 *
 *  $Module name: remcom.c $  
 *  $Revision: 1.25 $
 *  $Date: 89/05/16 14:34:00 $
 *  $Contributor:     Lake Stevens Instrument Division$
 *  
 *  $Description:     low level support for gdb debugger. $
 *
 *  $Considerations:  only works on target hardware $
 *
 *  $Written by:      Glenn Engel $
 *  $ModuleState:     Experimental $ 
 *
 *  $NOTES:           See Below $
 * 
 *  To enable debugger support, two things need to happen.  One, a
 *  call to set_debug_traps() is necessary in order to allow any breakpoints
 *  or error conditions to be properly intercepted and reported to gdb.
 *  Two, a breakpoint needs to be generated to begin communication.  This
 *  is most easily accomplished by a call to breakpoint().  Breakpoint()
 *  simulates a breakpoint by executing a trap #1.
 *  
 *  Some explanation is probably necessary to explain how exceptions are
 *  handled.  When an exception is encountered the 68000 pushes the current
 *  program counter and status register onto the supervisor stack and then
 *  transfers execution to a location specified in it's vector table.
 *  The handlers for the exception vectors are hardwired to jmp to an address
 *  given by the relation:  (exception - 256) * 6.  These are decending 
 *  addresses starting from -6, -12, -18, ...  By allowing 6 bytes for
 *  each entry, a jsr, jmp, bsr, ... can be used to enter the exception 
 *  handler.  Using a jsr to handle an exception has an added benefit of
 *  allowing a single handler to service several exceptions and use the
 *  return address as the key differentiation.  The vector number can be
 *  computed from the return address by [ exception = (addr + 1530) / 6 ].
 *  The sole purpose of the routine _catchException is to compute the
 *  exception number and push it on the stack in place of the return address.
 *  The external function exceptionHandler() is
 *  used to attach a specific handler to a specific 68k exception.
 *  For 68010/68020 machines, the ability to have a return address around just
 *  so the vector can be determined is not necessary because the '020 pushes an
 *  extra word onto the stack containing the vector offset
 * 
 *  Because gdb will sometimes write to the stack area to execute function
 *  calls, this program cannot rely on using the supervisor stack so it
 *  uses it's own stack area reserved in the int array remcomStack.  
 * 
 *************
 *
 *    The following gdb commands are supported:
 * 
 * command          function                               Return value
 * 
 *    g             return the value of the CPU registers  hex data or ENN
 *    G             set the value of the CPU registers     OK or ENN
 * 
 *    mAA..AA,LLLL  Read LLLL bytes at address AA..AA      hex data or ENN
 *    MAA..AA,LLLL: Write LLLL bytes at address AA.AA      OK or ENN
 * 
 *    c             Resume at current address              SNN   ( signal NN)
 *    cAA..AA       Continue at address AA..AA             SNN
 * 
 *    s             Step one instruction                   SNN
 *    sAA..AA       Step one instruction from AA..AA       SNN
 * 
 *    k             kill
 *
 *    ?             What was the last sigval ?             SNN   (signal NN)
 * 
 * All commands and responses are sent with a packet which includes a 
 * checksum.  A packet consists of 
 * 
 * $<packet info>#<checksum>.
 * 
 * where
 * <packet info> :: <characters representing the command or response>
 * <checksum>    :: < two hex digits computed as modulo 256 sum of <packetinfo>>
 * 
 * When a packet is received, it is first acknowledged with either '+' or '-'.
 * '+' indicates a successful transfer.  '-' indicates a failed transfer.
 * 
 * Example:
 * 
 * Host:                  Reply:
 * $m0,10#2a               +$00010203040506070809101112131415#42
 * 
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "cpu.h"
/************************************************************************
 *
 * external low-level support routines 
 */
typedef void (*ExceptionHook)(int);   /* pointer to function with int parm */
typedef void (*Function)();           /* pointer to a function */

extern putDebugChar();   /* write a single character      */
extern getDebugChar();   /* read and return a single char */

extern Function exceptionHandler();  /* assign an exception handler */
extern ExceptionHook exceptionHook;  /* hook variable for errors/exceptions */


/************************************************************************/
/* BUFMAX defines the maximum number of characters in inbound/outbound buffers*/
/* at least NUMREGBYTES*2 are needed for register packets */
#define BUFMAX 400

static char initialized;  /* boolean flag. != 0 means we've been initialized */
static char tutor_flag;

int     remote_debug = 0;
/*  debug >  0 prints ill-formed commands in valid packets & checksum errors */ 

char hexchars[]="0123456789abcdef";

/* var length frame record--extra long for addr/bus errors */
typedef struct FrameStruct
{
    struct FrameStruct  *previous;
    int       exceptionPC;      /* pc value when this frame created */
    int       exceptionVector;  /* cpu vector causing exception     */
    short     frameSize;        /* size of cpu frame in words       */
    short     sr;               /* for 68000, this not always sr    */
    int       pc;
    short     format;
    int       morejunk[0];        /* exception frame, fp save... */
} Frame;

#define FRAMESIZE 500
static Frame *lastFrame;
static int   frameStack[FRAMESIZE];

/*
 * these should not be static cuz they can be used outside this module
 */
int registers[NUMREGBYTES/4];
int superStack;

static int remcomStack[2000];
static int* stackPtr = &remcomStack[1999];

/*
 * In many cases, the system will want to continue exception processing
 * when a continue command is given.  
 * oldExceptionHook is a function to invoke in this case.
 */

static ExceptionHook oldExceptionHook;

/* the size of the exception stack on the 68020 varies with the type of
 * exception.  The following table is the number of WORDS used
 * for each exception format.
 */
/* static short exceptionSize[] = { 4,4,6,4,4,4,4,4,29,10,16,46,4,4,4,4 }; */
/* However, the 68010 has only 2 formats, 4 words with format 0000 binary */
/*  and 29 words with format 1000 binary */

/************* jump buffer used for setjmp/longjmp **************************/
jmp_buf env;

/***************************  ASSEMBLY CODE MACROS *************************/
/* 									   */
#ifdef m68010
asm("
text
global return_to_super
return_to_super:
        mov.l   registers+60,%sp #/* get new stack pointer */        
        mov.l   lastFrame,%a0   #/* get last frame info  */              
        bra     return_to_any

global return_to_user
return_to_user:
        mov.l   registers+60,%a0 # /* get usp */                          
        mov.l   %a0,%usp        #   /* set usp */				
        mov.l   superStack,%sp  #/* get original stack pointer */        

return_to_any:
        mov.l   lastFrame,%a0   #/* get last frame info  */              
        mov.l   (%a0)+,lastFrame# /* link in previous frame     */        
        addq.l   &8,%a0         #  /* skip over pc, vector#*/              
        mov.w   (%a0)+,%d0      #   /* get # of words in cpu frame */       
        add.w    %d0,%a0        #   /* point to end of data        */       
        add.w    %d0,%a0        #   /* point to end of data        */       
        mov.l   %a0,%a1                                                   
#                                                                       
# copy the stack frame                                                  
        subq.l   &1,%d0                                                   
copyUserLoop:                                                               
        mov.w   -(%a1),-(%sp)                                               
        dbf     %d0,copyUserLoop                                             
");                                                                     
  asm("   movm.l  registers,&0x7fff");			        
   asm("   rte");              /* pop and go! */                                    

#else   /* 68000--mecb */
asm("
.text
.globl _return_to_super
_return_to_super:
        movel   _registers+60,sp /* get new stack pointer */        
        movel   _lastFrame,a0   /* get last frame info  */              
        bra     return_to_any

.globl _return_to_user
_return_to_user:
        movel   _registers+60,a0 /* get usp */                          
        movel   a0,usp           /* set usp */				
        movel   _superStack,sp  /* get original stack pointer */        

return_to_any:
        movel   _lastFrame,a0   /* get last frame info  */              
        movel   a0@+,_lastFrame /* link in previous frame     */        
        addql   #8,a0           /* skip over pc, vector#*/              
        movew   a0@+,d0         /* get # of words in cpu frame */       
        addw    d0,a0           /* point to end of data        */       
        addw    d0,a0           /* point to end of data        */       
        movel   a0,a1                                                   
#                                                                       
# copy the stack frame                                                  
        subql   #1,d0                                                   
copyUserLoop:                                                               
        movew   a1@-,sp@-                                               
        dbf     d0,copyUserLoop                                             
   asm("   moveml  _registers,d0-d7/a0-a6"");                                                                     
);			        
   asm("   rte");  /* pop and go! */                                    
#endif
#ifdef m68010
#define DISABLE_INTERRUPTS()   asm("         ori.w   &0x0700,%sr");
#define BREAKPOINT() asm("   trap &1");
#else
#define DISABLE_INTERRUPTS()   asm("         oriw   #0x0700,sr");
#define BREAKPOINT() asm("   trap #1");
#endif
/* this function is called immediately when a level 7 interrupt occurs */
/* if the previous interrupt level was 7 then we're already servicing  */
/* this interrupt and an rte is in order to return to the debugger.    */
/* For the 68000, the offset for sr is 6 due to the jsr return address */
#ifdef m68010
asm("
text
global _debug_level7
_debug_level7:
	mov.w   %d0,-(%sp)");
asm("	mov.w   2(%sp),%d0");
asm("	and.w   &0x700,%d0
	cmp.w   %d0,&0x700
	beq     already7
        mov.w   (%sp)+,%d0	
        bra     _catchException
already7:
	mov.w   (%sp)+,%d0");
asm("	rte");

#else /* 68000-MECB */

asm("
.text
.globl __debug_level7
__debug_level7:
	movew   d0,sp@-");
asm("	movew   sp@(6),d0");
asm("	andiw   #0x700,d0
	cmpiw   #0x700,d0
	beq     _already7
        movew   sp@+,d0	
        bra     __catchException
_already7:
	movew   sp@+,d0");
asm("	lea     sp@(4),sp");     /* pull off 68000 return address */
asm("	rte");
#endif

extern void _catchException();

/*
 * remcomHandler is a front end for handle_exception.  It moves the
 * stack pointer into an area reserved for debugger use in case the
 * breakpoint happened in supervisor mode.
 */
#ifdef m68010
asm("remcomHandler:");
asm("           add.l    &4,%sp");        /* pop off return address     */
asm("           mov.l   (%sp)+,%d0");      /* get the exception number   */
asm("		mov.l   stackPtr,%sp"); /* move to remcom stack area  */
asm("		mov.l   %d0,-(%sp)");	/* push exception onto stack  */
asm("		jsr    handle_exception");    /* this never returns */
asm("           rts");                  /* return */
#else /* mc68000 */
asm("_remcomHandler:");
asm("           addl    #4,sp");        /* pop off return address     */
asm("           movel   sp@+,d0");      /* get the exception number   */
asm("		movel   _stackPtr,sp"); /* move to remcom stack area  */
asm("		movel   d0,sp@-");	/* push exception onto stack  */
asm("		jbsr    _handle_exception");    /* this never returns */
asm("           rts");                  /* return */
#endif

/* This function is called when a 68010 exception occurs.  It saves
 * all the cpu regs in the _registers array, creates a frame on a
 * linked list of frames which has the cpu stack frames needed
 * to properly restore the context of the processor, and invokes
 * an exception handler (remcom_handler).
 *
 * stack on entry:                       stack on exit:
 *   N bytes of junk                     exception # MSWord
 *   Exception Format Word               exception # MSWord
 *   Program counter LSWord              
 *   Program counter MSWord             
 *   Status Register                    
 *   More stuff if bus or addr exception                                    
 *                                       
 */
#ifdef m68010
asm(" 

text
global _catchException
_catchException:");
DISABLE_INTERRUPTS();
asm("
       movm.l  &0x7fff,registers  #/* save registers        */
	mov.l	lastFrame,%a0 #	/* last frame pointer */
");
asm("
	lea     registers,%a5  # /* get address of registers     */
        mov.w   (%sp),%d1       #   /* get status register          */
        mov.w   %d1,66(%a5)    #  /* save sr		 	*/	
	mov.l   2(%sp),%a4    #   /* save pc in a4 for later use  */
        mov.l   %a4,68(%a5)    #  /* save pc in regisers[]      	*/

#
# figure out how many bytes in the stack frame
	mov.w   6(%sp),%d0    #	/* get '010/'020 exception format	*/
        mov.w   %d0,%d2      #     /* make a copy of format word   */
        and.w   &0xf000,%d0 #     /* mask off format type         */
        mov.l &4,%d3	#	/* assume smaller type */
        cmp.w  %d0,&0x8000 #	/* larger type? */
        bne smalltyp	#	/* no, len=4, already in d3 */
        mov.l &29,%d3	#	/* yes, len=29 */
smalltyp:
        mov.l   %d3,%d0	#	/* put len in d0 */
        sub.w    %d0,%a0#		/* adjust save pointer          */
        sub.w    %d0,%a0#		/* adjust save pointer(bytes)   */
	mov.l   %a0,%a1 #          /* copy save pointer            */
	subq.l   &1,%d0 #          /* predecrement loop counter    */
#
# copy the frame
saveFrameLoop:
	mov.w  	(%sp)+,(%a1)+
	dbf     %d0,saveFrameLoop
#
# now that the stack has been clenaed,
# save the a7 in use at time of exception
        mov.l   %sp,superStack # /* save supervisor sp           */
        andi.w   &0x2000,%d1   #   /* were we in supervisor mode ? */
        beq     userMode       
        mov.l   %a7,60(%a5)    #  /* save a7                  */
        bra     a7saveDone
userMode:  
	mov.l   %usp,%a1    	
        mov.l   %a1,60(%a5)    #  /* save user stack pointer	*/
a7saveDone:

#
# save size of frame
        mov.w   %d3,-(%a0)

#
# compute exception number
	and.l    &0xfff,%d2   #	/* mask off vector offset	*/
	lsr.l    &2,%d2   #	/* divide by 4 to get vect num	*/
        mov.l   %d2,-(%a0) #        /* save it                      */
#
# save pc causing exception
        mov.l   %a4,-(%a0)
#
# save old frame link and set the new value
	mov.l	lastFrame,%a1#	/* last frame pointer */
	mov.l   %a1,-(%a0)#		/* save pointer to prev frame	*/
        mov.l   %a0,lastFrame

        mov.l   %d2,-(%sp)#	/* push exception num           */
	mov.l   exceptionHook,%a0 #/* get address of handler */
        jsr    (%a0)	#	/* and call it */
        jmp     _returnFromException #    /* now, return        */
");
#else /* mc68000 */
/* This function is called when an exception occurs.  It translates the
 * return address found on the stack into an exception vector # which
 * is then handled by either handle_exception or a system handler.
 * _catchException provides a front end for both.  
 *
 * stack on entry:                       stack on exit:
 *   Program counter MSWord              exception # MSWord 
 *   Program counter LSWord              exception # MSWord
 *   Status Register                     
 *   Return Address  MSWord              
 *   Return Address  LSWord             
 */
asm("
.text
.globl __catchException
__catchException:");
DISABLE_INTERRUPTS();
asm("
        moveml d0-d7/a0-a6,_registers  /* save registers               */
	movel	_lastFrame,a0	/* last frame pointer */
");
asm("
        lea     _registers,a5   /* get address of registers     */
        movel   sp@+,d2         /* pop return address           */
	addl 	#1530,d2        /* convert return addr to 	*/
	divs 	#6,d2   	/*  exception number		*/
	extl    d2   

        moveql  #3,d3           /* assume a three word frame     */

        cmpiw   #3,d2           /* bus error or address error ? */
        bgt     normal          /* if >3 then normal error      */
        movel   sp@+,a0@-       /* copy error info to frame buff*/
        movel   sp@+,a0@-       /* these are never used         */
        moveql  #7,d3           /* this is a 7 word frame       */
     
normal:   
	movew   sp@+,d1         /* pop status register          */
        movel   sp@+,a4         /* pop program counter          */
        movew   d1,a5@(66)      /* save sr		 	*/	
        movel   a4,a5@(68)      /* save pc in _regisers[]      	*/
        movel   a4,a0@-         /* copy pc to frame buffer      */
	movew   d1,a0@-         /* copy sr to frame buffer      */

        movel   sp,_superStack  /* save supervisor sp          */

        andiw   #0x2000,d1      /* were we in supervisor mode ? */
        beq     userMode       
        movel   a7,a5@(60)      /* save a7                  */
        bra     saveDone             
userMode:
        movel   usp,a1    	/* save user stack pointer 	*/
        movel   a1,a5@(60)      /* save user stack pointer	*/
saveDone:

        movew   d3,a0@-         /* push frame size in words     */
        movel   d2,a0@-         /* push vector number           */
        movel   a4,a0@-         /* push exception pc            */

#
# save old frame link and set the new value
	movel	_lastFrame,a1	/* last frame pointer */
	movel   a1,a0@-		/* save pointer to prev frame	*/
        movel   a0,_lastFrame

        movel   d2,sp@-		/* push exception num           */
	movel   _exceptionHook,a0  /* get address of handler */
        jbsr    a0@             /* and call it */
        jmp     __returnFromException     /* now, return        */
");
#endif


void _returnFromException( Frame *frame )
{
    /* if no existing frame, dummy one up */
    if (! frame)
    {
        frame = lastFrame -1;
        frame->frameSize = 4;
        frame->format = 0;
        frame->previous = lastFrame;
    }

#ifndef m68010
    /* a 68000 cannot use the internal info pushed onto a bus error
     * or address error frame when doing an RTE so don't put this info
     * onto the stack or the stack will creep every time this happens.
     */
    frame->frameSize=3;
#endif

    /* throw away any frames in the list after this frame */
    lastFrame = frame;

    frame->sr = registers[(int) PS];
    frame->pc = registers[(int) PC];

    if (registers[(int) PS] & 0x2000)
    { 
        /* return to supervisor mode... */
        return_to_super();
    }
    else
    { /* return to user mode */
        return_to_user();
    }
}

int hex(ch)
char ch;
{
  if ((ch >= 'a') && (ch <= 'f')) return (ch-'a'+10);
  if ((ch >= '0') && (ch <= '9')) return (ch-'0');
  return (0);
}


/* scan for the sequence $<data>#<checksum>     */
void getpacket(buffer)
char * buffer;
{
  unsigned char checksum;
  unsigned char xmitcsum;
  int  i;
  int  count;
  char ch;
  
  do {
    /* wait around for the start character, ignore all other characters */
    while ((ch = getDebugChar()) != '$'); 
    checksum = 0;
    count = 0;
    
    /* now, read until a # or end of buffer is found */
    while (count < BUFMAX) {
      ch = getDebugChar();
      if (ch == '#') break;
      checksum = checksum + ch;
      buffer[count] = ch;
      count = count + 1;
      }
    buffer[count] = 0;

    if (ch == '#') {
      xmitcsum = hex(getDebugChar()) << 4;
      xmitcsum += hex(getDebugChar());
      if ((remote_debug ) && (checksum != xmitcsum)) {
        printf("bad checksum.  My count = 0x%x, sent=0x%x. buf=%s\n",
						     checksum,xmitcsum,buffer);
      }
      
      if (checksum != xmitcsum) putDebugChar('-');  /* failed checksum */ 
      else {
	 putDebugChar('+');  /* successful transfer */
	 /* if a sequence char is present, reply the sequence ID */
	 if (buffer[2] == ':') {
	    putDebugChar( buffer[0] );
	    putDebugChar( buffer[1] );
	    /* remove sequence chars from buffer */
	    count = strlen(buffer);
	    for (i=3; i <= count; i++) buffer[i-3] = buffer[i];
	 } 
      } 
    } 
  } while (checksum != xmitcsum);
  
}

/* send the packet in buffer.  The host get's one chance to read it.  
   This routine does not wait for a positive acknowledge.  */


void putpacket(buffer)
char * buffer;
{
  unsigned char checksum;
  int  count;
  char ch;
  
  /*  $<packet info>#<checksum>. */
  do {
  putDebugChar('$');
  checksum = 0;
  count    = 0;
  
  while (ch=buffer[count]) {
    if (! putDebugChar(ch)) return;
    checksum += ch;
    count += 1;
  }
  
  putDebugChar('#');
  putDebugChar(hexchars[checksum >> 4]);
  putDebugChar(hexchars[checksum % 16]);

  } while (1 == 0);  /* (getDebugChar() != '+'); */
  
}

static char  inbuffer[BUFMAX];
static char  outbuffer[BUFMAX];
static short error;


void debug_error(format, parm)
char * format;
char * parm;
{
  if (remote_debug) printf(format,parm);
}

/* convert the memory pointed to by mem into hex, placing result in buf */
/* return a pointer to the last char put in buf (null) */
char* mem2hex(mem, buf, count)
char* mem;
char* buf;
int   count;
{
      int i;
      unsigned char ch;
      for (i=0;i<count;i++) {
          ch = *mem++;
          *buf++ = hexchars[ch >> 4];
          *buf++ = hexchars[ch % 16];
      }
      *buf = 0; 
      return(buf);
}

/* convert the hex array pointed to by buf into binary to be placed in mem */
/* return a pointer to the character AFTER the last byte written */
char* hex2mem(buf, mem, count)
char* buf;
char* mem;
int   count;
{
      int i;
      unsigned char ch;
      for (i=0;i<count;i++) {
          ch = hex(*buf++) << 4;
          ch = ch + hex(*buf++);
          *mem++ = ch;
      }
      return(mem);
}

/* a bus error has occurred, perform a longjmp
   to return execution and allow handling of the error */

void handle_buserror()
{
  longjmp(env,1);
}

/* this function takes the 68000 exception number and attempts to 
   translate this number into a unix compatible signal value */
int computeSignal( exceptionVector )
int exceptionVector;
{
  int sigval;
  switch (exceptionVector) {
    case 2 : sigval = 10; break; /* bus error           */
    case 3 : sigval = 10; break; /* address error       */
    case 4 : sigval = 4;  break; /* illegal instruction */
    case 5 : sigval = 8;  break; /* zero divide         */
    case 6 : sigval = 16; break; /* chk instruction     */
    case 7 : sigval = 16; break; /* trapv instruction   */
    case 8 : sigval = 11; break; /* privilege violation */
    case 9 : sigval = 5;  break; /* trace trap          */
    case 10: sigval = 4;  break; /* line 1010 emulator  */
    case 11: sigval = 4;  break; /* line 1111 emulator  */
    case 31: sigval = 2;  break; /* interrupt           */
    case 33: sigval = 5;  break; /* breakpoint          */
    case 40: sigval = 8;  break; /* floating point err  */
    case 48: sigval = 8;  break; /* floating point err  */
    case 49: sigval = 8;  break; /* floating point err  */
    case 50: sigval = 8;  break; /* zero divide         */
    case 51: sigval = 8;  break; /* underflow           */
    case 52: sigval = 8;  break; /* operand error       */
    case 53: sigval = 8;  break; /* overflow            */
    case 54: sigval = 8;  break; /* NAN                 */
    default: 
      sigval = 7;         /* "software generated"*/
  }
  return (sigval);
}

/*
 * This function dispatches all command processing for tutor and gdb.
 */
void handle_exception(int exceptionVector)
{
  void gdb_loop();

  while (1) {   /* returns out of loop--otherwise switch gdb/tutor */
  if (tutor_flag)
    {
      if (tutor(exceptionVector)==1)  /* call tutor, retval = 1 to continue */
      _returnFromException (lastFrame); /* continuing with tutor */
      else
        tutor_flag = 0; /* tutor switching to gdb */
    }
  else {
    gdb_loop(exceptionVector); /* does own _returnFromExceptions */
    tutor_flag = 1; /* only returns here on gdb kill command */
    }
  }  /* end while */
}

void gdb_loop(int exceptionVector)
{
  int    sigval;
  int    addr, length;
  char * ptr;
  int    newPC;
  Frame  *frame;
  
  if (remote_debug) printf("vector=%d, sr=0x%x, pc=0x%x\n", 
			    exceptionVector,
			    registers[ PS ], 
			    registers[ PC ]);

  /* reply to host that an exception has occurred */
  sigval = computeSignal( exceptionVector );
  sprintf(outbuffer,"S%02x",sigval);
  putpacket(outbuffer); 

  while (1==1) { 
    error = 0;
    outbuffer[0] = 0;
    getpacket(inbuffer);
    switch (inbuffer[0]) {
      case '?' : sprintf(outbuffer,"S%02x",sigval);
                 break; 
      case 'd' : remote_debug = !(remote_debug);  /* toggle debug flag */
                 break; 
      case 'g' : /* return the value of the CPU registers */
                mem2hex((char*) registers, outbuffer, NUMREGBYTES);
                break;
      case 'G' : /* set the value of the CPU registers - return OK */
                hex2mem(&inbuffer[1], (char*) registers, NUMREGBYTES);
                strcpy(outbuffer,"OK");
                break;
      
      /* mAA..AA,LLLL  Read LLLL bytes at address AA..AA */
      case 'm' : 
	        if (setjmp(env) == 0) {
		    exceptionHandler(2,handle_buserror); 

		    if (2 == sscanf(&inbuffer[1],"%x,%x",&addr,&length)) {
		      mem2hex((char*) addr, outbuffer, length);
		    }
		    else {
		      strcpy(outbuffer,"E01");
		      debug_error("malformed read memory command: %s",inbuffer);
		      }     
                } 
		else {
		  exceptionHandler(2,_catchException);   
		  strcpy(outbuffer,"E03");
		  debug_error("bus error");
		  }     
                
		/* restore handler for bus error */
		exceptionHandler(2,_catchException);   
		break;
      
      /* MAA..AA,LLLL: Write LLLL bytes at address AA.AA return OK */
      case 'M' : 
	        if (setjmp(env) == 0) {
		    exceptionHandler(2,handle_buserror); 

		    if (2 == sscanf(&inbuffer[1],"%x,%x:",&addr,&length)) {
			for (ptr=inbuffer;*ptr!='\0';ptr++)
			  if (*ptr == ':') break;
		      ptr += 1; /* point 1 past the colon */
		      hex2mem(ptr, (char*) addr, length);
		      strcpy(outbuffer,"OK");
		    }
		    else {
		      strcpy(outbuffer,"E02");
		      debug_error("malformed write memory command: %s",inbuffer);
		      }     
                } 
		else {
		  exceptionHandler(2,_catchException);   
		  strcpy(outbuffer,"E03");
		  debug_error("bus error");
		  }     
                break;
     
     /* cAA..AA    Continue at address AA..AA(optional) */
     /* sAA..AA   Step one instruction from AA..AA(optional) */
     case 'c' : 
     case 's' : 
          /* try to read optional parameter, addr unchanged if no parm */
          if (1 == sscanf(&inbuffer[1],"%x",&registers[ PC ])); 
          newPC = registers[ PC];
          
          /* clear the trace bit */
          registers[ PS ] &= 0x7fff;
          
          /* set the trace bit if we're stepping */
          if (inbuffer[0] == 's') registers[ PS ] |= 0x8000;
          
          /*
           * look for newPC in the linked list of exception frames.
           * if it is found, use the old frame it.  otherwise,
           * fake up a dummy frame in returnFromException().
           */
          if (remote_debug) printf("new pc = 0x%x\n",newPC);
          frame = lastFrame;
          while (frame)
          {
              if (remote_debug)
                  printf("frame at 0x%x has pc=0x%x, except#=%d\n",
                         frame,frame->exceptionPC,
                         frame->exceptionVector);
              if (frame->exceptionPC == newPC) break;  /* bingo! a match */
              /*
               * for a breakpoint instruction, the saved pc may
               * be off by two due to re-executing the instruction
               * replaced by the trap instruction.  Check for this.
               */
              if ((frame->exceptionVector == 33) &&
                  (frame->exceptionPC == (newPC+2))) break;
              frame = frame->previous;
          }
          
          /*
           * If we found a match for the PC AND we are not returning
           * as a result of a breakpoint (33),
           * trace exception (9), nmi (31), jmp to
           * the old exception handler as if this code never ran.
           */
          if (frame) 
          {
              if ((frame->exceptionVector != 9)  && 
                  (frame->exceptionVector != 31) && 
                  (frame->exceptionVector != 33))
              { 
                  /*
                   * invoke the previous handler.
                   */
                  if (oldExceptionHook)
                      (*oldExceptionHook) (frame->exceptionVector);
                  newPC = registers[ PC ];    /* pc may have changed  */
                  if (newPC != frame->exceptionPC)
                  {
                      if (remote_debug)
                          printf("frame at 0x%x has pc=0x%x, except#=%d\n",
                                 frame,frame->exceptionPC,
                                 frame->exceptionVector);
                      /* dispose of this frame, we're skipping it (longjump?)*/
                      lastFrame = frame->previous;
                      frame = (Frame *) 0;
                  }
              }
          }         

          _returnFromException( frame );

          break;
          
      /* kill the program */
      case 'k' :  
        return;   /* go back to tutor */
      } /* switch */ 
    
    /* reply to the request */
    putpacket(outbuffer); 
    }
}


/* this function is used to set up exception handlers for tracing and 
   breakpoints */
void set_debug_traps()
{
  extern void _debug_level7();
  extern void remcomHandler();
  int exception;

  for (exception = 2; exception <= 30; exception++)
    exceptionHandler(exception,_catchException);   

  /* level 7 interrupt              */
  exceptionHandler(31,_debug_level7);    
  
  /* breakpoint exception (trap #1) */
  exceptionHandler(33,_catchException);
  
  /* floating point error (trap #8) */
  exceptionHandler(40,_catchException);
  
  /* 48 to 54 are floating point coprocessor errors */
  for (exception = 48; exception <= 54; exception++)
    exceptionHandler(exception,_catchException);   

  if (oldExceptionHook != remcomHandler)
    {
      oldExceptionHook = exceptionHook;
      exceptionHook    = remcomHandler;
    }
  
  initialized = 1;
  tutor_flag = 1;		/* for now */

  lastFrame = (Frame *) &frameStack[FRAMESIZE] - 1;
  lastFrame->previous = (Frame *) 0;
}

/* This function will generate a breakpoint exception.  It is used at the
   beginning of a program to sync up with a debugger and can be used
   otherwise as a quick means to stop program execution and "break" into
   the debugger. */
   
void breakpoint()
{
  if (initialized) BREAKPOINT();
}

