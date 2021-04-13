
/* 
 * sitespecific.h - 
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 16:01:48 1988
 *
 * Copyright (c) 1988/* Jim Griffioen
 */

/*
 *---------------------------------------------------------------------------
 * These are site specific constants and should be modified to work with
 * your machines and file system
 *---------------------------------------------------------------------------
 */

#define BED_EXEC_PATH	"/usr/xinu/bin/bed"
					/* full bed program name	*/

#define BASEDIR		"/tmp/xinu/"
					/* basedir for lock files	*/
    
#define MACHINES_FILE	"/usr/xinu/database/machines" 
					/* machines database name	*/
    
#define BEDSERVERS_FILE	"/usr/xinu/database/bedservers"	 
					/* list of bedservers on the net*/
    
#define	LOGGINGDIR	"/usr/xinu/bedlog/"
					/* log file to write errors to	*/
    
#define KNOWN_UDP_PORT	2024		/* port bed will listen on	*/
#define MAXNETPACKET	1500		/* low est of network packet size */
#define APPROXUDPHEAD	200		/* hi est of eth/ip/udp header size */
#define STATTIMEOUT	1		/* status timeout set to 1 seconds */
#define DOMAINNAME	"purdue.edu"	/* domain name not to be printed */

/* Environment Variables */
#define ENVFILENAME	"ODTFILENAME"
#define ENVBEMACHINE	"ODTBEMACHINE"
#define ENVBEDSERVERS 	"ODTBEDSERVERS"
#define ENVCLASS	"ODTCLASS"
#define DEFAULTCLASS	"SUN"		/* default class		*/

/* number of times to restart the bed */
#define MAXRESTARTS 5

/* should the bed be in background ? */
#define BACKGROUND

/***** End site specific constants ******/


