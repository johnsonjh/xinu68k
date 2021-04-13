/* kernel.h - disable, enable, halt, restore, isodd, min */

/* Symbolic constants used throughout Xinu */

typedef	char		Bool;		/* Boolean type			*/
#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
/*#define	NULL		(char *)0	/* Null pointer for linked lists*/
#define	NULL		0		/* Null pointer for linked lists*/
#define	NULLCH		'\0'		/* The null character		*/
#define	NULLSTR		""		/* Pointer to empty string	*/
#define	SYSCALL		WORD		/* System call declaration	*/
#define	LOCAL		static 		/* Local procedure declaration	*/
#define	COMMAND		int		/* Shell command declaration	*/
#define	BUILTIN		int		/* Shell builtin " "		*/
#define	INTPROC		WORD		/* Interrupt procedure  "	*/
#define	PROCESS		WORD		/* Process declaration		*/
#define	RESCHYES	1		/* tell	ready to reschedule	*/
#define	RESCHNO		0		/* tell	ready not to resch.	*/
#define	MININT		0x8000
#define	MAXINT		0x7fff
#define	LOWBYTE		0377		/* mask for low-order 8 bits	*/
#define	HIBYTE		0177400		/* mask for high 8 of 16 bits	*/
#define	LOW16		0177777		/* mask for low-order 16 bits	*/
#define	MINSTK		200		/* minimum process stack size	*/
#define	NULLSTK		0x1000		/* process 0 stack size		*/
#define	DISABLE		0x2700
#define	MAGIC		0125252		/* unusual value for top of stk	*/

/* Universal return constants */

#define	OK		 1		/* system call ok		*/
#define	SYSERR		MINLONG		/* system call failed		*/
#define	EOF		-2		/* End-of-file (usu. from read)	*/
#define	TIMEOUT		-3		/* time out  (usu. recvtim)	*/
#define	INTRMSG		-4		/* keyboard "intr" key pressed	*/
					/*  (usu. defined as ^B)	*/
/* Initialization constants */

#define	INITSTK		0x4000		/* initial process stack size	*/
#define	INITPRIO	20		/* initial process priority	*/
#define	INITNAME	"main"		/* initial process name		*/
#define	INITARGS	1,0		/* initial count/arguments	*/
#define	INITRET		userret		/* processes return address	*/
#define	INITPS		0x2000		/* initial process PS		*/
#define	INITREG		0		/* initial register contents	*/
#define	QUANTUM		10		/* clock ticks until preemption	*/



/* Machine size definitions						*/

typedef	char	CHAR;		/* sizeof the unit the holds a character*/
typedef	long	WORD;		/* maximum of (int, char *)		*/
typedef	char	*PTR;		/* sizeof a char. or fcnt. pointer 	*/
typedef int	INT;		/* sizeof compiler integer		*/
typedef	long	REG;		/* sizeof machine register		*/

/* Machine dependent type definitions	*/

#define	SAVEDPS		INT		

#define	MINSTK		200		/* minimum process stack size	*/
#define	NULLSTK		0x1000		/* process 0 stack size		*/
#define HINTLEV   	8		/* highest autovector level, LL */
#define MAXSER		6		/* max. device per autovector LL*/

/* Machine dependent constants	*/

#define	SSP		15 		/* Supervisor stack pointer	*/
#define	PS		16
#define	PC		17
#define MAXLONG		0x7fffffff	
#define MINLONG		0x80000000

typedef short	STATWORD[1];	/* saved machine status for disable/restore */
				/* by declaring it to be an array, the	*/
				/* name provides an address so forgotten*/
				/* &'s don't become a problem		*/

/* Miscellaneous utility inline functions */
#define	isodd(x)	(01&(WORD)(x))
#define	min(a,b)	( (a) < (b) ? (a) : (b) )

extern	int	rdyhead, rdytail;

extern	int	preempt;

extern int noint;








