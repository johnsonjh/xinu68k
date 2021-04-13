
/* 
 * parsestatus.c - These routine parse the status structure and return desired info
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun  1 11:06:22 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

#include <stdio.h>

#include "../h/bed.h"

/*----------------------------------------------------------------------
 * getbestatus - returns a pointer to the status structure
 *		for backend bename
 *----------------------------------------------------------------------
 */
struct bestatus *getbestatus(allbestats, bename)
struct statresp *allbestats;		/* config status of all bends	*/
char *bename;				/* backend name to look for	*/
{
	struct bestatus *bestat;	/* be status pointer		*/
	int i;				/* counter variable		*/
	int numbackends;		/* number of backends		*/

	numbackends = atoi(allbestats->festatus.numbends);
	for (i=0; i<numbackends; i++) {
		bestat = &(allbestats->bestatus[i]);
		if (strcmp(bestat->bename, bename) == 0) {
			return(bestat);	/* we found it			*/
		}
	}
	return(NULL);			/* did not find it		*/
}

