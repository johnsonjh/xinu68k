
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

struct odtdata {
    char *sline,
         *bename,
         *dload,
         *betype,
         *dloadpath,
         *baud,
         *class,
         *odttype;
};

extern struct odtdata **table,
    *entry;

extern int size, cnt;

extern struct odtdata **readodtdata(), *makeodt();
extern struct odtdata *getbeconfig();

