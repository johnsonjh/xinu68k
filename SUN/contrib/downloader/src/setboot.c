/* 
 * setboot.c - setboot
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


/*------------------------------------------------------------------------
 *  setboot - set the Xinu boot information
 *------------------------------------------------------------------------
 */

main()
{
	IPaddr	myaddr;			/* my IP address		*/
	Eaddr   desteaddr;		/* eth address to send to	*/
	IPaddr  destaddr;		/* IP address to send to	*/
	char	fname[MAXFILENAME];
	
	kprintf("\nXinu setboot program\n\n");

	getbootparms(myaddr,desteaddr,destaddr,fname);
	
	kprintf("\nPlease enter the appropriate addresses below\n");
	kprintf("     (IP addresses in decimal)\n");
	kprintf("     (Ether addresses in hexadecimal, upper case)\n");

	ReadIP ("  Backend IP Address",myaddr);
	ReadIP ("  TFTP Source IP Address",destaddr);
	ReadEth("  TFTP Source Ethernet Address",desteaddr);
	ReadStr("  Default image file name",fname);

	setbootparms(myaddr,desteaddr,destaddr,fname);

	kprintf("\nNew boot parameters in eeprom:\n");
	printbootparms();

	kprintf("\nDONE\n\n");
}
