/* getoptions.c - get the command line options
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Sun Aug 19 15:49:17 1990
 */

#include <stdio.h>
#include "bed.h"

#define TRUE 	1
#define FALSE 	0

static int cnt = 0, cpos = 0;
static char **options, *pos = NULL;
static char buf[3];

extern char *nextword(), *nextopt(), *nextarg();

/*---------------------------------------------------------------------------
 * setupopt
 *---------------------------------------------------------------------------
 */
void
setupopt(csb, rgsb)
     int csb;
     char *rgsb[];
{
	cnt = csb - 1;
	options = &(rgsb[ 1 ]);
	if ( cnt > 0 ) {
		pos = *options;
	}
	else {
		pos = NULL;
	}
	cpos = 0;
}

/*---------------------------------------------------------------------------
 * resetopt
 *---------------------------------------------------------------------------
 */
static void
resetopt()
{
	if ( *pos == '\0' ) {
		cpos++;
		options++;
		if ( cpos < cnt ) {
			pos = *options;
		}
		else {
			pos = NULL;
		}
	}
}

/*---------------------------------------------------------------------------
 * nextword
 *---------------------------------------------------------------------------
 */
char *
nextword()
{
	char *ans;
	
	if ( ( ans = nextopt() ) == NULL ) {
		ans = nextarg();
	}
	return( ans );
}


/*---------------------------------------------------------------------------
 * nextopt
 *---------------------------------------------------------------------------
 */
char *
nextopt()
{
	if ( pos == NULL || *pos != '-' ) {
		return NULL;
	}
	buf[ 1 ] = buf[ 2 ] = '\0';
	buf[ 0 ] = *pos++;
	if ( *pos != '\0' ) {
		buf[ 1 ] = *pos++;
	}
	resetopt();
	return( buf );
}

/*---------------------------------------------------------------------------
 * nextarg
 *---------------------------------------------------------------------------
 */
char *
nextarg()
{
	char *ans;
	
	if ( pos == NULL || *pos == '-') {
		return( NULL );
	}
	ans = pos;
	pos = ans + strlen( ans );	/* pos points to end of string */
	resetopt();
	return( ans );
}



