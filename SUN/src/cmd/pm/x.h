struct exec {
	unsigned short	a_machtype;	
	unsigned short	a_magic;	
	unsigned long	a_text;		
	unsigned long	a_data;		
	unsigned long	a_bss;		
	unsigned long	a_syms;		
	unsigned long	a_entry;	
	unsigned long	a_trsize;	
	unsigned long	a_drsize;	
};
#define	OMAGIC	0407		
#define	NMAGIC	0410		
#define	ZMAGIC	0413		
#define M_OLDSUN2	0	
#define M_68010		1	
#define M_68020		2	
#define	PAGSIZ		0x2000
#define	SEGSIZ		0x20000
#define	OLD_PAGSIZ	0x800	
#define	OLD_SEGSIZ	0x8000	
#define	N_BADMAG(x) \
	((x).a_magic!=OMAGIC && (x).a_magic!=NMAGIC && (x).a_magic!=ZMAGIC)
#define	N_PAGSIZ(x) \
	((x).a_machtype == M_OLDSUN2? OLD_PAGSIZ : PAGSIZ)
#define	N_SEGSIZ(x) \
	((x).a_machtype == M_OLDSUN2? OLD_SEGSIZ : SEGSIZ)
#define	N_TXTOFF(x) \
	 \
	( (x).a_machtype == M_OLDSUN2 \
	? ((x).a_magic==ZMAGIC ? N_PAGSIZ(x) : sizeof (struct exec)) \
	: ((x).a_magic==ZMAGIC ? 0 : sizeof (struct exec)) )
#define	N_SYMOFF(x) \
	 \
	(N_TXTOFF(x)+(x).a_text+(x).a_data+(x).a_trsize+(x).a_drsize)
#define	N_STROFF(x) \
	 \
	(N_SYMOFF(x) + (x).a_syms)
#define N_TXTADDR(x) \
	((x).a_machtype == M_OLDSUN2? N_SEGSIZ(x) : N_PAGSIZ(x))
#define N_DATADDR(x) \
	(((x).a_magic==OMAGIC)? (N_TXTADDR(x)+(x).a_text) \
	: (N_SEGSIZ(x)+((N_TXTADDR(x)+(x).a_text-1) & ~(N_SEGSIZ(x)-1))))
#define N_BSSADDR(x)  (N_DATADDR(x)+(x).a_data)
struct relocation_info {
	long	r_address;	
unsigned int	r_symbolnum:24,	
		r_pcrel:1, 	
		r_length:2,	
		r_extern:1,	
		:4;		
};
struct	nlist {
	union {
		char	*n_name;	
		long	n_strx;		
	} n_un;
	unsigned char	n_type;		
	char	n_other;		
	short	n_desc;			
	unsigned long	n_value;	
};
#define	N_UNDF	0x0		
#define	N_ABS	0x2		
#define	N_TEXT	0x4		
#define	N_DATA	0x6		
#define	N_BSS	0x8		
#define	N_COMM	0x12		
#define	N_FN	0x1f		
#define	N_EXT	01		
#define	N_TYPE	0x1e		
#define	N_STAB	0xe0		
#define	N_FORMAT	"%08x"
#define	NULLPTR	(char *)0
struct	devsw	{			
	int	dvnum;
	char	*dvname;
	int	(*dvinit)();
	int	(*dvopen)();
	int	(*dvclose)();
	int	(*dvread)();
	int	(*dvwrite)();
	int	(*dvseek)();
	int	(*dvgetc)();
	int	(*dvputc)();
	int	(*dvcntl)();
	int	dvcsr;
	int	dvivec;
	int	dvovec;
	int	(*dviint)();
	int	(*dvoint)();
	char	*dvioblk;
	int	dvminor;
	};
