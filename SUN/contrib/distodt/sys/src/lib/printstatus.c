/* 
 * printstatus.c - log the status
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Jun 11 10:41:10 1990
 *
 * Copyright (c) 1990 Patrick A. Muckelbauer
 */
#include <stdio.h>
#include <ctype.h>

#include "bed.h"
#include "bedata.h"

/*
 *---------------------------------------------------------------------------
 * printstatus - print the status message recieved
 *---------------------------------------------------------------------------
 */
void
printstatus( stats )
     struct statresp * stats;
{
	struct festatus * festat;	/* ptr to front end structure	*/
	struct bestatus * bestat;	/* ptr to backend structure	*/
	int i;				/* counter variable		*/
	
	festat = &( stats->festatus );
	printf( "\nFront end status for machine '%s'\n", festat->fename );
	printf( "\tNum users = %s,\tLoad average = %s\n",
	       festat->numusers, festat->loadav );
	for ( i=0; i < atoi( festat->numbends ); i++ ) {
		bestat = &( stats->bestatus[ i ] );
		printf( "\t%2d. %s\n", i + 1, bestat->bename );
		printf( "\t\tclass = %-20s  tty used  = %s\n",
		       bestat->class, bestat->ttyname );
		printf("\t\tuser  = %-20s  idle time = %s\n",
		       bestat->user, bestat->idle );
	}
}

/*
 *---------------------------------------------------------------------------
 * printbusers - print the backend users for given frontend
 *---------------------------------------------------------------------------
 */
void
printbusers( stats )
     struct statresp * stats;
{
	struct festatus * festat;	/* ptr to front end structure	*/
	struct bestatus * bestat;	/* ptr to backend structure	*/
	int i;				/* counter variable		*/
	
	festat = &( stats->festatus );
	for ( i = 0; i < atoi( festat->numbends ); i++ ) {
		bestat = &( stats->bestatus[ i ] );
		printf( "%-10s ", bestat->bename );
		printf( "user = %-10s ", bestat->user );
		printf( "idle time = %-10s ", bestat->idle );
		printf( "class = %s\n", bestat->class );
	}
}


/*---------------------------------------------------------------------------
 * printName -- print the name of the frontend (ignoring DOMAINNAME)
 *---------------------------------------------------------------------------
 */
static void
printName( sb )
     char * sb;
{
	char *tmp;
	
	tmp = sb;
	while ( *tmp != '\0' ) {
		for( ; *tmp != '\0' && *tmp != '.'; tmp++ ) {
			/* null statement */;
		}
		if ( *tmp != '\0' ) {
			tmp++;
		}
		if ( strcmp( tmp, DOMAINNAME ) ) {
			continue;
		}
		tmp--;
		*tmp = '\0';
		printf( "%-15s  ", sb );
		*tmp = '.';
		return;
	}
	printf( "%-15s  ", sb );
}

/*---------------------------------------------------------------------------
 * printfusers - print the front end machine's status
 *---------------------------------------------------------------------------
 */
void
printfusers( stats )
     struct statresp * stats;
{
	struct festatus * festat;	/* ptr to front end structure	*/
	int i;				/* counter variable		*/
	double load1, load2;
	
	festat = &( stats->festatus );
	printName( festat->fename );
	printf( "users = %-3s  ", festat->numusers );
	sscanf( festat->loadav, "%lf %lf", & load1, & load2 );
	printf( "Load = %5.2lf %5.2lf   ", load1, load2 ); 
	printf( "backends: " );
	for( i = 0; i < atoi( festat->numbends ) ; i++ ) {
		if ( i != 0 ) {
			printf(", ");
		}
		printf( "%s", stats->bestatus[ i ].bename );
	}
	printf( "\n" );
}


