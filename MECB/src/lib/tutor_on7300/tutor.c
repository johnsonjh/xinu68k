/*
 *		originally written by  Yasuhiro Endo
 * 				        for CS241
 *					in  Oct., 1989
 *              edited by Betty O'Neil for ATT7300 Jan, '91
 *
 *  Commands: accept lower or upper case in any combination. One line commands.
 *
 *  md <hex address>   Display contents of a single byte at specified address.
 *
 *  ms <hex address> <new value>   Store new value into specified byte.
 * 		     <new value> = decimal integer or char in single quotes.
 *
 *  h [command]   Print information about command, if none, print all.
 *
 */

#include <stdio.h>
#include "slex.h"
#include "cpu.h"

#define MAXBYTE 0xFF	/* biggest value which a byte long data can have */
#define LINELEN 80
#define CTRL_A  0x01

extern int registers[];

/* in mem.c-- */
extern int disp_regs ( char * line, struct cmd * cp );
extern int mem_display ( char * line, struct cmd * cp );
extern int mem_set     ( char * line, struct cmd * cp );
extern int mem_mod     ( char * line, struct cmd * cp );
extern int dotSR       ( char *line, struct cmd *cp);
int help ( char * line, struct cmd * cp );
int trans_mode  ( char * line, struct cmd * cp );
int slu_ini (char * line, struct cmd *cp);
int slu_tst (char *line, struct cmd *cp);
int slu_baud (char *line, struct cmd *cp);
int load_file (char *line, struct cmd *cp);
int gdb_switch (char *line, struct cmd *cp);
int go_cmd ( char * line, struct cmd * cp);
int set_console (char *line, struct cmd *cp);
int quit ( char * line, struct cmd * cp );
int trace (char * line, struct cmd *cp );
int brkpoint (char *line, struct cmd *cp);
int unbrkpoint (char *line, struct cmd *cp);

/* --command table for tutor-- */

struct cmd cmds[] = {
                     "DF" , disp_regs  , "Display regs    : DF",
		     ".SR", dotSR      , "Display/set SR  : .SR [<val>]",
		     "MD" , mem_display, "Memory display  : MD <addr>",
		     "MS" , mem_set    , "Memory set      : MS <addr> <val>",
		     "MM" , mem_mod    , "Memory modify   : MM <addr>[;L]...",
		     "H"  , help       , "Display help    : H [topic]",
#ifdef OBS
		     "CO" , set_console, "Set console     : CO <dev>",
		     "SI" , slu_ini    , "SLU init        : SI",
		     "TM" , trans_mode , "Transparent mode: TM",
#endif
		     "BAUD", slu_baud  , "Set baudrate    : BAUD <baudrate>",
		     "LO" , load_file  , "Download        : LO",
		     "GO"  , go_cmd    , "GO, start/resume: GO [<addr>]",
		     "BR"  , brkpoint  , "Set breakpoint  : BR [<addr>]",
		     "NOBR", unbrkpoint, "Unset breakpoint: NOBR [<addr>]",
		     "T"  , trace      , "Trace           : T",
		     "GDB" ,gdb_switch , "Remote gdb      : GDB",
		     NULL , NULL       , NULL};
					/* null cmd to flag end of table */

int tutor(int vector)
{
    int done = 0;
    int cnum,linepos;
    char linebuf[LINELEN];
    static int first = 1;

#ifdef NEED_CONSOLE_FLEXIBILITY
    if (first) {  /* see which line gives cr-- */
      int c;

      first = 0;
      /* type out on default console (serial line at back)--
         user of keyboard console has to know to do this s prompt */
      printf("Type CR to confirm Console setting: ");

      while (1) {
        if ((c = checkc(1)) == '\r')
	  break;
	if ((c = checkc(3)) == '\r') {
          slu_set_console(3);
	  break;
        }
      }
    }
#endif
    if (adjust_bps_on_next_entry())     /* fix up bkpts, trace bit */
      return;                            /* was "extra" trace--keep going */
    printf("pc = %x, exception %d: ",registers[PC],vector);
    print_trap(vector);
    printf("\n");
    
    while (!done) {
        /* get cmd token from user by parsing first part of line in linebuf-- */
        getcmd(linebuf,&cnum,&linepos);
        /* call implementation routine for matched cmd-- */
        /* (remaining part of line to parse starts at linebuf+linepos) */
        done = cmds[cnum].cmdfn (linebuf+linepos,&cmds[cnum]); 
    }
  return done;   /* 1 for continue in tutor, 2 for gdb processing */
}

