/* dlc.h */

/* driver states */

#define	DLIINIT		0		/* initial input (nonblockmode) */
#define	DLIREAD		1		/* doing block input		*/
#define	DLIREADY	2		/* ready to begin block mode	*/
#define	DLIWAIT		3		/* waiting for start of block	*/
#define	DLIDONE		4		/* block mode read completed 	*/
#define	DLOINIT		0		/* initial output (nonblockmode)*/
#define	DLOREADY	1		/* ready to begin block mode	*/
#define	DLOWRITE	2		/* doing block mode write	*/
#define	DLOSTALL	3		/* stall before sending frame	*/
#define	DLODONE		4		/* blockmode write completed	*/
#define	DLORESTART	5		/* reset and resend block	*/

/* character stuffing constants */

#define	DLESC		'\251'		/* block mode ESCape character	*/
#define	DLRESTART	'\257'		/* RESTART character: sent from	*/
					/*  blockmode receiver BACK TO	*/
					/*  blockmode transmitter	*/
#define	DLSTART		'\253'		/* START of frame character	*/
#define	DLEOB		'\252'		/* End Of Block (frame) char.	*/
#define	DLESCPED	0250		/* char escaped if it's 025x	*/
#define	DLESCMASK	0770		/* mask of bits to test for esc	*/
#define	DLESCBIT	0200		/* bit that is masked on escape	*/
#define	DLEBMASK	0177		/* mask for other 7 bits	*/
#define	DLNSTALL	3		/* number of times to stall	*/

/* dlc control function codes */

#define	DCSETREC	1		/* set normal mode process id	*/
#define	DCCLRREC	2		/* clear normal mode process id	*/

struct	dlblk	{			/* dlc control block		*/
	struct	csr	*dioaddr;	/* csr address			*/
	char	distate, dostate;	/* input and output states	*/
	int	dicount;		/* current number of chars read	*/
	int	docount;		/* number of chars left to write*/
	int	diproc;			/* process to resume after read	*/
	int	doproc;			/* process to resume after write*/
	char	*dinext;		/* next input buffer location	*/
	char	*donext;		/* next output buffer location	*/
	char	*distart;		/* addr of start of input buffer*/
	char	*dostart;		/* addr of start of output "	*/
	int	dimax;			/* maximum characters to read	*/
	int	dotot;			/* total characters to write	*/
	Bool	diesc;			/* true iff next input escaped	*/
	Bool	doesc;			/* true iff DLESC just sent and	*/
					/* next char should be escaped	*/
	int	dostall;		/* num. of times output stalled	*/
	int	dpid;			/* non-blockmode input process	*/
	char	dochar;			/* non-blockmode output char.	*/
	Bool	dovalid;		/* true iff dochar valid	*/
	};

#ifndef	Ndlc
#define	Ndlc	1
#endif

extern	struct	dlblk	dlc[];
