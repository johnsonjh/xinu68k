
/* 
 * fusers.c - This program list the status of the front end machine
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
 *---------------------------------------------------------------------------
 */
main (csb, rgsb)
int csb;
char *rgsb[];
{
	int i;				/* counter variable		*/
	struct bm *bm[30];		/* list of status messages	*/
	int nummach;			/* number of machines		*/

	if (csb > 1) {
		fprintf(stderr, "usage: fuser\n");
		exit(1);
	}
	
	nummach = obtainstatus(bm);

	for (i = 0; i<nummach; i++) {
		prfusers((struct statresp *) bm[i]->detail);
	}
}


