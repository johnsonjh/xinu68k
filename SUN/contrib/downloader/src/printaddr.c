/* 
 * printaddr.c - PrintIPAddr, PrintEAddr
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Oct 24 15:36:47 1988
 *
 * Copyright (c) 1988 Shawn Ostermann
 */

#include <ether.h>
#include <ip.h>


/*
 * PrintIPAddr - print the IP address in dot notation preceeded by the
 *               given string
 */
PrintIPAddr(msg,addr)
     char *msg;
     IPaddr addr;
{
    kprintf("%s: %d.%d.%d.%d\n", msg, addr[0],addr[1],addr[2],addr[3]);
}



/*
 * PrintEAddr - print the Ethernet address in dot notation preceeded by the
 *              given string
 */
PrintEAddr(msg,addr)
     char *msg;
     Eaddr addr;
{
    kprintf("%s: %x.%x.%x.%x.%x.%x\n", msg,
	    addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
}
