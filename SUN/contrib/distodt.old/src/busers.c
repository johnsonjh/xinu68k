
/* 
 * busers.c - This program list the backend user - similar to the old lusers
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 26 23:07:00 1988
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
 &---------------------------------------------------------------------------
 */
main (csb, rgsb)
int csb;
char *rgsb[];
{
	int i;				/* counter variable		*/
	struct bm *bm[30];		/* list of status messages	*/
	int nummach;			/* number of machines		*/

	if (csb > 2) {
		fprintf(stderr, "usage: buser [-v]\n");
		exit(1);
	}

	opt.verbose = FALSE;

	for (i = 1; i < csb; i++) {
		if (strncmp(rgsb[i], "-v", 2) == 0) {
			opt.verbose = TRUE;
		}
		else {
			fprintf(stderr, "unexpected argument '%s'\n", rgsb[i]);
			exit(1);
		}
	}
	
	nummach = obtainstatus(bm);

	for (i = 0; i<nummach; i++) {
		prbusers((struct statresp *) bm[i]->detail);
	}
}


