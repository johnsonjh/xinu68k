/* tty.h */

#define IOCHERR		0200		/* bit set on when an error	*/
					/* occurred reading the char.	*/
#define	OBMINSP		40		/* min space in buffer before	*/
					/* processes awakened to write	*/
#define	EBUFLEN		20		/* size of echo queue		*/

/* size constants */

#ifndef	Ntty
#define	Ntty		1		/* number of serial tty lines	*/
#endif
#ifndef	IBUFLEN
#define	IBUFLEN		256		/* num.	chars in input queue	*/
#endif
#ifndef	OBUFLEN
#define	OBUFLEN		128		/* num.	chars in output	queue	*/
#endif

/* mode constants */

#define	IMRAW		'R'		/* raw mode => nothing done	*/
#define	IMCOOKED	'C'		/* cooked mode => line editing	*/
#define	IMCBREAK	'K'		/* honor echo, etc, no line edit*/
#define	OMRAW		'R'		/* raw mode => normal processing*/

struct	tty	{			/* tty line control block	*/
	int	ihead;			/* head of input queue		*/
	int	itail;			/* tail of input queue		*/
	char	ibuff[IBUFLEN];		/* input buffer for this line	*/
	int	isem;			/* input semaphore		*/
	int	ohead;			/* head of output queue		*/
	int	otail;			/* tail of output queue		*/
	char	obuff[OBUFLEN];		/* output buffer for this line	*/
	int	osem;			/* output semaphore		*/
	int	odsend;			/* sends delayed for space	*/
	int	ehead;			/* head of echo queue		*/
	int	etail;			/* tail of echo queue		*/
	char	ebuff[EBUFLEN];		/* echo queue			*/
	char	imode;			/* IMRAW, IMCBREAK, IMCOOKED	*/
	Bool	iecho;			/* is input echoed?		*/
	Bool	ieback;			/* do erasing backspace on echo?*/
	Bool	evis;			/* echo control chars as ^X ?	*/
	Bool	ecrlf;			/* echo CR-LF for newline?	*/
	Bool	icrlf;			/* map '\r' to '\n' on input?	*/
	Bool	ierase;			/* honor erase character?	*/
	char	ierasec;		/* erases character, usu. backsp*/
	Bool	ikill;			/* honor line kill character?	*/
	char	ikillc;			/* line kill character		*/
	int	icursor;		/* current cursor position	*/
	Bool	oflow;			/* honor ostop/ostart?		*/
	Bool	oheld;			/* output currently being held?	*/
	char	ostop;			/* character that stops output	*/
	char	ostart;			/* character that starts output	*/
	Bool	ocrlf;			/* echo CR/LF for LF ?		*/
	char	ifullc;			/* char to send when input full	*/
	struct	csr	*ioaddr;	/* device address of this unit	*/
	int	intrstate;		/* device interrupt state	*/
};
extern	struct	tty tty[];

#define	BACKSP	'\b'
#define	BELL	'\07'
#define	ATSIGN	'@'
#define	BLANK	' '
#define	NEWLINE	'\n'
#define	RETURN	'\r'
#define	STOPCH	'\023'			/* control-S stops output	*/
#define	STRTCH	'\021'			/* control-Q restarts output	*/
#define	UPARROW	'^'

/* ttycontrol function codes */

#define	TCSETBRK	1		/* turn on BREAK in transmitter	*/
#define	TCRSTBRK	2		/* turn off BREAK "       "	*/
#define	TCNEXTC		3		/* look ahead 1 character	*/
#define	TCMODER		4		/* set input mode to raw	*/
#define	TCMODEC		5		/* set input mode to cooked	*/
#define	TCMODEK		6		/* set input mode to cbreak	*/
#define	TCICHARS	8		/* return number of input chars	*/
#define	TCECHO		9		/* turn on echo			*/
#define	TCNOECHO	10		/* turn off echo		*/
#define	TFULLC		BELL		/* char to echo when buffer full*/
