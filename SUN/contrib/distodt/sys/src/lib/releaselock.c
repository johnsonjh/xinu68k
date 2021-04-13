/* releaselock.c - This file contains routines for releasing a lock
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 26 17:23:44 1988
 */


#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <setjmp.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "bed.h"
#include "bmutils.h"

#define MAXBM	30


/*---------------------------------------------------------------------------
 * releaselock - release lock on a backend machine
 *			- release specified backend machine is bename!=""
 *			- else release last machine reservered by user in
 *			  class is class is not ""
 *			- else release last machine used by user
 *---------------------------------------------------------------------------
 */
int
releaselock( class, bename, remove )
     char * class, * bename, remove;
{
	FILE * fs;			/* lock file ptr		*/
	struct stat statbuf;		/* status of file struct	*/
	char filename[ MAXFILENAME ];	/* temp filename variable	*/
	char femachine[ MAXMACHINENAME ];/* temp fename str variable	*/
	char bemachine[ MAXMACHINENAME ];/* temp bename str variable	*/
	char beclass[ MAXCLASS ];	/* temp class str variable	*/
	char user[ 256 ];
	int i, j;			/* counter variable		*/
	struct bm * bm[ MAXBM ];	/* list of status messages	*/
	struct statresp * stresp;	/* temp status response ptr	*/
	struct bestatus * bestat;	/* temp backend status ptr	*/
	int nummach;			/* number of machines		*/
	char * ttyline;			/* tty line used		*/
	char * devname;			/* tty device line name		*/
	
	
	
	if ( getuser( user ) < 0 ) {
		fprintf( stderr, "error: intruder alert\n" );
		return( -1 );
	}
	getfilename( filename, user );
	if ( stat( filename, & statbuf ) == 0 ) {	/* has mach reserved */
		fs = fopen( filename, "r" );
		fscanf( fs, "%s %s %s", femachine, bemachine, beclass );
		fclose( fs );
	}
	else {
		fs = NULL;
	}
	if ( strlen( bename ) == 0 ) {	/* release machine reserved */
		if ( fs == NULL ) {
			fprintf( stderr, "error: no machine reserved\n" );
			return( -1 );
		}
		if ( ! strlen( class )  || strequ( class, beclass ) ) {
			if ( requnlock( femachine, bemachine, user ) >= 0 ) {
				if ( remove && unlink( filename ) ) {
					fprintf( stderr,
					  "error: cannot remove lock file\n" );
				}
				strcpy( bename, bemachine );
				return( 1 );
			}
			fprintf( stderr, "error: requnlock failed\n" );
			return( -1 );
		}
		fprintf( stderr, "error: reserved backend not of class %s\n",
			class );
		return( -1 );
	}
	
	/* if we get to this point we need to find a particular machine	*/
	nummach = obtainstatus( bm, MAXBM );
	for ( i = 0; i < nummach; i++ ) {
		stresp = (struct statresp *) bm[ i ]->detail;
		for ( j = 0; j < atoi( stresp->festatus.numbends ); j++ ) {
			bestat = &( stresp->bestatus[ j ] );
			if ( strcmp( bename, bestat->bename ) ) {
				continue;
			}
			checkidletime( bestat, user );
			if ( requnlock( stresp->festatus.fename, bestat->bename, user ) >= 0 ) {
				if ( remove && fs != NULL && strequ( bemachine, bename ) ) {
					if ( unlink( filename ) ) {
						fprintf( stderr, "error: cannot remove lock file\n" );
					}
				}
				return( 1 );
			}
			fprintf( stderr, "error: requnlock failed\n" );
			return( -1 );
		}
	}
	fprintf( stderr, "error: reserved backend not found\n" );
	return( -1 );
}


/*
 * checkidletime -- print a warning if idle time is less then threshhold
 */
static int
checkidletime( bestat, user )
     struct bestatus * bestat;
     char * user;
{
	int min;
	
	if ( ! strcmp( NOBODY, bestat->user ) ||
	    ! strcmp( user, bestat->user ) ) {
		return( 0 );
	}
	min = getidleminutes( bestat->idle );
	if ( min < RESERVETIME ) {
		fprintf( stderr,
		    "warning: releasing backend '%s' with only %s idle time\n"
			, bestat->bename, bestat->idle );
	}
}

