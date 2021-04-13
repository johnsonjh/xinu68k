
/* 
 * utils.c - Miscellaneous Utility routines like Log(), Error(), SysError()
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 11:36:27 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <assert.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern int errno;

#include "../h/utils.h"

/*
 *---------------------------------------------------------------------------
 * TimeToSb (ti) -- return pointer to current time string.
 *---------------------------------------------------------------------------
 */
char *TimeToSb (ti)
long ti;
{
    struct tm *ptm;
    char *sb;

    ptm = localtime (&ti);
    sb = asctime (ptm);

    /* sb in form: "Sat Nov 10 11:23:13 1984" */

    sb[19] = '\0';	/* clobber year */
    sb += 4;		/* scoot over day of week */

    return sb;
}

/*
 *---------------------------------------------------------------------------
 * Log (sb, w1, w2, w3, w4, w5) -- add log message to stderr.
 *---------------------------------------------------------------------------
 */

Log (sb, w1, w2, w3, w4, w5)
char *sb;
int w1, w2, w3, w4, w5;
{
   fprintf (stderr, "%s\t", TimeToSb (time (0L)));
   fprintf (stderr, sb, w1, w2, w3, w4, w5);
   fprintf (stderr, "\n");
   fflush (stderr);
}

/*
 *---------------------------------------------------------------------------
 * Error (sb, w1, w2) -- print error and exit.
 *---------------------------------------------------------------------------
 */
Error (sb, w1, w2)
char *sb;
int w1, w2;
{
    fprintf (stderr, sb, w1, w2);
    fprintf (stderr, "\n");
    exit (1);
}

/*
 *---------------------------------------------------------------------------
 * SysError (sb) -- report system error and exit.
 *---------------------------------------------------------------------------
 */
SysError (sb)
char *sb;
{
    perror (sb);
    exit (1);
}

/*
 *---------------------------------------------------------------------------
 * RgchToSb (sb, rgch, cchMax) -- convert a character buffer in rgch, of 
 * maximum length cchMax, to printable characters in sb. Ignore nulls.
 * NB: cchMax applies to rgch, not sb (no overflow checking!).
 *---------------------------------------------------------------------------
 */
RgchToSb (sb, rgch, cchMax)
char *sb;
char rgch[];
int cchMax;
{
    char *pch, *pchMax;
    int i = 0;

    assert (rgch != NULL);
    assert (sb != NULL);

    for (pch = rgch, pchMax = pch+cchMax; pch < pchMax; pch++)
        if (isascii (*pch) && isprint (*pch))
	    sb[i++] = *pch;
	else if (*pch != '\0') {
	    sprintf (&sb[i], "\\%03o", *pch);
	    i += 4;
	    }
    sb[i] = '\0';
}

