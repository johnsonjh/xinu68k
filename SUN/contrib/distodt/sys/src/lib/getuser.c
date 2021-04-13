/* getuser.c - 
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 22:02:03 1988
 */

#include <stdio.h>
#include <pwd.h>



/* getlogin does not work when odt, or download is used within a 	*/
/* 'script' session - so we must use getpwuid like whoami does		*/

struct passwd * getpwuid();		/* get password entry 		*/


/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
int
getuser( user )
     char * user;
{
	struct passwd * pp;
	
	pp = getpwuid( geteuid() );
	if ( pp == (struct passwd *) NULL ) {
		return( -1 );
	}
	strcpy( user, pp->pw_name );
	return( 1 );
}
