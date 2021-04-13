
/* 
 * download.c -  version 1 - copies a.out file to backend mach
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Mar  2 11:08:01 1988
 *
 * Copyright (c) 1988 Jim Griffioen
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
#include <setjmp.h>
#include <errno.h>
#include <pwd.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"
#include "../h/downloadd.h"

/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
extern char *getlogin();		/* get user id		*/

struct passwd *getpwuid();		/* get password entry	*/

jmp_buf env;		/* used to return from read time-outs	*/
int errno;	        /* error number when system err		*/
struct option opt;			/* command line options		*/



#ifndef TRUE
#define TRUE	1
#define FALSE	0
typedef int	Bool;
#endif

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
	char *sb;
	int remoteport;
	char class[16];			/* class of machine desired	*/
	char femachine[MAXMACHINENAME];	/* front end machine to use	*/
	char bemachine[MAXMACHINENAME];	/* back end machine to use	*/
	char filename[256];		/* file to download		*/
	struct genericreq *dnreq;	/* download request struct	*/
	char unspecifiedclass;		/* user specified class?	*/
	struct passwd *pp;		/* password entry		*/
	    

	if (csb > 5) {
		fprintf(stderr, "usage: download [-cCLASS] [-mBACKENDMACHINE] file\n");
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
		else if (i != (csb-1)) {
			fprintf(stderr,"unexpected arguement '%s'\n", rgsb[i]);
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

	if (opt.verbose) printf("found a machine to use\n");
	
	printf("using front end '%s':    Using backend '%s'\n",
	       femachine, bemachine);

	if (opt.verbose) printf("filling in a download request\n");
	
	bm.ver = CURVER;
	sprintf(bm.cmd, "%d", REQ_DNLD_CONNECT);
	dnreq = (struct genericreq *) bm.detail;
/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
/*	sb = getlogin();*/
	pp = getpwuid(geteuid());
	if (pp == 0) {
		printf("Intruder alert\n");
		exit(1);
	}
	strcpy(dnreq->uid, pp->pw_name /*sb*/);
	if (gethostname(dnreq->hostid, MAXMACHINENAME) != 0) {
		strcpy(dnreq->hostid, "dunno");
		fprintf(stderr, "error getting local host name\n");
	}
	strcpy(dnreq->bename, bemachine);
	
	sock = ConnectUdp (femachine, KNOWN_UDP_PORT);
	
	if (bmsend (sock, &bm, lbm, 0) <= 0) {
		fprintf(stderr,"error sending connection request\n");
		exit(1);
	}

	if (opt.verbose) printf("Just sent download request\n");
	
	len = bmrecv (sock, &bm, lbm, 0);
	if (opt.verbose) printf("recieve reply to download request\n");
	if (len<=0)  {
		fprintf(stderr, "receive error: trying to establish a connection\n");
		exit(1);
	}
	
	if (atoi(bm.cmd) != RESP_OK) {
		fprintf(stderr, "recv(): bad response (%s) from daemon: '%s'\n",bm.cmd,bm.detail);
		exit(1);
	}
	if (opt.verbose) {
		printf("we have permission from the bed to use\n");
		printf("the backend machine\n");
	}
	
	/* get the TCP port to use */
	remoteport = atoi(bm.detail);
	
	
	fdTcp = ConnectTcp(femachine, remoteport);
	
	if ((*(rgsb[csb-1]) == '-') || (csb == 1)) {
		strcpy(filename, "a.out");
	}
	else {
		strcpy(filename, rgsb[csb-1]);
	}

	printf("downloading file: %s\n", filename);
	download(fdTcp, filename);
	
	printf("download completeted\n");
	exit(0);
	
}



/*
 *---------------------------------------------------------------------------
 * download
 *    this routine sends the a.out file to the front end machine so that
 *    it can store it in /tftpboot/a.out for the backend machine to boot
 *    with
 *---------------------------------------------------------------------------
 */
download(fdTcp, aoutfile)
     int fdTcp;
     char *aoutfile;
{
	int fdaout;			/* file descriptor of a.out file       */
	char buff[BIGBUFSIZE];	/* big buffer to read into and out of  */
	char sblen[SMALLSTR];	/* string form of length               */
	struct stat statbuf;	/* file status structure               */
	long aoutsize;		/* size of a.out file                  */
	int  packetnum;		/* number of packets sent              */
	long  bytessent;	/* number of bytes sent			*/
	
	
	if ((fdaout=open(aoutfile, O_RDONLY, 0400)) < 0) {
		fprintf(stderr, "error opening file %s\n", aoutfile);
		exit(1);
	}
	
	
	/* Get name of the machine you are using */
	if (sockgetstr(fdTcp, buff, STIMEOUT) == NULL) {
		fprintf(stderr, "Error reading machine name during downloading");
		exit(1);
	}
	
	if (fstat(fdaout,&statbuf) != 0) {
		fprintf(stderr, "error finding status info about %s\n", aoutfile);
		exit(1);
	}
	aoutsize = (long) statbuf.st_size;
	sprintf(sblen,"%ld", (long) aoutsize);
	if (write(fdTcp, sblen, strlen(sblen)+1) != (strlen(sblen)+1)) {
		fprintf(stderr, "error writing file len to front end machine\n");
		exit(1);
	}
	bytessent = 0;
	packetnum = 1;
	while (aoutsize > sizeof(buff)) {
		if (read(fdaout, buff, sizeof(buff)) != sizeof(buff)) {
			fprintf(stderr, "error read from %s\n", aoutfile);
			exit(1);
		}
		if (write(fdTcp, buff, sizeof(buff)) != sizeof(buff)) {
			fprintf(stderr, "error writing to front end\n");
			exit(1);
		}
		packetnum++;
		bytessent += sizeof(buff);
		aoutsize -= sizeof(buff);
		printf("Sent %ldk bytes\r", bytessent/1000);
	}
	if (read(fdaout, buff, aoutsize) != aoutsize) {
		fprintf(stderr, "error read from %s\n", aoutfile);
		exit(1);
	}
	if (write(fdTcp, buff, aoutsize) != aoutsize) {
		fprintf(stderr, "error writing to front end\n");
		exit(1);
	}
	bytessent += aoutsize;
	printf("Sent %ldk bytes\n", bytessent/1000);
	close(fdTcp);
	close(fdaout);
}






/*
 *---------------------------------------------------------------------------
 * sockgetstr(sock, str, timeout) - get str from tcp socket-ret NULL if error
 *---------------------------------------------------------------------------
 */
char *sockgetstr(sock, str, timeout)
     int sock;				/* socket descriptor handle	*/
     char *str;
     int timeout;
{
	char *sb;
	
	
	settimer(timeout);
	if (setjmp(env) == EINTR) {
		canceltimer();
		return(NULL);
	}
	sb = str;
	if (recv(sock, sb, 1, 0) < 1) {
		canceltimer();
		return(NULL);
	}
	while (*sb++ != '\0') {
		if (recv(sock, sb, 1, 0) <1) {
			canceltimer();
			return(NULL);
		}
	}
	canceltimer();
	return(str);
}


