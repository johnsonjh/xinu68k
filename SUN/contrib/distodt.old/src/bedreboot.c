
/* 
 * bedreboot.c - This program reboots the specified backends
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 20:50:05 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"

struct option opt;			/* command line options		*/

/*
 *---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main (csb, rgsb)
int csb;
char *rgsb[];
{
    int i;
    int sock;
    struct bm bm;

    if (csb == 1) {
	    fprintf(stderr, "usage: bedreboot [frontend-machine] ...\n");
	    exit(1);
    }
    
    for (i = 1; i < csb; i++) {
	    bm.ver = CURVER;
	    sprintf(bm.cmd, "%d", REQ_REBOOT);
	    strncpy (bm.detail, "", MAXDETAIL);

	    sock = ConnectUdp (rgsb[i], KNOWN_UDP_PORT);

	    if (bmsend (sock, &bm, lbm, 0) <= 0)
		SysError ("send");

	    bmrecv (sock, &bm, lbm, 0);
	    printf("rebooted front end %s: %s\n", rgsb[i], bm.detail);
    }
    exit (0);
}