/* Better than scanf--read whole line in, parse resulting string */
/* Uses slex package to match cmd token in first part of linebuf */
/*  and return command # and where to continue parsing           */
/* Here getcmd loops until user provides proper command, so no error return */

getcmd( char * linebuf, int * cnum_ptr, int * pos_ptr) {

    /* cnum_ptr :  returned command number */
    /* pos_ptr  :  new position in linebuf (after cmd token)*/

    int done = 0;
    int cnum, newpos;

    while (!done) {
        printf("\nTutor> ");
	get_edited_line(linebuf); /* read line from user, null terminate */
	/* match cmd token to get cnum--call slex package to do lookup-- */
	if (slex(linebuf,cmds,&cnum,&newpos)==0)
	    done = 1;	/* success in matching cmd */
	else
	    printf("No such command: %s\n",linebuf);
    }
    *cnum_ptr = cnum;
    *pos_ptr = newpos;
    return done;		/* bug in supplied code--this was missing! */
}

/*
 *  ===================================================================
 *		command			routines		     
 *  =================================================================== 
 *  Note that each command routine is called with 2 args, the remaining 
 *  part of the line to parse and a pointer to the struct cmd for this  
 *  command. Each returns with 0 for continue or 1 for all-done
 *
 */

int trace ( char * line, struct cmd * cp)
{
  set_trace();
  resume (-1);  /* from here */
  return 1;
}

int go_cmd ( char * line, struct cmd * cp)
{
	int addr;

  if (sscanf(line,"%x",&addr)==1) 
    resume(addr);
  else
    resume(0);  /* diddle breakpoint if necessary */
  return 1;
}
int brkpoint( char *line, struct cmd *cp)
{
  int addr;

  if (sscanf(line,"%x",&addr)==1)
    set_bp(addr);
  else
    show_bp();
  return 0;
}
int unbrkpoint( char *line, struct cmd *cp)
{
  int addr;

  if (sscanf(line,"%x",&addr)==1)
    clear_bp(addr);
  else
    clear_bp(-1);
  return 0;
}

/* Display contents of a single byte in hex. */
#ifdef OBS
int mem_display ( char * line, struct cmd * cp ) {

    unsigned char * addr;

    if ( sscanf ( line, "%X", &addr ) == 1 )
	printf ( "%06X: %02X\n", addr, * addr );
    else
	printf ( "\nWhat!\n" );
    return 0;
}
#endif

/* trans_mode - transparent mode */

int trans_mode ( char * line, struct cmd * cp ) {

    int exit = 0;
    int c;

    printf("TRANSPARENT MODE  $01=CTRL-A to exit\n");
    while ( !exit ) {
	if ((c = checkc(1))>=0 ) { /* something from one line?*/
	  if ( c == CTRL_A ) /* control-A? */
	    exit = 1;		/* yes, done here */
	  else {
            rawputc(3,c);
	   }
         }
	if ((c = checkc(3))>=0 ) { /* something from other line? */
	   if ( c == CTRL_A ) /* yes, control-A? */
	     exit = 1;
           else
             rawputc(1,c);
	}
    }
    return 0;
}



int help ( char * line, struct cmd * cp ) {
 
    int cnum;			/* command number */
    int newpos;			/* new position on the line */

    if ( slex( line, cmds, &cnum, &newpos ) == 0 )
        printf ( "%s\n", cmds[cnum].help );
    else 
	for ( cnum = 0; cmds[cnum].help != NULL ; cnum++ )
	    printf ( "%s\n", cmds[cnum].help );
    return 0;
}

int slu_ini( char * line, struct cmd *cp)
{
  int dev;
    
   if (sscanf(line,"%d",&dev)==1)
     slu_init(dev);
   else 
     printf("Unknown dev %d\n",dev);
   return 0;
}

int slu_baud(char *line, struct cmd *cp)
{
  int baudrate;

   if (sscanf(line, "%d",&baudrate)==1)
     set_baudrate(baudrate);
   else
    printf("Need baudrate (9600 or whatever)\n");
   return 0;
}
set_console(char *line, struct cmd *cp)
{
   int dev;

   if (sscanf(line,"%d",&dev)==1)
    slu_set_console(dev);
   else
    printf("Unknown dev %d\n",dev);
   return 0;
}

int load_file(char *line, struct cmd *cp)
{
   loadfile();
   init_bps();
   return 0;
}

gdb_switch(char *line, struct cmd *cp)
{
  return 2;
}

