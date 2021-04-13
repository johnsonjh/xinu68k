
/* 
 * odtunlock.c - This program unlock a backend machine locked by odt or download
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 26 20:07:53 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"

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
	char class[CLASSLEN];		/* class to release from	*/
	char bename[MAXMACHINENAME];	/* backend machine to release	*/
	char *sbclass;			/* ptr to class (maybe NULL)	*/
	char *sbbename;			/* ptr to bename (maybe NULL)	*/
	int i;				/* counter variable		*/
	char removelock;		/* bool - remove the local lock?*/

	if (csb > 5) {
		fprintf(stderr, "usage: odtunlock [-cCLASS] ");
		fprintf(stderr, "[-mBACKENDMACHINE] [-r] [-v]\n");
		exit(1);
	}
	strcpy(class, "");
	strcpy(bename, "");
	removelock = FALSE;
	opt.verbose = FALSE;

	/* parse the command line */
	for (i = 1; i < csb; i++) {
		if (strncmp(rgsb[i], "-m", 2) == 0) {
			strcpy(bename, (char *) ((rgsb[i])+2));
		}
		else if (strncmp(rgsb[i], "-c", 2) == 0) {
			strcpy(class, (char *) ((rgsb[i])+2));
		}
		else if (strncmp(rgsb[i], "-r", 2) == 0) {
			removelock = TRUE;
		}
		else if (strncmp(rgsb[i], "-v", 2) == 0) {
			opt.verbose = TRUE;
		}
		else {
			fprintf(stderr, "unexpected arguement '%s'\n",
				rgsb[i]);
			exit(1);
		}
	}


	if (releaselock(class, bename, removelock) == SYSOK)
	    printf("release machine '%s'\n", bename);
}



