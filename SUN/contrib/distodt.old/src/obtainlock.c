
/* 
 * obtainlock.c - Routines for obtaining a lock on a remote machine
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue May 24 22:10:01 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <strings.h>
#include <pwd.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"
#include "../h/status.h"


extern char *getlogin();	/* system routine to get login id	*/
jmp_buf env;			/* used to return from read time-outs	*/
int errno;		        /* error number when system err		*/
struct	passwd *getpwuid();		/* get password file uid	*/
struct	passwd *pp;			/* password entry		*/

/*----------------------------------------------------------------------
 * reqlock - request lock on remote machine 
 *----------------------------------------------------------------------
 */
reqlock(fename, bename)
char *fename;				/* name of frontend with bename	*/
char *bename;				/* name of backend desired	*/
{
	int i;
	struct bm bm;
	struct lockreq *lreq;
	char *sb;
	char buf[MAXMACHINENAME];
	int sock;

	

					/* make lock request packet	*/
	bm.ver = CURVER;
	sprintf(bm.cmd, "%d", REQ_LOCK);
	lreq = (struct lockreq *) bm.detail;
/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
/*	sb = getlogin();*/  
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
 * obtainlock - Obtains and locks a machine, or verifies a current lock
 *		- if bename != "" then we try for bename
 *		- else we try for anything in class
 *---------------------------------------------------------------------------
 */
obtainlock(class, fename, bename, unspecifiedclass)
char *class;				/* class machine desired	*/
char *fename;				/* front end to use (out param)	*/
char *bename;				/* backend to use (in/out param)*/
char unspecifiedclass;			/* user did not specify class	*/
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
	int minutes;			/* number of minutes idle	*/
	char *ttyline;			/* tty line used		*/
	char *devname;			/* tty device line name		*/
	


	if (opt.verbose) {
		printf("Options are:\n");
		printf("\tUnspecified Class = %d\n", unspecifiedclass);
		printf("\tClass = '%s' if specified\n", class);
		printf("\tBackend requested = '%s'\n", bename);
	}

	checkbasedir();
	
	strcpy(filename, BASEDIR);
/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/
/*	strcat(filename, getlogin());*/
	pp = getpwuid(geteuid());
	if (pp == 0) {
		printf("Intruder alert.\n");
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

	if (opt.verbose) printf("now checking for local lock file %s\n",
				filename);
	if (stat(filename, &statbuf) == 0) { /* lock file exists */
		/* user has mach reserved - try it */
		if (opt.verbose) printf("Using local lock file\n");
		fs = fopen(filename, "r");
		fscanf(fs, "%s %s %s", femachine, bemachine, beclass);
		fclose(fs);
		if (opt.verbose) {
			printf("local lock file says:\n");
			printf("\tfrontend	= '%s'\n", femachine);
			printf("\tbackend	= '%s'\n", bemachine);
			printf("\tclass		= '%s'\n", beclass);
		}
		if ((strcmp(class, beclass) == 0) ||
		    (unspecifiedclass)) { 	/* it is the same class? */
			if ((strlen(bename) == 0) || /* is it the same mach? */
			    (strcmp(bename,bemachine) == 0)) {
				if (opt.verbose) printf("requesting lock\n");
				if (reqlock(femachine, bemachine) == SYSOK) {
					strcpy(fename, femachine);
					strcpy(bename, bemachine);
					return(SYSOK);
				}
			}
			else {		/* release old lock */
#ifdef AUTORELEASELOCK
				if (releaselock(beclass,bemachine,TRUE)==SYSOK)
				    printf("released lock on machine '%s'\n",bemachine);
#endif
				/* fall through and find a new machine */
				if (opt.verbose) {
					printf("Backend machine name ");
					printf("in the lockfile does\n");
					printf("not match the requested ");
					printf("backend machine name\n");
				}
			}
		}
		else {			/* release old lock */
#ifdef AUTORELEASELOCK
			if (releaselock(beclass, bemachine, TRUE) == SYSOK)
			    printf("released lock on machine '%s'\n",bemachine);
#endif
			/* fall through and find a new machine */
			if (opt.verbose) {
				printf("Class name in the lockfile does\n");
				printf("not match requested class name\n");
			}
		}
	}


	/* Now check if the user has one reserved, but we did not 	*/
	/* know about it cause there was no local lock file		*/
	if (opt.verbose) printf("Issuing status broadcast message\n");
	nummach = obtainstatus(bm);	/* get status from all machines	*/
	if (opt.verbose) {
		printf("Consult the BED servers to see if user has one reserved\n");
		printf("Looking for user '%s'\n", pp->pw_name /*getlogin()*/);
		if (strlen(bename)!=0)
		    printf("with a lock on backend machine %s\n", bename);
		else
		    printf("with a lock on any backend machine\n");
	}
	for (i = 0; i < nummach; i++) {
		stresp = (struct statresp *) bm[i]->detail;
		for (j = 0; j < atoi(stresp->festatus.numbends); j++) {
			bestat = &(stresp->bestatus[j]);
			if (opt.verbose) {
				printf("comparing to ");
				printf("class '%s': backend '%s': user '%s'\n",
				       bestat->class, bestat->bename,
				       bestat->user);
			}
			if (((strcmp(class, bestat->class) == 0) ||
			     ((unspecifiedclass) && (strlen(bename) != 0))) &&
			    ((strlen(bename) == 0) ||
			     (strcmp(bename,bestat->bename) == 0)) &&
			    (strcmp(pp->pw_name /*getlogin()*/, bestat->user) == 0)) {
				if (reqlock(stresp->festatus.fename,
					    bestat->bename) == SYSOK) {
					strcpy(fename,stresp->festatus.fename);
					strcpy(bename,bestat->bename);
					fs = fopen(filename, "w+");
					if (fs == NULL) {
						fprintf(stderr, "error opening file '%s'\n",filename);
						exit(1);
					}
					fprintf(fs, "%s %s %s\n",fename,bename,
						class);
					fclose(fs);
					return(SYSOK);
				}
				/* else keep looking */
			}
			else if (opt.verbose) {
				/* else keep looking - but print logic */
				if (!((strcmp(class, bestat->class) == 0) ||
				      ((unspecifiedclass) && (strlen(bename) != 0))))
				    printf("\tclass mismatch\n");
				else if (!((strlen(bename) == 0) ||
					   (strcmp(bename, bestat->bename) == 0)))
				    printf("\tbackend machine name mismatch\n");
				else if (!(strcmp(pp->pw_name,bestat->user) == 0))
				    printf("\tnot reserved by user %s\n",
					   pp->pw_name);
			}
		}
	}

	/* If we get to this point we need to find a free machine 	*/
	/* if we get here, just grap the first free machine that 	*/
	/* satisfy the class and backend name specified by the user	*/
	/* (either, or both of which he may not have specified)		*/
	if (opt.verbose) {
		if (strlen(bename) == 0)
		    printf("No machine reserved => find any free one\n");
		else {
			printf("No machine reserved => looking for machine %s\n",
			   bename);
			printf("with user='nobody' or user='%s'\n", pp->pw_name);
		}
	}
	for (i = 0; i < nummach; i++) {
		stresp = (struct statresp *) bm[i]->detail;
		for (j = 0; j < atoi(stresp->festatus.numbends); j++) {
			bestat = &(stresp->bestatus[j]);
			minutes = getidleminutes(bestat->idle);
			if (opt.verbose) {
				printf("comparing to ");
				printf("class '%s': backend '%s': user '%s'\n",
				       bestat->class, bestat->bename,
				       bestat->user);
				if (!((strcmp(class, bestat->class) == 0) ||
				      ((unspecifiedclass) && (strlen(bename) != 0))))
				    printf("\tclass mismatch\n");
				else if (!((strlen(bename) == 0) ||
					   (strcmp(bename, bestat->bename) == 0)))
				    printf("\tbackend machine name mismatch\n");
				else if (!((minutes >= RESERVETIME) || 
					   (strcmp(NOBODY, bestat->user) == 0)))
				    printf("\treserved by someone else\n");
			}
			if (((strcmp(class, bestat->class) == 0) ||
			     ((unspecifiedclass) && (strlen(bename) != 0))) &&
			    ((strlen(bename) == 0) ||
			     (strcmp(bename, bestat->bename) == 0)) &&
			    ((minutes >= RESERVETIME) || 
			    (strcmp(NOBODY, bestat->user) == 0))) {
				if (strcmp(NOBODY, bestat->user) != 0) {
					/* idle time exceeded */
					releaselock(bestat->class,
						    bestat->bename,
						    FALSE);
				}
				if (reqlock(stresp->festatus.fename,
					    bestat->bename) == SYSOK) {
					strcpy(fename,stresp->festatus.fename);
					strcpy(bename,bestat->bename);
					fs = fopen(filename, "w+");
					if (fs == NULL) {
						fprintf(stderr, "error opening file '%s'\n",filename);
						exit(1);
					}
					fprintf(fs, "%s %s %s\n", fename,bename,
						class);
					fclose(fs);
					return(SYSOK);
				}
				/* else keep looking */
				else {
					if (opt.verbose) {
						printf("\tlock requested and denied\n");
					}
				}
			}
		}
	}
	return(SYSERR);
}


/*---------------------------------------------------------------------------
 * getidleminutes - extract minutes from idletime string
 *---------------------------------------------------------------------------
 */
getidleminutes(idletime)
char *idletime;
{
	int hours, minutes, seconds;

	sscanf(idletime, "%d:%d:%d", &hours, &minutes, &seconds);
	if (hours > 0) minutes = minutes + (hours * 60);
	return(minutes);
}


/*---------------------------------------------------------------------------
 * checkbasedir - make sure that BASEDIR is there
 *---------------------------------------------------------------------------
 */
checkbasedir()
{
	char directory[256];

	/* make sure all the necessary directories and files are there */
	strcpy(directory, BASEDIR);
	directory[strlen(directory)-1] = '\0';	/* remove the backslash */
	if (access(directory, F_OK) != 0) {
		if (mkdir(directory, 0777) != 0) {
			fprintf("directory '%s' not found\n", directory);
			exit(1);
		}
	}
}
