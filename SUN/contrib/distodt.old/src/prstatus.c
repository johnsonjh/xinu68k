
/* 
 * prstatus.c - Routines for printing status of front and backends
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 19 13:40:17 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

#include <stdio.h>

#include "../h/bed.h"


/*
 *---------------------------------------------------------------------------
 * prstatus - print the status message recieved
 *---------------------------------------------------------------------------
 */
prstatus(stats)
struct statresp *stats;
{
    struct festatus *festat;		/* ptr to front end structure	*/
    struct bestatus *bestat;		/* ptr to backend structure	*/
    int i;				/* counter variable		*/

    festat = &(stats->festatus);
    printf("\nFront end status for machine '%s'\n", festat->fename);
    printf("\tNum users = %s,\tLoad average = %s,\tNum backends = %s\n",
	   festat->numusers, festat->loadav, festat->numbends);
    printf("\tBack end machines connected to front end machine '%s'\n",
	   festat->fename);
    for (i=0; i<atoi(festat->numbends); i++) {
	    bestat = &(stats->bestatus[i]);
	    printf("\t\t%2d. %s\n", i+1, bestat->bename);
	    printf("\t\t\tclass = %-20s  tty used  = %s\n",
		   bestat->class, bestat->ttyname);
	    printf("\t\t\tuser  = %-20s  idle time = %s\n",
		   bestat->user, bestat->idle);
    }
}



/*
 *---------------------------------------------------------------------------
 * prbusers - print the backend users for given frontend
 *---------------------------------------------------------------------------
 */
prbusers(stats)
struct statresp *stats;
{
    struct festatus *festat;		/* ptr to front end structure	*/
    struct bestatus *bestat;		/* ptr to backend structure	*/
    int i;				/* counter variable		*/

    festat = &(stats->festatus);
    for (i=0; i<atoi(festat->numbends); i++) {
	    bestat = &(stats->bestatus[i]);
	    printf("%-20s ", bestat->bename);
	    printf("idle time = %-10s ", bestat->idle);
	    printf("class = %-8s ", bestat->class);
	    printf("reserved by %s\n", bestat->user);
    }
}



/*
 *---------------------------------------------------------------------------
 * prfusers - print the front end machine's status
 *---------------------------------------------------------------------------
 */
prfusers(stats)
struct statresp *stats;
{
    struct festatus *festat;		/* ptr to front end structure	*/
    int i;				/* counter variable		*/

    festat = &(stats->festatus);
    printf("%-40s \n \t# of users = %-3s Load = %-24s # of backends = %s\n",
	   festat->fename, festat->numusers, festat->loadav, festat->numbends);
}

