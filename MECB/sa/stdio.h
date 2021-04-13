/* stdio.h for Xinu or standalone programs, MECB or ATT7300-- */

/* from UNIX stdio, and valid here too-- */
#define	NULL	0
extern char	*gets();
extern char     *fgets();

#define EOF (-1)
#define EOFCHAR 4

/* Device numbers to correspond to Xinu devs-- */
/* Used for getc, putc (and these are used by C library) */

/* On ATT7300, the RS232 port at back, with NEC7201 UART, channel A-- */
/* On MECB, the console MC6850-- */
#define CONSOLE 0

/* On ATT7300, the secondary channel, channel B, of the NEC7201-- */
/* On MECB, the hostline MC6850-- */
#define TTYB 1
/* another name for secondary serial tty line-- */
#define OTHER 1

/* On ATT7300, the keyboard interface, with a 6850 UART-- */
/* On MECB, possible added 6850 in p. 3-- */
#define TTYC 2 

#define	RING0IN     3			/* type dlc      */
#define	RING0OUT    4			/* type dlc      */
#define	LPR         5			/* type lp       */

#define SYSERR      -1