extern	struct	devsw devtab[];		
#define	CONSOLE     0			
#define	ETHER       1			
#define	INTERNET    2			
#define	DGRAM1      3			
#define	DGRAM2      4			
#define	DGRAM3      5			
#define	DGRAM4      6			
#define	RFILSYS     7			
#define	RFILE1      8			
#define	RFILE2      9			
#define	RFILE3      10			
#define	RFILE4      11			
#define	NAMESPACE   12			
#define	Ntty	1
#define	Neth	1
#define	Ndgm	1
#define	Ndg	4
#define	Nrfm	1
#define	Nrf	4
#define	Nnam	1
#define	NDEVS	13
extern	int	ttyinit();
extern	int	ionull();
extern	int	ttyread();
extern	int	ttywrite();
extern	int	ioerr();
extern	int	ttycntl();
extern	int	ttygetc();
extern	int	ttyputc();
extern	int	ethinit();
extern	int	ethread();
extern	int	ethwrite();
extern	int	ethinter();
extern	int	dgmopen();
extern	int	dgmcntl();
extern	int	dginit();
extern	int	dgclose();
extern	int	dgread();
extern	int	dgwrite();
extern	int	dgcntl();
extern	int	rfopen();
extern	int	rfcntl();
extern	int	rfinit();
extern	int	rfclose();
extern	int	rfread();
extern	int	rfwrite();
extern	int	rfseek();
extern	int	rfgetc();
extern	int	rfputc();
extern	int	naminit();
extern	int	namopen();
#define	NPROC	    30			
#define	NSEM	    50			
#define	MEMMARK				
#define	RTCLOCK				
#define	STKCHK				
#define	NETDAEMON			
#define	GATEWAY	     128,10,2,1		
#define	TSERVER	    "128.10.2.3:37"	
#define	RSERVER	    "128.10.2.88:2001"	
#define	NSERVER	    "128.10.2.8:53"	
#define	VERSION	    "7.0 SUN3 (02/29/88)"
typedef	char		Bool;		
#define	FALSE		0		
#define	TRUE		1
#define	NULLCH		'\0'		
#define	NULLSTR		""		
#define	SYSCALL		WORD		
#define	LOCAL		static 		
#define	COMMAND		int		
#define	BUILTIN		int		
#define	INTPROC		WORD		
#define	PROCESS		WORD		
#define	RESCHYES	1		
#define	RESCHNO		0		
#define	MININT		0x8000
#define	MAXINT		0x7fff
#define	LOWBYTE		0377		
#define	HIBYTE		0177400		
#define	LOW16		0177777		
#define	MINSTK		200		
#define	NULLSTK		0x1000		
#define	DISABLE		0x2700
#define	MAGIC		0125252		
#define	OK		 1		
#define	SYSERR		MINLONG		
#define	EOF		-2		
#define	TIMEOUT		-3		
#define	INTRMSG		-4		
#define	INITSTK		0x4000		
#define	INITPRIO	20		
#define	INITNAME	"main"		
#define	INITARGS	1,0		
#define	INITRET		userret		
#define	INITPS		0x2000		
#define	INITREG		0		
#define	QUANTUM		10		
typedef	char	CHAR;		
typedef	long	WORD;		
typedef	char	*PTR;		
typedef int	INT;		
typedef	long	REG;		
#define	SAVEDPS		INT		
#define	MINSTK		200		
#define	NULLSTK		0x1000		
#define HINTLEV   	8		
#define MAXSER		6		
#define	SSP		15 		
#define	PS		16
#define	PC		17
#define MAXLONG		0x7fffffff	
#define MINLONG		0x80000000
typedef short	STATWORD[1];	
#define	isodd(x)	(01&(WORD)(x))
#define	min(a,b)	( (a) < (b) ? (a) : (b) )
extern	int	rdyhead, rdytail;
extern	int	preempt;
extern int noint;
#ifndef	NQENT
#define	NQENT		NPROC + NSEM + NSEM + 4	
#endif
struct	qent	{		
	int	qkey;		
	int	qnext;		
	int	qprev;		
	};
