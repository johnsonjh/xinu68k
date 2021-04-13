
/* 
 * status.c - This program prints both frontend and backend status
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 11:20:43 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

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

	nummach = obtainstatus(bm);

	for (i = 0; i<nummach; i++) {
		prstatus((struct statresp *) bm[i]->detail);
	}
}


