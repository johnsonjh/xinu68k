/* 
 * scanaddr.c - ScanIPAddr, ScanEAddr
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Oct 24 15:36:47 1988
 *
 * Copyright (c) 1988 Shawn Ostermann
 */

#include <tftpload.h>
#include <ether.h>
#include <ip.h>

#define isdigit(x)     ( ((x) >= '0') && ((x) <= '9'))
#define ishexdigit(x)  ( (isdigit(x)) || ((x >= 'A') && (x <= 'F')))


/*
 * ScanIPAddr - convert the string to an IP address.  Returns OK if a valid
 *              address was found, and SYSERR otherwise.  The addr must be in
 *    	        dotted decimal notation, and must end in a null or a '\r'. 
 */
ScanIPAddr(psb, addr)
     char *psb;
     IPaddr addr;
{
    int i;

    /* we want to avoid scanf, because the code is too long to download */
    for (i=0; i<IPLEN; ++i) {
	if (! isdigit(*psb))
	    return(SYSERR);
	addr[i] = 0;
	while (isdigit(*psb)) {
	    addr[i] = 10 * addr[i] + (*psb - '0');
	    ++psb;
	}
	if ((i < (IPLEN-1)) && (*psb != '.'))
	    return(SYSERR);
	if (*psb == '.')
	    ++psb;
    }

    if ((*psb != '\00') && (*psb != '\r'))
	return(SYSERR);
    return(OK);
}



/*
 * ScanEAddr -  convert the string to an Ether address.  Returns OK if a valid
 *              address was found, and SYSERR otherwise.  The addr must be in
 *    	        dotted hexadecimal notation, and must end in a null or a '\r'. 
 */
ScanEAddr(psb, addr)
     char *psb;
     Eaddr addr;
{
    int i;

    /* we want to avoid scanf, because the code is too long to download */
    for (i=0; i<EPADLEN; ++i) {
	if (! ishexdigit(*psb))
	    return(SYSERR);
	addr[i] = 0;
	while (ishexdigit(*psb)) {
	    if (isdigit(*psb))
		addr[i] = 16 * addr[i] + (*psb - '0');
	    else
		addr[i] = 16 * addr[i] + 10 + (*psb - 'A');
	    ++psb;
	}
	if ((i < (IPLEN-1)) && (*psb != '.'))
	    return(SYSERR);
	if (*psb == '.')
	    ++psb;
    }

    if ((*psb != '\00') && (*psb != '\r'))
	return(SYSERR);
    return(OK);
}