extern	struct	qent q[];
extern	int	nextqueue;
#define	isempty(list)	(q[(list)].qnext >= NPROC)
#define	nonempty(list)	(q[(list)].qnext < NPROC)
#define	firstkey(list)	(q[q[(list)].qnext].qkey)
#define lastkey(tail)	(q[q[(tail)].qprev].qkey)
#define firstid(list)	(q[(list)].qnext)
#define	EMPTY	-1		
#ifndef	NPROC				
#define	NPROC		30		
#endif
#ifndef	_NFILE
#define _NFILE		20		
#endif
#define	FDFREE		-1		
#define	PRCURR		'\001'		
#define	PRFREE		'\002'		
#define	PRREADY		'\003'		
#define	PRRECV		'\004'		
#define	PRSLEEP		'\005'		
#define	PRSUSP		'\006'		
#define	PRWAIT		'\007'		
#define	PRTRECV		'\010'		
#define	PNREGS		19		
#define	PNMLEN		16		
#define	NULLPROC	0		
#define	BADPID		-1		
#define	isbadpid(x)	(x<=0 || x>=NPROC)
struct	pentry	{
	char	pstate;			
	int	pprio;			
	WORD	pregs[PNREGS];		
	int	psem;			
	WORD	pmsg;			
	char	phasmsg;		
	WORD	pbase;			
	int	pstklen;		
	WORD	plimit;			
	char	pname[PNMLEN];		
	int	pargs;			
	WORD	paddr;			
	WORD	pnxtkin;		
	short	pdevs[2];		
	int	fildes[_NFILE];		
};
extern	struct	pentry proctab[];
extern	int	numproc;		
extern	int	nextproc;		
extern	int	currpid;		
#ifndef	NSEM
#define	NSEM		50	
#endif
#define	SFREE	'\01'		
#define	SUSED	'\02'		
struct	sentry	{		
	char	sstate;		
	int	semcnt;		
	int	sqhead;		
	int	sqtail;		
};
extern	struct	sentry	semaph[];
extern	int	nextsem;
#define	isbadsem(s)	(s<0 || s>=NSEM)
#define IOCHERR		0200		
#define	OBMINSP		40		
#define	EBUFLEN		50		
#ifndef	Ntty
#define	Ntty		1		
#endif
#ifndef	IBUFLEN
#define	IBUFLEN		512		
#endif
#ifndef	OBUFLEN
#define	OBUFLEN		512		
#endif
#define	IMRAW		'R'		
#define	IMCOOKED	'C'		
#define	IMCBREAK	'K'		
#define	OMRAW		'R'		
struct	tty	{			
	int	ihead;			
	int	itail;			
	char	ibuff[IBUFLEN];		
	int	isem;			
	int	icnt;			
	int	ohead;			
	int	otail;			
	char	obuff[OBUFLEN];		
	int	osem;			
	int	ocnt;			
	int	odsend;			
	int	ehead;			
	int	etail;			
	char	ebuff[EBUFLEN];		
	char	imode;			
	Bool	iecho;			
	Bool	ieback;			
	Bool	evis;			
	Bool	ecrlf;			
	Bool	icrlf;			
	Bool	ierase;			
	char	ierasec;		
	Bool	ikill;			
	char	ikillc;			
	Bool	iintr;			
	char	iintrc;			
	int	iintpid;		
	Bool	ieof;			
	char	ieofc;			
	int	icursor;		
	Bool	oflow;			
	Bool	oheld;			
	char	ostop;			
	char	ostart;			
	Bool	ocrlf;			
	char	ifullc;			
	struct	zscc_device	*ioaddr;	
	int	intrstate;		
	Bool	tbusy;			
	char	constatus;		
};
extern	struct	tty tty[];
#define	ATSIGN	'@'
#define	BACKSP	'\b'
#define	BELL	'\007'
#define	BLANK	' '
#define	EOFC	'\004'			
#define	KILLCH	'\025'			
#define	NEWLINE	'\n'
#define	RETURN	'\r'
#define	STOPCH	'\023'			
#define	STRTCH	'\021'			
#define	INTRCH	'\002'			
#define	UPARROW	'^'
#define	TCSETBRK	1		
#define	TCRSTBRK	2		
#define	TCNEXTC		3		
#define	TCMODER		4		
#define	TCMODEC		5		
#define	TCMODEK		6		
#define	TCICHARS	8		
#define	TCECHO		9		
#define	TCNOECHO	10		
#define	TCINT		11		
#define	TCINTCH		12		
#define	TCNOINT		13		
#define	TFULLC		BELL		
