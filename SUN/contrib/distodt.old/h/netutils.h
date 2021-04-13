
/* 
 * netutils.h - Header file for network utilitie routines
 * 
 * Author:	Tim Korb/Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 16:06:18 1988
 *
 * Copyright (c) 1988 Tim Korb/Shawn Oostermann/Jim Griffioen
 */

# define lsa (sizeof (struct sockaddr_in))

char *IaToSb ();
char *GetHostName ();
struct sockaddr_in *FdToRemoteSa ();
struct sockaddr_in *FdToLocalSa ();
