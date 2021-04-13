#include "pm.h"
#include "../../h/bed.h"

/*
 *	pm - a post mortem analyzer of coreX files
 *	that come from the Vax Xinu operating system.
 *
 *	Chris Kent - October 1981, lots of changes
 *	by D. Comer & Droms 82-86
 *
 *	Adapted for the Sun3 by Shawn Ostermann 10/87
 *          Currently only supports serial upload.
 *          Xinu now saves all registers into currpid proc table on exit
 */

/*
 *	Arguments/options:
 *
 *		pm [-m] [-t] [-r] [-p] [-s] [ objfile ]
 *
 *	where objfile is the text that was downloaded, and corefile
 *	is the result of upload. Defaults are a.out and coreX.
 *
 */

#define		INTVECTOR	0x400	/* # bytes of interrupt vectors	*/

struct option opt;		/* bed options - not used much		*/
char		*corefile;
FILE		*corefd;	/* the core image			 */
char		*txtfile;
FILE		*txtfd;		/* the system				 */
char		*text;		/* pointer to system image		 */
char		*core;		/* pointer to core image		 */
struct nlist	*symtab;	/* symbol table space 			 */
struct nlist	*esymtab;	/* end of same				 */
struct nlist	*ssymtab;	/* external symbol table space		 */
struct nlist	*essymtab;	/* end of same				 */
char		*strtab;	/* table of names of symbols		 */
/*struct coreX	c_header;	/* registers at time of dump		 */
struct exec	a_out;		/* a.out header				 */
int		errflg;		/* global error indicator		 */
int		allopts, copt, mopt, popt, sopt, topt, ropt;	/* option indicators	 */
int		verbose;	/* print verbose description or not	 */
char   *vers;			/* pointer to version string		 */
long   clktim;			/* pointer to time variable		 */
struct pentry	*proctb;	/* pointer to image of proc table	 */
struct qent	*Q;		/*    "     "   "   "  q		 */
struct sentry	*semtab;	/*    "     "   "   "  semaphore table	 */
struct tty	*tptr;		/*    "     "   "   "  tty structures	 */
int    currpid;			/* current process id number		 */
main(argc, argv)
int	argc;
char	*argv[];
{

	INT		base;

	register	i, j;

	setup(argc, argv);	/* open files, etc. */

	/* 
	 * compare the core image text segment and the 
	 * original text segment from the system
	 */
			
	printf("Postmortum for Sun3 Xinu\n");
	
	if(allopts || mopt)
		checktext(text,core);

	if(allopts || popt) {
	    checkcurrent();
	    procdump(currpid);
	}

	if(allopts || sopt)
		semdump();

	if(allopts || topt)
		ttydump();
}

/*
 *	dump the proc table
 */

procdump(currpid)
{

	char		*pidname;
	register int	i;


	for(i = 0; i < NPROC; i++){
		if(i == currpid)
			continue;
		pidname = proctb[i].pname;
		printf("\nprocess # %2d ( %s ):\tstate=", i, pidname);
		switch(proctb[i].pstate){
		case PRFREE:
			printf("free\n");
			continue;
		case PRREADY:
			printf("ready\n");
			break;
		case PRCURR:	/* shouldn't happen! */
			printf("CURRENTLY RUNNING???\n");
			break;
		case PRSUSP:
			printf("suspended\n");
			break;
		case PRWAIT:
			printf("waiting on semaphore %d\n", proctb[i].psem);
			break;
		case PRSLEEP:
			printf("sleeping\n");
			break;
		case PRRECV:
			printf("receiving\n");
			break;
		case PRTRECV:
			printf("timing recv.\n");
			break;
		default:
			printf("garbage state 0x%x\n", proctb[i].pstate&0xff);
		}
		if (proctb[i].pstate == PRFREE)
			continue;
		printf("\tat priority %d ", proctb[i].pprio);
		printf("stack is %d bytes long\n", 
			(proctb[i].pbase - proctb[i].pregs[SSP]));
		printf("\tstack range: 0x%0x to 0x%0x\n",
			proctb[i].plimit,
			proctb[i].pbase);
		printregs(proctb[i].pregs);
		if (allopts || ropt) {
		    printf("     traceback:\n");
		    printtrace(proctb[i].pregs, proctb[i].pbase, i);
		    printf("\n");
		}
		printf("\n");
	}
}

/*
 *	dump the semaphore table and queues
 */

semdump()
{
	short		qptr;
	register int	i, j;
	
	printf("\nSemaphores:\n\n");
	for(i = 0; i < NSEM; i++){
		if (semtab[i].sstate == SFREE)
			continue;
		printf("semaphore # %d : ", i);
		if (semtab[i].sstate != SUSED) {
			printf("garbage state 0x%x\n",
				semtab[i].sstate&0xff);
			continue;
		}
		printf("count is %d\n", semtab[i].semcnt);
		if(Q[semtab[i].sqhead].qnext < NPROC) {
			printf("\tqueued proc(s) are: ");
			qptr = semtab[i].sqhead; 
			j = 0;
			do {
				printf("%d ", Q[qptr].qnext);
				qptr = Q[qptr].qnext;
				j++;
			} while(Q[qptr].qnext < NPROC && j<=NPROC );
			if (Q[qptr].qnext != semtab[i].sqtail)
				printf("...QUEUE MALFORMED");
			printf("\n\t%d proc%s actually on queue\n", j,
				j!=1?"s":"");
		}
	}
}

