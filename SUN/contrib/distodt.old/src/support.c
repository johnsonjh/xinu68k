
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

#include "../h/bed.h"
#include "../h/bedata.h"

struct odtdata **table, *entry;

int size, cnt;

/*
 *---------------------------------------------------------------------------
 * readodtdata(ptbl, pnum):
 * 	struct odtdata ***ptbl;
 * 	int *pnum;
 *
 * will fill the table from the machines database, and then
 * will return the table and count of entries read.
 *---------------------------------------------------------------------------
 */
struct odtdata **readodtdata(ptbl, pnum)

struct odtdata ***ptbl;
int *pnum;

{
    int ifd, savefd, pos;

    savefd = dup(0);

    if (savefd < 0) {
  	perror("readodtdata()");
	exit(1);
    }
    
    if ((ifd = open(IN_FILE, O_RDONLY, 0644)) < 0) {
	perror(IN_FILE);
	exit(1);
    }


    if (dup2(ifd, 0) < 0) {
	perror("readodtdata() : dup2(ifd, 0)");
	exit(1);
    }

    if (!(table = (struct odtdata **) 
		malloc(size * sizeof (struct odtdata *)))) {
	fprintf(stderr, "Not enough memory for data table.\n");
	exit(1);
    }

    yyparse();

    if (dup2(savefd, 0) < 0) {
	perror("readodtdata() : dup2(savefd, 0)");
	exit(1);
    }
	
    *pnum = cnt;
    *ptbl = (cnt > 0) ? table : NULL;

    close(ifd);
}

/*
 *---------------------------------------------------------------------------
 * char *newsb(sb)
 * 	char *sb;
 *
 * makes a new string and copies in the contents of the arg.
 *---------------------------------------------------------------------------
 */
char *newsb(sb)

     char *sb;

{
    char *sb2 = (char *) malloc(strlen(sb) + 1);
    strcpy(sb2, sb);
    return(sb2);
}

/*
 *---------------------------------------------------------------------------
 * struct odtdata *makeodt(sb1, sb2, ..., sb8)
 * 	char *sb1, ..., *sb8;
 *
 * takes the 8 strings it is handed and makes an odt table
 * entry from them.  Order is this:
 *
 *  sline = sb1;
 *  bename = sb2;
 *  dload = sb3;
 *  betype = sb4;
 *  dloadpath = sb5;
 *  baud = sb6;
 *  class = sb7;
 *  odttype = sb8;
 *---------------------------------------------------------------------------
 */
struct odtdata *makeodt(sb1, sb2, sb3, sb4, sb5, sb6, sb7, sb8)

     char *sb1, *sb2, *sb3, *sb4, *sb5, *sb6, *sb7, *sb8;

{
    struct odtdata *podtent = (struct odtdata *) malloc(sizeof(struct odtdata));

    podtent->sline = sb1;
    podtent->bename = sb2;
    podtent->dload = sb3;
    podtent->betype = sb4;
    podtent->dloadpath = sb5;
    podtent->baud = sb6;
    podtent->class = sb7;
    podtent->odttype = sb8;

    return(podtent);
}



/*----------------------------------------------------------------------
 * getbeconfig - returns a pointer to the odtdata structure
 *		for backend bename
 *----------------------------------------------------------------------
 */
struct odtdata *getbeconfig(beconfig, becount, bename)
struct odtdata *beconfig[];		/* config status of all bends	*/
int  becount;				/* number of backends		*/
char *bename;				/* backend name to look for	*/
{
	struct odtdata *bedata;		/* be odtdata pointer		*/
	int i;				/* counter variable		*/

	for (i=0; i<becount; i++) {
		bedata = beconfig[i];
		if (strcmp(bedata->bename, bename) == 0) {
			return(bedata);	/* we found it			*/
		}
	}
	return(NULL);			/* did not find it		*/
}

