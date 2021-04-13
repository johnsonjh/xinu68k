/* kernel.h - disable, enable, halt, restore, isodd */

/* Symbolic constants used throughout Xinu */

typedef	char		Bool;		/* Boolean type			*/

/* Machine size definitions						*/

typedef	char	CHAR;		/* sizeof the unit the holds a character*/
typedef	int	WORD;		/* maximum of (int, char *)		*/
typedef	char	*PTR;		/* sizeof a char. or fcnt. pointer 	*/
typedef int	INT;		/* sizeof compiler integer		*/
typedef	int	REG;		/* sizeof machine register		*/

/* Machine dependent type definitions	*/

#ifdef	lsi11
#define	SAVEDPS		CHAR		/* Saved processor status type 	*/
#else
#define	SAVEDPS		INT		
#endif

#define	SYSCALL		WORD		/* System call declaration	*/
#define	LOCAL		static WORD	/* Local procedure declaration	*/
#define	INTPROC		WORD		/* Interrupt procedure  "	*/
#define	PROCESS		WORD		/* Process declaration		*/
#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
#define	NULL		(char *)0	/* Null pointer for linked lists*/
#define	RESCHYES	1		/* tell	ready to reschedule	*/
#define	RESCHNO		0		/* tell	ready not to resch.	*/
#define	MINSTK		800		/* minimum process stack size	*/
#define	NULLSTK		800		/* process 0 stack size		*/
#define	OK		1		/* returned when system	call ok	*/

/* Machine dependent constants	*/

#ifdef	lsi11
#define	SP		6		/* reg.	6 is stack pointer	*/
#define	PC		7		/* reg.	7 is program counter	*/
#define	PS		8		/* proc. status	in 8th reg. loc	*/
#define	INITPS		0		/* initial proc. status		*/
#define	MININT		0x8000		/* minimum integer (-32768)	*/
#define	MAXINT		0x7fff		/* maximum integer		*/
#define	MINLONG		0x8000		/* minimum long (-32768)	*/
#define	MAXLONG		0x7fff		/* maximum long			*/
#define	SYSERR		-1		/* returned when sys. call fails*/
#define	DISABLE		0340		/* PS to disable interrupts	*/

#else
#define	SSP		15 		/* Supervisor stack pointer	*/
#define	PS		16
#define	PC		17
#define USP		18		/* User stack pointer		*/
#define	INITPS		0x2000		/* initial proc. status		*/
#define	MININT		0x80000000
#define	MAXINT		0x7fffffff
#define MAXLONG		0x7fffffff	
#define MINLONG		0x80000000
#define	SYSERR		-1
#define	DISABLE		0x2700
#endif

/* initialization constants */

#define	INITARGC	1		/* initial process argc		*/
#define	INITSTK		1000		/* initial process stack size	*/
#define	INITPRIO	20		/* initial process priority	*/
#define	INITNAME	"main"		/* initial process name		*/
#define	INITRET		userret		/* processes return address	*/
#define	INITREG		0		/* initial register contents	*/
#define	QUANTUM		10		/* clock ticks until preemption	*/

/* misc. utility inline	functions */

#define	isodd(x)	(01&(WORD)(x))

/* Machine dependent machine status functions */

#ifdef	lsi11
#define	disable(ps)	asm("mfps ~ps");asm("mtps $0340")
#define	restore(ps)	asm("mtps ~ps")	/* restore interrupt status	*/
#define	enable()	asm("mtps $000")/* enable interrupts		*/
#define	pause()		asm("wait")	/* machine "wait for interr."	*/
#define	halt()		asm("halt")	/* machine halt	instruction	*/

#else
/*	Note: disable and restore are now true procedures		*/
#define	enable()	asm("	movw #0x2000, sr")
					/* machine wait for intr.	*/
#define	pause()		asm("pause:	jmp pause")
					/* machine halt instruction	*/
#define	halt()		asm("halt:	jmp halt")
#endif

extern	int	rdyhead, rdytail;

#ifdef	RTCLOCK
extern	int	preempt;
#endif