/*
 * routines to print contents of Xinu IO control block
 */
char	*unctrl[]	= {	/* unctrl codes for ttys		*/
	"^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G", "^H", "^I", "^J", "^K",
	"^L", "^M", "^N", "^O", "^P", "^Q", "^R", "^S", "^T", "^U", "^V", "^W",
	"^X", "^Y", "^Z", "^[", "^\\", "^]", "^~", "^_",
	" ", "!", "\"", "#", "$",  "%", "&", "'", "(", ")", "*", "+", ",", "-",
	".", "/", "0",  "1", "2",  "3", "4", "5", "6", "7", "8", "9", ":", ";",
	"<", "=", ">",  "?", "@",  "A", "B", "C", "D", "E", "F", "G", "H", "I",
	"J", "K", "L",  "M", "N",  "O", "P", "Q", "R", "S", "T", "U", "V", "W",
	"X", "Y", "Z",  "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e",
	"f", "g", "h",  "i", "j",  "k", "l", "m", "n", "o", "p", "q", "r", "s",
	"t", "u", "v",  "w", "x",  "y", "z", "{", "|", "}", "~", "^?"
};
ttydump()
{
	register int i;

	for (i=0 ; i<Ntty ; i++)
		tty1dmp(i);
}
tty1dmp(ttynum)
int ttynum;
{
	int len;
	int i;
	struct tty *tp;

	tp = (struct tty *) (
		(unsigned)(tptr) +
		(unsigned)(ttynum* (sizeof(struct tty) - 2) ) );
		/* adjusted for VAX sizes */
	printf("\nTty device %d CSR at 0x%x\n",
		ttynum, (*( (short *)(( (char *)(&tp->ifullc))+1))) &0xffff);
		/* actually tp->ioaddr, but Vax aligns differently */
        printf("INPUT -- tail at %d head at %d sem # %d value %d\n",
		tp->itail,tp->ihead,tp->isem,i = semtab[tp->isem].semcnt);
	if(i < 0)
		i = 0;
	else if(i > IBUFLEN)
		i = IBUFLEN;
	ioqdump(tp->itail,i,IBUFLEN,tp->ibuff);
        printf("OUTPUT -- tail at %d head at %d sem # %d value %d\n",
		tp->otail,tp->ohead,tp->osem,i = semtab[tp->osem].semcnt);
	i = OBUFLEN - i;
	if(i < 0)
		i = 0;
	else if(i > OBUFLEN)
		i = OBUFLEN;
	ioqdump(tp->otail,i,OBUFLEN,tp->obuff);
}

ioqdump(start,len,maxlen,buff)
short start,len,maxlen;
char *buff;
{
	register int i;

        for(i=start; len>0; len--){
                if(buff[i]&0200)printf("M-");
                printf("%s", unctrl[buff[i]&0177]);
		if(++i >= maxlen)
			i = 0;
        }
	printf("\n");
}


checktext(text,core)
     long *text, *core;
{
    long *TextPtr;
    long *CorPtr;
    int  FirstDiff;
	FirstDiff = 0;
	TextPtr = text;
	CorPtr = core;
	while((char *)CorPtr - (char *)core < a_out.a_text) {
		if(*CorPtr != *TextPtr) {
			register s, c;

			if(!FirstDiff){
				printf("Text has been changed:\n");
				printf("\t\tProgram\t\tCore\n");
				FirstDiff = 1;
			}
			s = (int) *TextPtr;
			c = (int) *CorPtr;
			printf("0x%08x:\t0x%02x\t0x%02x\n",
			  (SUN3START + (char *)CorPtr - (char *)core), s, c);
			/* would be nice to do opcode lookup here */
		}
		TextPtr++;
		CorPtr++;
	}
}



checkcurrent()
{
    char		*pidname;
	
    if (currpid<0 || currpid>NPROC) {
	printf("Invalid current process id (currpid==%d)\n",
	       currpid);
    } else {
	pidname = proctb[currpid].pname;
	printf("process # %d ( %s ) was running\n", currpid, pidname);
	printf("\tat priority %d ", proctb[currpid].pprio);
	if (	((unsigned)proctb[currpid].pregs[SSP]) <
	    ((unsigned)proctb[currpid].plimit) )
	    printf("Stack has OVERFLOWED actual=0x%x, limit=0x%x\n",
		   ((unsigned)proctb[currpid].pregs[SSP]),
		   ((unsigned)proctb[currpid].plimit) );
	else
	    printf("stack is currently %d bytes long\n", 
		   (proctb[currpid].pbase-proctb[currpid].pregs[SSP]));
	printf("\tstack range: 0x%08x to 0x%08x\n",
	       proctb[currpid].plimit,
	       proctb[currpid].pbase);
        printregs(proctb[currpid].pregs);
	if (allopts || ropt) {
	    printf("     traceback:\n");
	    printtrace(proctb[currpid].pregs, proctb[currpid].pbase, currpid);
	    printf("\n");
	}
    }
}
