/* 
 * readaddr.c - readaddr
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Oct 24 15:36:47 1988
 *
 * Copyright (c) 1988 Shawn Ostermann
 */

#include <tftpload.h>
#include <network.h>
#include <ether.h>
#include <ip.h>
#include <eeprom.h>

/*
 * ReadIP -- persistently prompt for an IP address until recognizable
 */
ReadIP(msg, addr)
     char *msg;
     IPaddr addr;
{
    char *ibuf;
    
    for(;;) {
	kprintf("%s (default %d.%d.%d.%d) : ",msg,
		addr[0],addr[1],addr[2],addr[3]);
        ibuf = (char *) kgetline();
	if (*ibuf == '\r')
	    break;
	
	if (ScanIPAddr(ibuf,addr) == OK)
	    break;
	
	kprintf("Invalid input, try again...\n");
    }
}



/*
 * ReadEth -- persistently prompt for an Ethernet address until recognizable
 */
ReadEth(msg, addr)
     char *msg;
     Eaddr addr;
{
    char *ibuf;

    for (;;) {
	kprintf("%s (default %x.%x.%x.%x.%x.%x) : ",msg,
		addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
	ibuf = (char *) kgetline();
	if (*ibuf == '\r')
	    break;
	
	if (ScanEAddr(ibuf,addr) == OK)
	    break;
	
	kprintf("Invalid input, try again...\n");
    }
    
}


/*
 * ReadStr -- read a string into pch unless defaulted by <return>
 */
ReadStr(msg, pch)
     char *msg;
     char pch[MAXFILENAME];
{
    char *ibuf;
    int i;

    pch[MAXFILENAME-1] = '\00';
    
    kprintf("%s (default %s): ",msg,pch);
    
    ibuf = (char *) kgetline();
    if (*ibuf != '\r')  {
	i = 0;
	while ((i < MAXFILENAME-1) &&
	       (*ibuf != NULLCH) &&
	       (*ibuf != '\r')) {
	    pch[i++] = *ibuf++;
	}
	pch[i] = '\00';
    }
}    
