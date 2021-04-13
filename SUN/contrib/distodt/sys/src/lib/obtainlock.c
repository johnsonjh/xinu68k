/* obtainlock.c - Routines for obtaining a lock on a remote machine
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
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "bed.h"
#include "status.h"

#define MAXBM	30

/*---------------------------------------------------------------------------
 * obtainlock - Obtains and locks a machine, or verifies a current lock
 *		- if bename != "" then we try for bename
 *		- else we try for anything in class
 *---------------------------------------------------------------------------
 */
int
obtainlock( fename, bename, class )
     char *fename;			/* front end to use (out param)	*/
     char *bename;			/* backend to use (in/out param)*/
     char *class;			/* class machine desired	*/
{
	char user[ 256 ];
	int i, j;			/* counter variable		*/
	struct bm *bm[ MAXBM ];		/* list of status messages	*/
	struct statresp *stresp;	/* temp status response ptr	*/
	int nummach;			/* number of machines		*/
	int ret;
	struct bestatus *bestat;	/*  backend status ptr		*/
	
	
	if ( checkbasedir() < 0 ) {
		return( -1 );
	}
	if ( getuser( user ) < 0 ) {
		fprintf( stderr, "error: intruder alert.\n" );
		return( -1 );
	}
	if ( strequ( bename, "-new" ) ) {
		strcpy( bename, "" );
		nummach = obtainstatus( bm, MAXBM );
		goto skip;
	}
	if ( getreservedMachine( fename, bename, class, user ) > 0 ) {
		return( 1 );
	}
	/* get a reserved machine */
	nummach = obtainstatus( bm, MAXBM );
	for ( i = 0; i < nummach; i++ ) {
		stresp = (struct statresp *) bm[ i ]->detail;
		strcpy( fename, stresp->festatus.fename );
		for ( j = 0; j < atoi( stresp->festatus.numbends ); j++) {
			bestat = &( stresp->bestatus[ j ] );
			ret = canlock(fename, bename, class, user, bestat);
			if ( ret > 0 ) {
				ret = getalock( fename, user, bestat );
			}
			if ( ret > 0 ) {
				strcpy( bename, bestat->bename );
				strcpy( class, bestat->class );
				return( 1 );
			}
		}
	}
	
	setdfltbemachine( bename );
skip:
	setdfltclass( class, bename );
	for ( i = 0; i < nummach; i++ ) {
		stresp = (struct statresp *) bm[ i ]->detail;
		strcpy( fename, stresp->festatus.fename );
		for ( j = 0; j < atoi( stresp->festatus.numbends ); j++) {
			bestat = &( stresp->bestatus[ j ] );
			ret = canlock(fename, bename, class, NULL, bestat);
			if ( ret > 0 ) {
				ret = getalock( fename, user, bestat );
			}
			if ( ret > 0 ) {
				strcpy( bename, bestat->bename );
				strcpy( class, bestat->class );
				return( 1 );
			}
		}
	}
	fprintf( stderr, "error: no machine found, lock request failed\n" );
	return( -1 );
}


/*---------------------------------------------------------------------------
 * canlock - can lock machine
 *---------------------------------------------------------------------------
 */
static int
canlock( fename, bename, class, user, bestat )
     char * fename, * bename, * class, * user;
     struct bestatus * bestat;
{
	int minutes;			/* number of minutes idle	*/
	
	minutes = getidleminutes( bestat->idle );
	if ( *bename != '\0'  && ! strequ( bename, bestat->bename ) ) {
		return( -1 );
	}
	if ( *class != '\0' && ! strequ( class, bestat->class ) ) {
		return( -1 );
	}
	
	if ( user != NULL ) { /* not the owner */
		if ( ! strequ( user, bestat->user ) ) {
			return( -1 );
		}
	}
	else {	/* lock in use */
		if ( ( minutes < RESERVETIME ) &&
		    ! strequ( NOBODY, bestat->user ) ) {
			return( -1 );
		}
	}
	return( 1 );
}

/*---------------------------------------------------------------------------
 * getalock - getalock
 *---------------------------------------------------------------------------
 */
static int
getalock( fename, user, bestat)
     char * fename, * user;
     struct bestatus * bestat;
{
	FILE * fs;
	char filename[ 256 ];		/* temp filename variable	*/
	int ret = 1;
	
