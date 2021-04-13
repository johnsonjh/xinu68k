/* lock.c - lock routines
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Oct 26 23:30:52 1989
 */


#include <stdio.h>
#include <signal.h>

#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/time.h>

#include "bed.h"
#include "bedata.h"

extern struct option opt;

/*---------------------------------------------------------------------------
 * getlock(beconfig, user, host)
 * 	struct odtdata *beconfig; char *user, *host;
 *
 * get the lock on the backend machine for user@host, if possible.
 * if a) no one holds the lock, or
 *    b) user@host holds the lock,
 * SYSOK is returned, else SYSERR (i.e. someone else holds the lock,
 * or the user holds it at a different host).
 *---------------------------------------------------------------------------
 */
int
getlock( beconfig, user, host )
     struct bedata * beconfig;
     char * user, * host;
{
	if ( ! beconfig->lock.locked ) {
		beconfig->lock.pidodt = BADPID;
		beconfig->lock.piddown = BADPID;
		beconfig->lock.locked = TRUE;
		strncpy( beconfig->lock.user, user, MAXUID - 1 );
		beconfig->lock.user[ MAXUID - 1 ] = '\0';
		touchit( beconfig );
		return( SYSOK );
	}
	if ( ! strcmp( user, beconfig->lock.user ) ) {
		touchit( beconfig );
		return( SYSOK );
	}
	return( SYSERR );
}


/*---------------------------------------------------------------------------
 * unlock(beconfig, user, host)
 * 	struct bedata *beconfig;  char user, host;
 *
 * release the lock on the backend machine.
 *---------------------------------------------------------------------------
 */
int
unlock( beconfig, user, host )
     struct bedata * beconfig;
     char * user, * host;
{
	int flag = FALSE;
	
	if ( ! beconfig->lock.locked ) {
		return( SYSERR );
	}
	if ( beconfig->lock.pidodt != BADPID ) {
		kill( beconfig->lock.pidodt, SIGKILL );
	}
	if ( beconfig->lock.piddown != BADPID ) {
		kill( beconfig->lock.piddown, SIGKILL );
	}
	beconfig->lock.pidodt = BADPID;
	beconfig->lock.piddown = BADPID;
	strcpy( beconfig->lock.user, NOBODY );
	beconfig->lock.locked = FALSE;
	touchit( beconfig );
	return( SYSOK );
}


/*---------------------------------------------------------------------------
 * checkuid(beconfig, req->uid)
 * 	struct bedata *beconfig; char *user;
 *
 * allocate the backend if noone is using it
 *---------------------------------------------------------------------------
 */
int
checkuid( cmd, beconfig, user, msg )
     int cmd;
     struct bedata *beconfig;
     char *user;
     char *msg;
{
	if ( ! beconfig->lock.locked ) {
		beconfig->lock.pidodt = BADPID;
		beconfig->lock.piddown = BADPID;
		strncpy( beconfig->lock.user, user, MAXUID - 1 );
		beconfig->lock.user[ MAXUID - 1 ] = '\0';
		beconfig->lock.locked = TRUE;
		VERBOSE(Log( "assigning %s to %s", beconfig->data.bename,
			    beconfig->lock.user ));
		return( SYSOK );
	}
	if ( strcmp( beconfig->lock.user, user ) ) {
		strcpy( msg, "no privilege" );
		return( SYSERR );
	}
	if ( cmd == REQ_DNLD_CONNECT ) {
		if ( beconfig->lock.piddown != BADPID ) {
			strcpy( msg, "download in progress" );
			return( SYSERR );
		}
		return( SYSOK );
	}
	if ( beconfig->lock.pidodt != BADPID ) {
		strcpy( msg, "odt in progress" );
		return( SYSERR );
	}
	return( SYSOK );
}


#if defined(HPPA)
#define utimes utime
#endif

/*---------------------------------------------------------------------------
 * touchit(beconfig)
 * 	struct bedata *beconfig;
 *
 * touch the back end machine
 *---------------------------------------------------------------------------
 */
touchit( beconfig )
     struct bedata * beconfig;
{
	struct timeval tarray[ 2 ];
	time_t t;
	
	t = time( 0 );
	tarray[ 0 ].tv_sec = t;
	tarray[ 0 ].tv_usec = 0;
	tarray[ 1 ].tv_sec = t;
	tarray[ 1 ].tv_usec = 0;
	if ( utimes( beconfig->data.ttyname, tarray ) ) {
		Log( "can't touch serial line '%s' for back end '%s'",
		    beconfig->data.ttyname, beconfig->data.bename );
	}
	VERBOSE(Log( "touching backend machine '%s'", beconfig->data.bename ));
}
