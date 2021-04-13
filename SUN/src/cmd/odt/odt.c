#include "/usr/include/stdio.h"
#include "/usr/include/sgtty.h"
#include "/usr/include/signal.h"
#include "/usr/include/sys/types.h"
#include "/usr/include/sys/stat.h"
#include "/usr/include/sys/file.h"

#include <getdev.h>
#include <baud.h>

/************************************************************************/
/*									*/
/*  program:								*/
/*	odt -- talk to an LSI 11 from a VAX UNIX terminal		*/
/*									*/
/*  synopsis:								*/
/*	odt [-lx]							*/
/*									*/
/*  description:							*/
/*	  lsi attaches a user's terminal to a tty line that connects	*/
/*	to an LSI 11 so that keystrokes typed at the terminal are	*/
/*	sent to the LSI 11, and characters that the LSI 11 emits	*/
/*	are sent to the terminal display (without any processing).	*/
/*	The standard input, standard output, and standard error		*/
/*	files remain attached to the terminal.  File descriptor		*/
/*	lsifd is attached to the LSI 11 tty line.  A backslash /	*/
/*	causes lsi to change the keyboard mode to RAW for the next	*/
/*	character, making it possible to send special characters	*/
/*	like interrupt or kill characters.  Echo is turned off when	*/
/*	lsi begins.  Thus, any echoing of keystrokes is coming from	*/
/*	the LSI 11.							*/
/*									*/
/*	  Normally, odt looks for any available LSI 11.  The argument	*/
/*	-lx restricts the search to the LSI 11 with "number" x.		*/
/*									*/
/* Author/status:							*/
/*	Doug Comer, based on a program by Bob Brown			*/
/*	CS Dept., Purdue University					*/
/*									*/
/* Modification history:						*/
/*	12/86 -- Tom Stonecypher					*/
/*	deleted shell escape						*/
/*	modified -t argument to bypass reference to database		*/
/*									*/
/************************************************************************/

#define	STDOUT	1
#define	STDIN	0
#ifndef TRUE
#define TRUE	1
#define FALSE	0
typedef int	Bool;
#endif

int	devfd = -1;			/* line tty file descriptor	*/
int	flags;
int	quit();				/* procedure called to quit	*/
int	quit2();			/* safe interim quit procedure  */
struct	sgttyb	Lttymode;		/* line tty mode upon entry	*/
struct	sgttyb	lttymode;		/* line tty mode for gtty/stty	*/
struct	sgttyb	kttymode;		/* keyboard mode upon entry	*/
struct	sgttyb	cttymode;		/* changed keyboard mode	*/
int	baudrate;			/* rate line tty should be at	*/
int	timeout;			/* time keyboard can be idle	*/
int	pid;				/* process id of this process	*/
char	*class;				/* group of machines to use	*/
int	machnum;			/* number of requested machine	*/
char	*dev;
char	*lockfile;			/* name of lock file		*/
DevDesc *Dev;

/*
 *========================
 *   M a i n   E n t r y
 *========================
 */
main(argc, argv)
int argc;
char *argv[];
{
	int	tandem;
	int	i;
	int	fUsedb;

	if (ioctl(STDIN, TIOCGETP, &kttymode) < 0 ) {
		perror("Cannot set terminal modes");
		exit(1);
	}
	signal(SIGINT, quit2);		/* safe handler for parent and child
					   to execute until fork has completed
					   and both have reset the handler */

	class = NULL;
	machnum = GD_ANYDEV;
	flags = 0;
	baudrate = B9600;
	timeout = -1;
	tandem = TRUE;
	dev = "/dev/ttya";
	fUsedb = FALSE;
	for ( ; argc > 1; --argc) {
		if (**++argv != '-') {
			fprintf(stderr, "Argument %s unexpected.\n", *argv);
			exit(1);
		}
		switch (*++argv[0]) {
		case 'c':
			class = ++argv[0];
			break;
		case 'l':
			if (sscanf(++argv[0], "%d", &machnum) != 1) {
				fprintf(stderr, "Illegal device number: %s\n",
					argv[0]);
				exit(1);
			}
			break;
		case 'h':
			flags |= GD_HLDDEVS;
			break;
		case 't':
			dev = ++argv[0];
			fUsedb = FALSE;
			break;
		case 'B':
			if (*++argv[0] == '\0') {
				baudrate = NOBAUD;
				break;
			}
			for (i = 0; i < NBAUD; ++i)
				if (strcmp(baudlist[i].name, argv[0]) == 0)
					break;
			if (i < NBAUD)
				baudrate = baudlist[i].rate;
			else {
				fprintf(stderr, "Unknown baud rate: %s\n",
					argv[0]);
				exit(1);
			}
			break;
		case 'T':
			if (*++argv[0] == '\0')
				timeout = 0;
			else if (sscanf(argv[0], "%d", &timeout) != 1 ||
					timeout < 0) {
				fprintf(stderr,"Illegal timeout period: %s\n",
					argv[0]);
				exit(1);
			}
			break;
		case 'f':
			tandem = FALSE;
			break;
		default:
			fprintf(stderr, "Usage: odt [-cclass] [-lnumber] [-Bbaudrate] [-Ttimeout] [-f] [-h]\n");
			exit(1);
		}
	}
	if (fUsedb) {
		Dev = getdev(class, machnum, flags);
		if (Dev == NULL)
			exit(1);
		printf("Using %s\n", shortname(Dev->machine));
		devfd = Dev->fd;
		touch(lockfile = Dev->locknm);
		if (timeout < 0)
			timeout = atoi(get_limit(Dev->class));
		dev = get_name(Dev->machine);
	} else {
		if ( (devfd = open(dev, O_RDWR, 0)) < 0) {
			fprintf(stderr, "Cannot open %s\n", dev);
			exit(1);
		}
		printf("Using %s\n", dev);
	}
	if (ioctl(devfd, TIOCGETP, &Lttymode) < 0) {
		perror(dev);
		exit(1);
	} else {
		lttymode = Lttymode;
		lttymode.sg_flags |= RAW;
		lttymode.sg_flags &= ~ECHO;
		if (tandem)
			lttymode.sg_flags |= TANDEM;
		if (baudrate != NOBAUD) {
			Lttymode.sg_ospeed = Lttymode.sg_ispeed = baudrate;
			lttymode.sg_ospeed = lttymode.sg_ispeed = baudrate;
		}
		if (ioctl(devfd, TIOCSETP, &lttymode) < 0) {
			perror(dev);
			exit(1);
		}
	}

	/*
         * set up local terminal in cbreak mode
	 */
	cttymode = kttymode;
	cttymode.sg_flags |= CBREAK;
	cttymode.sg_flags &= ~ECHO;
	cttymode.sg_flags &= ~CRMOD;
	if (ioctl(STDIN, TIOCSETP, &cttymode) < 0) {
		perror("Cannot set terminal modes");
		exit(1);
	}
	/*
	 * start up the keyboard monitor and line monitor tasks
	 */
	if ((pid=fork()))
		keymon();
	else
		linemon();
	exit(0);
}


