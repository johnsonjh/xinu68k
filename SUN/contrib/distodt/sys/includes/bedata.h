/* 
 * bedata.h - Bed machines data base header file
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 26 23:01:36 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

/* #define SOCK_R 1 */
/* #define SOCK_W 0 */

#define NUM_BE_ARG	10

struct bedata {
    struct bedata_data {
	    
    char *ttyname,		/* serial line backend is connected 	*/
    	 *ttyrate,		/* baud rate to use on serial line	*/
	 *bename,		/* backend machine name			*/
         *beclass,		/* class of backend machine		*/
    	 *betype,		/* cpu type of backend machine		*/
         *dloaddpath,		/* downloadd program			*/
    	 *dloaddmethod,		/* serial or ethernet			*/
    	 *dloadfile,		/* download file			*/
         *odtdpath, 		/* odtd program				*/
    	 *odtdmethod,		/* serial or ethernet			*/
 	 *max_reservation;	/* maximum reservation  time		*/
    
    } data;

    struct lock {		/* lock structure			*/
	 char user[MAXUID];	/* user id				*/
	 int pidodt, piddown;	/* odtd and downloadd process		*/
	 char locked;		/* is backend machine reserved		*/
	 int fd;		/* file descriptor for tty line		*/
    } lock;

    struct bedata *next;	/* next pointer				*/
};

extern struct bedata *readbedata(), *makebedata();
extern struct bedata *getbeconfig();

#define BADPID (-1)


#define NUM_BED_DATA	(sizeof(struct bedata_data) / sizeof(char *))
