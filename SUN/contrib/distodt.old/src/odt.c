
/* 
 * odt.c - Connect the user's terminal to a backend machine
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 22:02:03 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sgtty.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <pwd.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"

/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
extern char *getlogin();		/* get user id			*/

struct passwd *getpwuid();		/* get password entry 		*/

#define	STDOUT	1
#define	STDIN	0
#ifndef TRUE
#define TRUE	1
#define FALSE	0
typedef int	Bool;
#endif

#define BUFSIZE 100

int	flags;
struct	sgttyb	kttymode;		/* keyboard mode upon entry	*/
struct	sgttyb	cttymode;		/* changed keyboard mode	*/
int	baudrate;			/* rate line tty should be at	*/
int	timeout;			/* time keyboard can be idle	*/
int	pid;				/* process id of this process	*/
char	*class;				/* group of machines to use	*/
int	machnum;			/* number of requested machine	*/
char	*dev;
char	*lockfile;			/* name of lock file		*/
struct option opt;			/* command line options		*/



/*
 *---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main (csb, rgsb)
int csb;
char *rgsb[];
{
	int i;
	int sock;
	struct bm bm;
	int fdTcp;
	int len;
	int devfd;
	char *sb;
	int remoteport;
	int quit();
	char class[16];			/* class of machine desired	*/
	char femachine[MAXMACHINENAME];	/* front end machine to use	*/
	char bemachine[MAXMACHINENAME];	/* back end machine to use	*/
	struct genericreq *odtreq;	/* odt request structure	*/
	char unspecifiedclass;		/* user specified class?	*/
	struct passwd *pp;		/* password entry		*/

	if (csb > 4) {
		fprintf(stderr, "usage: odt [-cCLASS] [-mBACKENDMACHINE]\n");
		exit(1);
	}
	
	
	strcpy(class, DEFAULTCLASS);
	strcpy(femachine, "");
	strcpy(bemachine, "");
	unspecifiedclass = TRUE;
	opt.verbose = FALSE;

	for (i = 1; i < csb; i++) {
		if (strncmp(rgsb[i], "-c", 2) == 0) {
			strcpy(class, (char *) (rgsb[i]+2));
			unspecifiedclass = FALSE;
		}
		else if (strncmp(rgsb[i], "-m", 2) == 0) {
			strcpy(bemachine, (char *) (rgsb[i]+2));
		}
		else if (strncmp(rgsb[i], "-v", 2) == 0) {
			opt.verbose = TRUE;
		}
		else {
			fprintf(stderr, "unexpected argument '%s'\n", rgsb[i]);
			exit(1);
		}
	}
	
	if (obtainlock(class,femachine,bemachine,unspecifiedclass) != SYSOK) {
		if (strlen(bemachine) > 0)
		    fprintf(stderr, "machine %s is unavailable\n", bemachine);
		else if (strlen(class) > 0)
		    fprintf(stderr, "no machines in class %s available\n",
			    class);
		exit(1);
	}
	
	signal(SIGINT, quit);

	printf("using front end '%s':    Using backend '%s'\n",
	       femachine, bemachine);
	
	
	
	bm.ver = CURVER;
	sprintf(bm.cmd, "%d", REQ_ODT_CONNECT);
	odtreq = (struct genericreq *) bm.detail;
/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
/*	sb = getlogin();*/
	pp = getpwuid(geteuid());
	if (pp == 0) {
		printf("Intruder alert\n");
		exit(1);
	}
	strcpy(odtreq->uid, pp->pw_name /*sb*/);
	if (gethostname(odtreq->hostid, MAXMACHINENAME) != 0) {
		strcpy(odtreq->hostid, "dunno");
		fprintf(stderr, "error getting local host name\n");
	}
	strcpy(odtreq->bename, bemachine);
	
	sock = ConnectUdp (femachine, KNOWN_UDP_PORT);
	
	if (bmsend (sock, &bm, lbm, 0) <= 0)
	    SysError ("send");
	
	len = bmrecv (sock, &bm, lbm, 0);
	if (len<=0) 
	    Error("recv(): response code %d\n",len);
	
	if (atoi(bm.cmd) != RESP_OK) {
		Error("recv(): bad response (%s) from daemon: '%s'\n",bm.cmd,bm.detail);
	}
	
	/* get the TCP port to use */
	remoteport = atoi(bm.detail);
	if (opt.verbose) printf("using TCP port %d\n", remoteport);
	
	
	/*
	 * set up local terminal in cbreak mode
	 */
	if (opt.verbose) printf("getting terminal modes\n");
	if (ioctl(STDIN, TIOCGETP, &kttymode) < 0 ) {
		perror("Cannot set terminal modes");
		exit(1);
	}
	cttymode = kttymode;
	cttymode.sg_flags |= CBREAK;
	cttymode.sg_flags &= ~ECHO;
	cttymode.sg_flags &= ~CRMOD;
	if (opt.verbose) printf("setting terminal modes\n");
	if (ioctl(STDIN, TIOCSETP, &cttymode) < 0) {
		perror("Cannot set terminal modes");
		exit(1);
	}
	fdTcp = ConnectTcp(femachine, remoteport);
	if (opt.verbose) printf("ready to read and write characters\n");
	
	STDINxTCP(fdTcp, STDIN);
	
	quit();
	
}

/*---------------------------------------------------------------------------
 * quit - exit odt
 *---------------------------------------------------------------------------
 */
quit()
{
	ioctl(STDIN, TIOCSETP, &kttymode);
	printf("\n");
	exit(0);
}


/*---------------------------------------------------------------------------
 * STDINxTCP - Take standard input and send it to BED process on frontend 
 *---------------------------------------------------------------------------
 */
STDINxTCP(fdTCP, fdStdin)
     int fdTCP, fdStdin;
{
	int bmaskRead;
	int bmaskResult;
	int zero = 0;
	int len;
	int esc;
	int idle;
	char buf[BUFSIZE];
	int i;
	
	bmaskRead = BITMASK(fdStdin) | BITMASK(fdTCP);	
	esc = 0;
	idle = 0;
	for (;;) {
		bmaskResult = bmaskRead;
		select(32, &bmaskResult, &zero, &zero, 0);
		
		if ((bmaskResult & BITMASK(fdTCP)) != 0) {
			len = read(fdTCP,buf,BUFSIZE);
			if (len <= 0) {
				if (len != 0)
				    fprintf(stderr,"Coupling: bad return from fdTCP read: %d\n",len);
				return;
			}
			if ( (len = write(fdStdin,buf,len)) <= 0)
			    SysError("Coupling: Bad return from fdStdin write");
		}
		if ((bmaskResult & BITMASK(fdStdin)) != 0) {
			len = read(fdStdin,buf,BUFSIZE);
			if (len <= 0) {
				if (len != 0)
				    fprintf(stderr,"Coupling: return from fdStdin read: %d\n",len);
				return;
			}
			idle = 0;
			for (i=0; i<len; ++i) {
				if ( esc ) {
					cttymode.sg_flags &= ~RAW;
					stty(fdStdin, &cttymode);
					esc = 0;
				}
				else if ( buf[i]=='\\' && esc==0 ) {
					cttymode.sg_flags |= RAW;
					stty(fdStdin,&cttymode);
					esc = 1;
				}
				
				if ( (write(fdTCP,buf+i,1)) <= 0) 
				    SysError("Coupling: bad return from fdTCP write");
			}
		}
	}
}