	if ( ! strequ( NOBODY, bestat->user ) &&
	    ! strequ( user, bestat->user ) ) {
		
		/* idle time exceeded, and I do not own the backend */
		requnlock( fename, bestat->bename, user );
	}
	if ( strequ( user, bestat->user ) ||
	    reqlock( fename, bestat->bename, user ) >= 0 ) {
		
		getfilename( filename, user );
		fs = fopen( filename, "w+" );
		if ( fs == NULL ) {
			fprintf( stderr,
				"error: cannot open file '%s'\n", filename );
			return( -1 );
		}
		fprintf( fs, "%s %s %s\n", fename, bestat->bename,
			bestat->class );
		fclose( fs );
		return( 1 );
	}
	return( -1 );
}

/*---------------------------------------------------------------------------
 * checkbasedir - make sure that BASEDIR is there
 *---------------------------------------------------------------------------
 */
static int
checkbasedir()
{
	char directory[ 256 ];
	int len;
	
	/* make sure all the necessary directories and files are there */
	strcpy( directory, BASEDIR );
	len = strlen( BASEDIR );
	if ( directory[ len - 1 ] == '/' ) {	/* remove backslash */
		directory[ len - 1 ] = '\0';
	}
	if ( access( directory, F_OK ) != 0 ) {
		mkdir( directory, 0777 );
		if ( chmod( directory, 0777 ) ) {
			fprintf( stderr,
				"error: chmod failed for directory '%s'\n",
				directory );
			return( -1 );
		}
	}
	return( 1 );
}

/*---------------------------------------------------------------------------
 * getreservedMachine
 *---------------------------------------------------------------------------
 */
static int
getreservedMachine( fename, bename, class, user )
     char * fename, * bename, * class, * user;
{
	struct stat statbuf;		/* status of file struct	*/
	char femachine[ MAXMACHINENAME ];/* temp fename str variable	*/
	char bemachine[ MAXMACHINENAME ];/* temp bename str variable	*/
	char beclass[ MAXCLASS ];	/* temp class str variable	*/
	char filename[ MAXFILENAME ];	/* temp filename variable	*/
	FILE * fs;		
	
	getfilename( filename, user );
	if ( stat( filename, & statbuf ) == 0 ) {
		/* user has mach reserved - try it */
		fs = fopen( filename, "r" );
		fscanf( fs, "%s %s %s", femachine, bemachine, beclass );
		fclose( fs );
		if ( ( *class == '\0' || strequ( class, beclass ) ) &&
		    ( *bename == '\0' || strequ( bename, bemachine ) ) ) {
			if ( reqlock( femachine, bemachine, user ) > 0 ) {
				strcpy( fename, femachine );
				strcpy( bename, bemachine );
				strcpy( class, beclass );
				return( 1 );
			}
		}
#ifdef AUTORELEASELOCK
		if ( releaselock( beclass, bemachine, TRUE, bedservers )
		    >= 0 ) {
			fprintf( stdout,
				"released lock on machine '%s'\n", bemachine);
		}
#endif
	}
	return( -1 );
}

extern char *getenv();

/*---------------------------------------------------------------------------
 * setdfltbemachine
 *---------------------------------------------------------------------------
 */
setdfltbemachine( bemachine )
     char * bemachine;
{
	char * sb;
	
	if ( *bemachine != '\0' ) {
		return;
	}
	bemachine[ MAXMACHINENAME - 1 ] = '\0';
	if ( ( sb = getenv( ENVBEMACHINE ) ) != NULL ) {
		strncpy( bemachine, sb, MAXMACHINENAME - 1 );
	}
}

/*---------------------------------------------------------------------------
 * setdfltclass
 *---------------------------------------------------------------------------
 */
setdfltclass( class, bename )
     char * class, * bename;
{
	char * sb;
	
	if ( *class != '\0' || *bename != '\0' ) {
		return;
	}
	class[ MAXCLASS - 1 ] = '\0';
	if ( ( sb = getenv( ENVCLASS ) ) != NULL ) {
		strncpy( class, sb, MAXCLASS - 1 );
	}
	else {
		strncpy( class, DEFAULTCLASS, MAXCLASS - 1 );
	}
	if ( strequ( class, "503" ) ) {
		strncpy( class, DEFAULTCLASS, MAXCLASS - 1 );
	}
}


