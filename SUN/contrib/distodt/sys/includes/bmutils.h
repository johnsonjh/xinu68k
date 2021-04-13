
/* 
 * bmutils.h - Definitions for bm structure utilities
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 16:04:11 1988char
 *
 * Copyright (c) 1988char *CmdToSb (); Shawn Oostermann/Jim Griffioen
 */

char *UidToSb ();
char *ServToSb ();
char *DescToSb ();
char *OmToSb ();
char *BmToSb();
char *SbToBm();

#define BITMASK(n) (1<<(n))

