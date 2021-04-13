/* 
 * eeprom.c - getbootparms, setbootparms, printbootparms
 * 
 * Author:	Shawn Ostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Oct 24 15:36:47 1988
 *
 * Copyright (c) 1988 Shawn Ostermann
 */

#include <network.h>
#include <ip.h>
#include <ether.h>
#include <eeprom.h>


/* The EEPROM requires a delay between writes for EEPROM recovery. */
/*  (this is a procedure rather than a macro, because the image	   */
/*   is smaller that way.  If we were really interested in speed,  */
/*   of course, then we'd have REAL problems with the delay!!)     */
#define DELAYVAL 10000
delay()
{
    int dl;
    for(dl=0;dl<DELAYVAL;++dl)
	/* sit and spin */;
}


/* the eeprom structure that we use */
struct eepromblock *eeprom = (struct eepromblock *) EEPROMADDR;


/*
 * getbootparms -- read the fields out of the boot parm block stored
 *                 in the EEPROM
 */
getbootparms(myaddr, desteaddr, destaddr, fname)
     IPaddr	myaddr;
     Eaddr	desteaddr;
     IPaddr	destaddr;
     char	fname[MAXFILENAME];
{
    /* grab both IP addresses */
    blkcopy(myaddr,eeprom->myaddr,IPLEN);
    blkcopy(destaddr,eeprom->destaddr,IPLEN);
    
    /* grab dest Ether address */
    blkcopy(desteaddr,eeprom->desteaddr,EPADLEN);

    /* copy in the file name */
    blkcopy(fname,eeprom->fname,MAXFILENAME);
}


/*
 * setbootparms -- write the fields into the boot parm block stored
 *                 in the EEPROM
 */
setbootparms(myaddr, desteaddr, destaddr, fname)
     IPaddr	myaddr;
     Eaddr	desteaddr;
     IPaddr	destaddr;
     char	fname[MAXFILENAME];
{
    int i;

    /* write both IP addresses */
    for (i=0; i<IPLEN; ++i) {
	eeprom->myaddr[i] = myaddr[i];
	delay();
	eeprom->destaddr[i] = destaddr[i];
	delay();
    }

    /* write dest Ether address */
    for (i=0; i<EPADLEN; ++i) {
	eeprom->desteaddr[i] = desteaddr[i];
	delay();
    };

    /* copy in the fname */
    for (i=0; i<MAXFILENAME; ++i) {
	eeprom->fname[i] = fname[i];
	delay();
    };
}



/*
 * printbootparms -- print the fields from the boot parm block stored
 *                   in the EEPROM
 */
printbootparms()
{
    IPaddr	myaddr;
    Eaddr	desteaddr;
    IPaddr	destaddr;
    char	fname[MAXFILENAME];
     
    getbootparms(myaddr,desteaddr,destaddr,fname);

    PrintIPAddr("          My IP address",myaddr);
    PrintIPAddr("        Dest IP address",destaddr);
    PrintEAddr ("  Dest Ethernet address",desteaddr);
    kprintf    ("      default File name: %s\n",fname);
}


