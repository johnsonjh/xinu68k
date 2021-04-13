/* frame.h - getfto, getfrom, getflen, setfto, setfa, setfrom */

#define	FRACK		'\030'	/* frame ACK/NACK/SACK codes -- frame	*/
#define	FRNACK		'\060'	/*   sequence number appears in low-	*/
#define	FRSACK		'\070'	/*   order 3 bits of each		*/
#define	FRCODE		070	/* mask for "code" part of ACK/NACK	*/
#define	FRSEQ		07	/* mask for sequence part of ACK/NACK	*/
#define	FRBCAST		0	/* broadcast address			*/
#define	FRTIME		4	/* number of seconds to timeout a frame	*/
#define	FRFAIL		2	/* number of failures for sack		*/
#define	FRMINLEN	3	/* minimum legal frame length		*/
#define	FRMAXLEN	128	/* maximum bytes in a frame		*/
#define	FRMAXSEQ	3	/* maximum sequence number for frames	*/
#define	FRSTACK		500	/* words of stack needed by frame proc.	*/
#define	FRLEN		sizeof(struct frame)
#define NFRAMES		40	/* total number of frame buffers	*/
#define	FRIPRIO		30	/* input process priority		*/
#define	FROPRIO		25	/* output process priority		*/
#define	FRTPRIO		100	/* priority of timer process		*/
#define	FIPORTS		20	/* size of input port queue		*/
#define	FOPORTS		8	/* size of output port queue		*/
#define	FFPORTS		10	/* size of forwarding queue		*/
#define	FRTMSG		8191	/* timeout message for transmitter	*/

#define	getfto(x)	( ((x)&070) >>3)
#define	getfrom(x)	( (x) & 07 )
#define	getflen(fptr)	( fptr->frlen & 0377 )
#define	setfto(fptr, x)	fptr->fraddr = (fptr->fraddr & ~070) | ((x&07)<<3)
#define	setfrom(fptr,x)	fptr->fraddr = (fptr->fraddr & ~07)  | (x&07)
#define	setfa(fptr,t,f)	fptr->fraddr = ( (t&07)<<3 ) | ( f&07 )

/* frame structure */

struct	frame	{
	char	fraddr;		/* from/to addresses, each 3 bits	*/
	char	frseq;		/* frame sequence number		*/
	char	frlen;		/* length of frame including fraddr	*/
				/* through all data.  Does not count	*/
				/* characters added by dlc		*/
	char	frdata[FRMAXLEN];
};

struct	fglob	{		/* global frame-level data		*/
	int	fiport;		/* local machine input port		*/
	int	foport;		/* local machine output port		*/
	int	ffport;		/* forwarded frame port			*/
	int	fosem;		/* output waiting semaphore		*/
	int	findev;		/* input device id for this network	*/
	int	foutdev;	/* output device id for this network	*/
	int	ftfuse;		/* timer fuse for send on this net	*/
	int	ftpid;		/* id of process to signal for timeout	*/
	int	fmachid;	/* id of this machine on this network	*/
	int	ftimproc;	/* timer process for this network	*/
	int	fiseq;		/* next input sequence to expect	*/
	int	foseq;		/* next output sequence number to write	*/
	int	fifails;	/* number of input failures		*/
	int	fofails;	/* number of output failures		*/
};

extern	struct	fglob	fdata[];
extern	int	frbpool;
