
/* 
 * releaselock.c - This file contains routines for releasing a lock
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 26 17:23:44 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <setjmp.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <strings.h>
#include <pwd.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"


/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
extern char *getlogin();		/* sys routine to get login id	*/

struct passwd *getpwuid();		/* get password entry		*/

/*----------------------------------------------------------------------
 * requnlock - request unlock on remote machine 
 *----------------------------------------------------------------------
 */
requnlock(fename, bename)
char *fename;				/* name of frontend with bename	*/
char *bename;				/* name of backend desired	*/
{
	int i;
	struct bm bm;
	struct lockreq *lreq;
	char *sb;
	char buf[MAXMACHINENAME];
	int sock;
	struct passwd *pp;	

	

					/* make lock request packet	*/
	bm.ver = CURVER;
	sprintf(bm.cmd, "%d", REQ_UNLOCK);
	lreq = (struct lockreq *) bm.detail;
/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
/*	sb = getlogin();*/
	pp = getpwuid(geteuid());
	if (pp == 0) {
		printf("Intruder alert\n");
		exit(1);
	}
	strcpy(lreq->uid, pp->pw_name /*sb*/);
	if (gethostname(buf, MAXMACHINENAME) != 0) {
		strcpy(buf, "dunno");
		fprintf(stderr, "error getting local host name\n");
	}
	strcpy(lreq->hostid, buf);
	strcpy(lreq->bename, bename);

	
	sock = ConnectUdp (fename, KNOWN_UDP_PORT);

	if (bmsend (sock, &bm, lbm, 0) <= 0)
	    SysError ("send");

	bmrecv (sock, &bm, lbm, 0);
	if (atoi(bm.cmd) != RESP_OK)
	    return(SYSERR);
	else
	    return(SYSOK);
}


/*---------------------------------------------------------------------------
 * releaselock - release lock on a backend machine
 *			- release specified backend machine is bename!=""
 *			- else release last machine reservered by user in
 *			  class is class is not ""
 *			- else release last machine used by user
 *---------------------------------------------------------------------------
 */
releaselock(class, bename, remove)
char *class;				/* class to release machine from*/
char *bename;				/* back end to use (out param)	*/
char remove;				/* boolean - remove local lock?	*/
{
	FILE *fs;			/* lock file ptr		*/
	char filename[256];		/* temp filename variable	*/
	struct stat statbuf;		/* status of file struct	*/
	char femachine[MAXMACHINENAME];	/* temp fename str variable	*/
	char bemachine[MAXMACHINENAME];	/* temp bename str variable	*/
	char beclass[CLASSLEN];		/* temp class str variable	*/
	int i, j;			/* counter variable		*/
	struct bm *bm[30];		/* list of status messages	*/
	struct statresp *stresp;	/* temp status response ptr	*/
	struct bestatus *bestat;	/* temp backend status ptr	*/
	int nummach;			/* number of machines		*/
	struct passwd *pp;		/* password entry		*/
	char *ttyline;			/* tty line used		*/
	char *devname;			/* tty device line name		*/

	

	strcpy(filename, BASEDIR);
/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
/*	strcat(filename, getlogin());*/
	pp = getpwuid(geteuid());
	if (pp == 0) {
		printf("Intruder alert\n");
		exit(1);
	}
	strcat(filename, pp->pw_name);
	strcat(filename, ".");
	/* lock files are associated with tty names - this allows the	*/
	/* user to be logged in twice or have multiple windows with	*/
	/* an odt to a different machine running in each window		*/
	if ((devname = (char *)ttyname(0)) != NULL) { /* get stdin name */
		ttyline = (char *)strrchr(devname, '/');
		ttyline++;
		strcat(filename, ttyline);
	}
	else {				/* no associated tty - hmmm	*/
		strcat(filename, "notty");
	}

		
	if (strlen(bename) == 0) {	/* release machine reserved */
		if (stat(filename, &statbuf) == 0) { /* has mach reserved */
			fs = fopen(filename, "r");
			fscanf(fs, "%s %s %s", femachine, bemachine, beclass);
			fclose(fs);
			if ((strlen(class) == 0) ||
			    (strcmp(class, beclass) == 0)) {
				if (requnlock(femachine, bemachine) == SYSOK) {
					strcpy(bename, bemachine);
					if (remove) {
						if (unlink(filename) != 0) {
							return(SYSERR);
						}
					}
					return(SYSOK);
				}
				else {
/*					fprintf(stderr, "error unlocking %s\n",*/
/*						bemachine);*/
					if (remove) {
						if (unlink(filename) != 0) {
							return(SYSERR);
						}
					}
					return(SYSERR);
				}
			}
			else {
				fprintf(stderr, "dunno what to do here yet\n");
				return(SYSERR);
			}
		}
		else {
			fprintf(stderr, "no machine reserver\n");
			return(SYSERR);
		}
	}
	
	/* if we get to this point we need to find a particular machine	*/
	nummach = obtainstatus(bm);	/* get status from all machines	*/
	for (i = 0; i < nummach; i++) {
		stresp = (struct statresp *) bm[i]->detail;
		for (j = 0; j < atoi(stresp->festatus.numbends); j++) {
			bestat = &(stresp->bestatus[j]);
			if (strcmp(bename, bestat->bename) == 0) {
				if (requnlock(stresp->festatus.fename,
					    bestat->bename) == SYSOK) {
  					if (remove) {
						fs = fopen(filename, "r");
						fscanf(fs, "%s %s %s", femachine, bemachine, beclass);
						fclose(fs);
						if (strcmp(bemachine,bename) == 0) {
							if (unlink(filename) != 0) {
								return(SYSERR);
							}
						}
					}
					return(SYSOK);
				}
				else {
					fprintf(stderr, "error releasing %s\n", bename);
					return(SYSERR);
				}
			}
		}
	}
	return(SYSERR);
}