int	idle = 0;

/*
 *------------------------------------------------------------------
 *
 * keymon() - terminal keyboard monitor process
 *
 *------------------------------------------------------------------
 */
keymon()
{
	int	i;
        int	esc;
        char	c, buf[BUFSIZ];
	int	sigsusp(), sigcont(), sigalrm();

	/*
	 * catch interrupt, suspend and continue signals
	 */
	signal(SIGINT,  quit);
	signal(SIGTSTP, sigsusp);
	signal(SIGCONT, sigcont);
	signal(SIGALRM, sigalrm);
	alarm(60);

	/*
	 * loop forever over the following: process each keystroke
	 * on the keyboard.
	 */
        esc = 0;
	while( (i=read(STDIN,&c,1)) != 0) {
	    if (i == 1) {
		idle = 0;
		if ( c=='\\' && esc==0 ) {
			cttymode.sg_flags |= RAW;
			stty(STDIN,&cttymode);
                        esc = 1;
		} else {
                        if ( esc && (c&0177)=='\0' ) {
                                ioctl(devfd, TIOCSBRK, NULL);
#ifdef LSI				
                                sleep(2);
#else
                                usleep(10000);
#endif				
                                ioctl(devfd, TIOCCBRK, NULL);
                        } else
                                write(devfd, &c, 1);
                        if ( esc ) {
                                cttymode.sg_flags &= ~RAW;
                                stty(STDIN, &cttymode);
                        }
                        esc = 0;
		}
	    }
	}
        esc = 0;
	ioctl(STDIN, TIOCSETP, &kttymode);
	ioctl(devfd, TIOCSETP, &Lttymode);
	quit();
}

quit()
{
	kill(pid,9);
	ioctl(STDIN, TIOCSETP, &kttymode);
	ioctl(devfd, TIOCSETP, &Lttymode);
	printf("\n");

	exit(0);
}

quit2()					/* safe interim quit procedure */
{
	ioctl(STDIN, TIOCSETP, &kttymode);
	if (devfd >= 0)
		ioctl(devfd, TIOCSETP, &Lttymode);
	printf("\n");
	touch(lockfile);

	exit(0);
}

sigsusp()
{
	ioctl(STDIN, TIOCSETP, &kttymode);
        fflush(stdout);
	signal(SIGTSTP, SIG_DFL);
	kill(getpid(), SIGTSTP);
}

sigcont()
{
	signal(SIGCONT, sigcont);
	signal(SIGTSTP, sigsusp);
	ioctl(STDIN, TIOCSETP, &cttymode);
}


/*
 * sigalrm -- terminate if keyboard idle too long
 */
sigalrm()
{
	if (timeout > 0 && ++idle > timeout) {
	    fprintf(stderr,"\nKeyboard idle %d minute%s.  %s released.\n",
		timeout, (timeout == 1)? "" : "s", dev);
	    quit();
	}
	signal(SIGALRM, sigalrm);
	touch(lockfile);
	alarm(60);
}


/*
 *------------------------------------------------------------------
 *
 *  linemon  -- monitor LSI line and print characters that arrive
 *
 *------------------------------------------------------------------
 */
linemon()
{
	char	buf[512];
	int	cch;

	/*
	 * ignore interrupt signal -- keyboard process kills us
	 */
        signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP,SIG_DFL);
	signal(SIGCONT, SIG_DFL);

	for (;;) {
		if ((cch=read(devfd, buf, sizeof(buf))) > 0)
			write(STDOUT, buf, cch);
	}
}
