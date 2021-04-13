/* 
 * support.c - Support routines for reading the machines database
 * 
 * Author:	Steve Chapin/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 11:33:19 1988
 *
 * Copyright (c) 1988 Steve Chapin/Jim Griffioen
 */


#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "bed.h"
#include "bedata.h"
#include "tokens.h"

extern char *xmalloc();
extern char *tkvalue();
extern char *matchtoken();

static char * staticFilename;

/*----------------------------------------------------------------------
 * getbeconfig - returns a pointer to the bedata structure
 *		for backend bename
 *----------------------------------------------------------------------
 */
struct bedata *
getbeconfig( bedata, bename )
     struct bedata *bedata;
     char *bename;
{
	for (; bedata != NULL; bedata = bedata->next ) {
		if ( strcmp( bedata->data.bename, bename ) == 0 ) {
			return( bedata );
		}
	}
	return( NULL );
}

/*
 *---------------------------------------------------------------------------
 * readbedata(pnum):
 * 	int *pnum;
 *
 * will fill the table from the machines database, and then
 * will return the table and count of entries read.
 *---------------------------------------------------------------------------
 */
struct bedata *
readbedata( filename )
     char * filename;
{
	struct bedata *ptbl, *scan;
	int savedFd, newFd;
	
	staticFilename = filename;
	
	if ( ( savedFd = dup( 0 ) ) < 0 ) {
		Error( "readbedata(): dup(0) failed\n" );
	}
	if ( ( newFd = open( filename, O_RDONLY, 0644 ) ) < 0 ) {
		Error("readbedata(): error opening %s", filename );
	}
	if ( dup2( newFd, 0 ) < 0 ) {
		Error( "readbedata(): error dup2( newFd, 0 )\n" );
	}
	
	ptbl = scan = makebedata();
	while ( scan != NULL ) {
		scan->next = makebedata();
		scan = scan->next;
	}
	
	if ( dup2( savedFd, 0 ) < 0 ) {
		Error( "readbedata(): dup2(savedFd, 0)\n" );
	}
	close( newFd );
	return( ptbl );
}

/*
 *---------------------------------------------------------------------------
 * struct bedata *makebedata()
 *
 * read the NUM_BE_ARG strings 
 *
 *---------------------------------------------------------------------------
 */
struct bedata *
makebedata()
{
	struct bedata *mdata;
	int tk;
	char * tv;
	
	while ( ( tk = tknext() ) == TKNEWLINE ) {
		/* NULL STATEMENT */;
			    }
	if ( tk == TKEOF ) {
		return( NULL );
	}
	if ( tk != TKSTRING ) {
		Error( "Error in '%s' file", staticFilename );
	}
	tv = tkvalue();
	if ( tv[ strlen( tv ) - 1 ] == ':' ) {
		tv[ strlen( tv ) -1 ] = '\0';
		if ( ! ismyname( tv ) ) {
			while ( ( ( tk = tknext() ) != TKNEWLINE ) &&
			       ( tk != TKEOF ) ) {
				/* NULL STATEMENT */;
					    }
			return( makebedata() );
		}
		tk = tknext();
		if ( tk != TKSTRING ) {
			Error( "Error in '%s' file", staticFilename );
		}
	}
	
	mdata = (struct bedata *) xmalloc( sizeof( struct bedata ) );
	mdata->data.ttyname = tkvalue();
	
	mdata->data.ttyrate = matchtoken( TKSTRING );
	mdata->data.bename = matchtoken( TKSTRING );
	mdata->data.beclass = matchtoken( TKSTRING );
	mdata->data.betype = matchtoken( TKSTRING );
	mdata->data.dloaddpath = matchtoken( TKSTRING );
	mdata->data.dloaddmethod = matchtoken( TKSTRING );
	mdata->data.dloadfile = matchtoken( TKSTRING );
	mdata->data.odtdpath = matchtoken( TKSTRING );
	mdata->data.odtdmethod = matchtoken( TKSTRING );
	mdata->data.max_reservation = matchtoken( TKSTRING );
	
	tk = tknext();
	if ( tk != TKNEWLINE && tk != TKEOF )  {
		Error( "Error in '%s' file", staticFilename );
	}
	
	mdata->next = NULL;
	mdata->lock.locked = FALSE;
	mdata->lock.pidodt = BADPID;
	mdata->lock.piddown= BADPID;
	strcpy( mdata->lock.user, NOBODY );
	
	if ( ( mdata->lock.fd = open( mdata->data.ttyname, O_RDONLY, 0644 ) )
	    < 0 ) {
		Error( "error: open(%s) failed", mdata->data.ttyname );
	}
	touchit( mdata );
	return( mdata );
}

char *
matchtoken( tk )
     int tk;
{
	if ( tk != tknext() ) {
		Error( "Error in '%s' file", staticFilename );
	}
	return( tkvalue() );
}


extern YYSTYPE yylval;
extern int yylex();

int
tknext()
{
	return( yylex() );
}

char *
tkvalue()
{
	return( yylval.sb );
}

/* 
 * test.c - is this MY name
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jul 30 12:36:38 1991
 *
 * Copyright (c) 1991 Shawn Ostermann
 */
ismyname( name )
     char * name;
{
	static char myrealname[ 100 ];
	static amnesia = 1;
	struct hostent *he;
	
	if (amnesia) {
		/* see who I am */
		char myname[ 100 ];
		
		gethostname( myname, sizeof( myname ) );
		he = gethostbyname( myname );
		strcpy( myrealname, he->h_name );
		amnesia = 0;
	}
	if ( ( he = gethostbyname( name ) ) == (struct hostent *) 0 ) {
		return( 0 );
	}
	return( strcmp( myrealname, he->h_name ) == 0 );
}
