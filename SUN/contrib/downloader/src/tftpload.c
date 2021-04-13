/* 
 * tftpload.c - tftpload 
 * 
 * Author:	Shawn Ostermann, Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Oct 24 15:36:47 1988
 *
 * Copyright (c) 1988 Shawn Ostermann, Jim Griffioen
 */


#include <tftpload.h>
#include <ether.h>
#include <network.h>
#include <ip.h>
#include <udp.h>
#include <arp.h>
#include <tftp.h>
#include <a.out.h>
#include <eeprom.h>
#include <sunromvec.h>


/* ethernet control structures */
extern struct etblk ethblock;
extern struct le_device le;

/* global addresses and TFTP information */
Eaddr	myeaddr;		/* my ethernet address		*/
IPaddr	myaddr;			/* my IP address		*/
Eaddr   desteaddr;		/* eth address to send to	*/
IPaddr  destaddr;		/* IP address to send to	*/
char	fname[MAXFILENAME];	/* file to download via tftp	*/


/*------------------------------------------------------------------------
 *  TFTPload  - This program copies itself up into high physical memory
 *		and then uses the tftp protocol to retrieve the desired
 *		file over the ethernet.  The file is deposited at location
 *		0x4000 and the program automatically jumps there and begins
 *		running.  Initially startup.s is called.  Startup.s copies
 *		this entire program up into high physical and then jumps
 *		up there and begins running.  So by the time tftpload is
 *		called, we are already running in high physical memory
 *------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------
 *  tftpload - load a file into the sun memory using tftp - the file
 *		name is prompted for
 *------------------------------------------------------------------------
 */
main()
{
    struct	epacket	*epktin;	/* pointer to Ethernet frame	*/
    struct	ip	*ipptr;		/* pointer to IP part of rbuf	*/
    struct	udp	*udpptr;	/* pointer to udp part of rbuf	*/
    struct	tftp	*prtftp;	/* pointer to read command pkt	*/
    int		i, k;			/* loop indices			*/
    int		len;			/* length of input		*/
    char	*wbuf;			/* buffer for ethernet write	*/
    char	*rbuf;			/* buffer for ethernet read	*/
    char	*from, *to;		/* char ptrs for copying	*/
    short	packetid;		/* IP packet id			*/
    Bool	lastpack;		/* true if last packet		*/
    Bool	interactive;		/* true if '-i' flag used	*/
    struct	exec *aouthdr;		/* ptr to a.out header		*/
    long	imsize;			/* size of program downloaded   */
    short	nextblock;		/* next block # we need		*/
    long	bytesread;		/* number of bytes downloaded	*/
    int		numpackets;		/* expected number of packets	*/
    int		tickinterval;		/* how many packets per tick	*/
    char	**argv;			/* stored by prom in hi mem	*/


    kprintf("Xinu downloader:\n");

#ifdef DEBUG	
    /* what the world looks like */
    kprintf("start: 0x%x\n",&start);
    kprintf("etext: 0x%x\n",&etext);
    kprintf("edata: 0x%x\n",&edata);
    kprintf("end:   0x%x\n",&end);
#endif

    /* initialize other variables */
    packetid = 1;		/* TFTP sequence number			*/
    to = (char *) LOADSTART;    /* start loading at location LOADSTART	*/
    lastpack = FALSE;		/* set to true when last packet seen	*/
    interactive = FALSE;	/* don't read addresses from stdin	*/
    bytesread = 0;		/* count the bytes read in		*/
    nextblock = 1;		/* next block expected			*/
    fname[0] = '\00';		/* file name is null for now		*/
    *((int *)myaddr) = 0;	/* my IP address is 0			*/
    *((int *)destaddr) = 0;	/* dest IP address is 0			*/
    bzero(desteaddr,EPADLEN);	/* dest ETHERNET address is 0		*/

    /* initialize DVMA memory */
    initdmamem();

    /* initialize ethernet device */
    einit();

#ifdef USE_INTERACTIVE
    /* parse the arguments */
    for (argv = (*romp->v_bootparam)->bp_argv; *argv != NULL; ++argv) {
	if (blkequ(*argv,"-i",3)) {
	    interactive = TRUE;
	    break;
	}
    }
#endif    
    
    
    /* set up the buffers to use for the packets */
    rbuf = (char *) 0;		/* eread gives us one */
    wbuf = (char *) getdmem(EMAXPAK);


    /* at this point, we know the following address:		*/
    /*   1) MY ETHER address (from eeprom)			*/
    /* We need the following addresses:				*/
    /*   1) my IP address					*/
    /*   2) tftp hosts's ETHER address				*/
    /*   3) tftp hosts's IP address				*/
    /* we have to be able to get ALL 3 from one of the 		*/
    /* sources:							*/
    /*   1) get the addresses from the eeprom			*/
    /*   2) if '-i' specified, allow the user to edit them	*/
    /*	 3) if not satisfactory, use RARP to obtain them	*/


#ifdef USE_EEPROM    
    /* 1) get the addresses from the eeprom */
    getbootparms(myaddr,desteaddr,destaddr,fname);
#endif


#ifdef USE_INTERACTIVE    
    /* 2) read from stdin if interactive */
    if (interactive) {
	kprintf("You must give me all of the following addresses\n");
	kprintf("in dotted notation:\n\n");
	kprintf("     (IP addresses in decimal)\n");
	kprintf("     (Ether addresses in hexadecimal, upper case)\n");
	ReadIP ("Backend IP Address",myaddr);
	ReadIP ("TFTP Source IP Address",destaddr);
	ReadEth("TFTP Source Ethernet Address",desteaddr);
	ReadStr("Default image file name",fname);
    }
#endif    

    /* 3) Use RARP if I don't have ALL the addresses */
    if ((*((int *) myaddr) == 0) ||
	(*((int *) destaddr) == 0) ||
	(*((int *) desteaddr) == 0)) {	/* close enuf */

#ifdef DEBUG	
	kprintf("Using RARP to get addresses\n");
#endif	
	/* Use RARP to find out interesting IP addresses */
	for (;;) {
	    if (RarpForAddresses(wbuf) == OK)
		break;
	    kprintf("NO response to RARP, trying again\n");
	}
    }

	
    /* unless told otherwise, the file to get is called "a.out" */
    if (*fname == '\00')
	strcpy(fname, "a.out");

    
#ifdef DEBUG
    PrintEAddr ("My physical addr",myeaddr);
    PrintIPAddr("My IP addr",myaddr);
    PrintEAddr ("Dest physical addr",desteaddr);
    PrintIPAddr("Dest IP addr",destaddr);
    kprintf    ("File to fetch: %s\n",fname);
#endif
	
	
    /* send out RRQ (read request) to my tftp server */
    makepack(wbuf, myeaddr, myaddr, MY_UDP_PORT,
	     desteaddr, destaddr, TFTPPORT, packetid++);
    tftpwr(TRRQ, 0, fname, OCTET, wbuf);
	
	
    /* Main loop: read packet and take action */
    while (TRUE) {

	/* read the next packet */
	rbuf = (char *) eread();

	/* since rbuf changes each time, so do these... */
	epktin = (struct epacket *) rbuf;
	ipptr = (struct ip *) epktin->ep_data;
	udpptr = (struct udp *) ipptr->i_data;
	prtftp = (struct tftp *) udpptr->u_data;
	    
	if ( net2hs(epktin->ep_hdr.e_ptype) == EP_ARP ) {
	    HandleArp(epktin);
	    continue;
	}
	    
	/* To be interesting, it must be:
	 *      1) IP type
	 *      2) UDP sub-type
	 *      3) Sent directly to me (not broadcast)
	 *      4) Sent to my UDP port
	 *      5) From the correct TFTP server
	 *      6) The block number I expect
	 */
	    
	if ((net2hs(epktin->ep_hdr.e_ptype) == EP_IP) &&
	    (ipptr->i_proto == IPRO_UDP) &&
	    (blkequ(ipptr->i_dest, myaddr, IPLEN)) &&
	    (net2hs(udpptr->u_dport) == MY_UDP_PORT) &&
	    (blkequ(destaddr,ipptr->i_src, IPLEN)) &&
	    (nextblock == net2hs(prtftp->info.block))) {

	    /* at this point, it's a tftp packet for me */
	    
	    /* check for tftp error packet */
	    if (net2hs(prtftp->op) == TERROR) {
		kprintf("TFTP error packet received\n");
		panic(prtftp->data.errmsg);
	    }

	    /* if it's not a data packet, I don't know what to do with it */
	    if (net2hs(prtftp->op) != TDATA)
		continue;


	    /* TFTP signals the end of the connection with a packet	*/
	    /* with less than 512 bytes					*/
	    if ((len=net2hs(udpptr->u_udplen)-UHLEN-TFTPHDR)<512) 
		lastpack = TRUE;

#ifdef DEBUG
	    kprintf(" len: %d  block: %d\n", len, net2hs(prtftp->info.block));
#endif

	    /* point to the first byte of the actual data */
	    from = (char *) prtftp->data.datastr;

	    /* if this is the FIRST packet, then we need to set up */
	    /* some things.  It 'could' be an a.out header	   */
	    if (net2hs(prtftp->info.block) == 1) {
		aouthdr = (struct exec *)prtftp->data.datastr;

		/* if the magic number is right, we'll assume */
		/* that it's an a.out header                  */
		if ((aouthdr->a_magic) == OMAGIC) {
		    /* we just want the text and data, not the bss */
		    imsize = (long) aouthdr->a_text +
			(long) aouthdr->a_data;

		    /* the data starts AFTER the a.out header */
		    from += sizeof(struct exec);
		    len -= sizeof(struct exec);
		    
		    /* fancy stuff for the ticks, since we know the	*/
		    /* number of packets to expect, we can scale the	*/
		    /* tick interval so that the line fits on an 80	*/
		    /* character screen.				*/
		    numpackets = imsize / 512;
		    tickinterval = 1 + (numpackets / 76);

		    /* draw an underlying bar, and then return the	*/
		    /* cursor to the left of the bar, just past the '|'	*/
		    kprintf("|");
		    for (k=0; k<(numpackets/tickinterval)+1; ++k)
			kprintf("-");
		    kprintf("|\r");
		}
		else {
		    /* no a.out header included, so we don't know how	*/
		    /* large the image will be.				*/
		    
		    imsize  = 0x0fffffff;   /* huge pos. number */
		    tickinterval = 2;	/* visual ack every other packet */
		}
#ifdef DEBUG
		kprintf("imsize: %d\n",imsize);
		kprintf("len: %d\n",len);
		kprintf("from: 0x%x\n",from);
		kprintf("to: 0x%x\n",to);
#endif
		
		kprintf("|");
	    }

	    /* we can speed up things by ACKing it now */
	    nextblock++;
	    makepack(wbuf, myeaddr, myaddr, MY_UDP_PORT,
		     epktin->ep_hdr.e_src, ipptr->i_src,
		     net2hs(udpptr->u_sport), packetid++);
	    tftpwr(TACK, net2hs(prtftp->info.block), 0, 0, wbuf);

	    /* visual ACK for every 'tickinterval'th packet */
	    if ((net2hs(prtftp->info.block)%tickinterval) == 0)
		kprintf("x");


	    /* adjust the expected image size */
	    imsize -= len;
	    bytesread += len;
	    if (imsize <= 0)
		lastpack = TRUE;
			
	    /* copy the image into memory */
	    while (len-- > 0) {
		*to++ = *from++;
	    }
			

	    if (lastpack) {
#ifdef DEBUG
		kprintf("ACKing last packet\n");
#endif
		kprintf("|  \n");
		/* send another quick ack */
		makepack(wbuf, myeaddr, myaddr, MY_UDP_PORT,
			 epktin->ep_hdr.e_src, ipptr->i_src,
			 net2hs(udpptr->u_sport), packetid++);
		tftpwr(TACK, net2hs(prtftp->info.block), 0, 0,
		       wbuf);

		kprintf("Downloaded %d bytes\n", bytesread);
		runprogram(LOADENTRY);
	    }
	}
    }
}
	    


/*
 * Jump to address 'startaddr'
 */
runprogram(startaddr)
     int (*startaddr)();
{
	startaddr();
	stopnow();
}



/*
 * use RARP to get MY address, as well as that of the TFTP server that
 *  I should use.  Sends 1 RARP packet, and reads NUM_RARP_RESPONSES looking
 *  for all the needed addresses.
 */
RarpForAddresses(wbuf)
    char	*wbuf;		/* buffer for ethernet write	*/
{
    struct	epacket	*epktin; /* pointer to Ethernet frame	*/
    struct	arppak	*apacptr; /* pointer to ARP packet	*/
    char	*rbuf;		/* buffer for ethernet read	*/
    int i;
    IPaddr	junk; /* needed for argument to mkarp; not ever used */

    /* make a RARP packet */
    mkarp(wbuf, EP_RARP, AR_RREQ, myeaddr, junk, junk);

    /* and send it */
    ewrite((struct epacket *) wbuf, EMINPAK);
#ifdef DEBUG	
    kprintf("Sent RARP packet\n");
#endif	
    
    i = 0;
    while ((i++) < NUM_RARP_RESPONSES) {
	rbuf = (char *) eread();
#ifdef DEBUG	
	kprintf("Recd packet while waiting for RARP reply\n");
#endif	
	epktin  = (struct epacket *) rbuf;
	apacptr = (struct arppak *) epktin->ep_data;
			
	if ( (net2hs(epktin->ep_hdr.e_ptype) == EP_RARP) && 
	     (net2hs(apacptr->ar_op) == AR_RRLY)         &&
	     (blkequ(apacptr->ar_tha, myeaddr, EPADLEN)) &&
	     (IsMyTftpServer(apacptr))) {
	        blkcopy(myaddr, apacptr->ar_tpa, IPLEN);
		blkcopy(destaddr, apacptr->ar_spa, IPLEN);
		blkcopy(desteaddr, apacptr->ar_sha, EPADLEN);
#ifdef DEBUG				
		PrintIPAddr("According to",destaddr);
		PrintIPAddr("        I am",myaddr);
#endif
		return(OK);
	}
    }
    return(SYSERR);
}



/*
 * Determine if the arp packet passed is from MY tftp server.  If it
 * is, return TRUE.  This can be tailored for a particular site using a
 * large switch statement or similar logic.
 */
IsMyTftpServer(apacptr)
    struct	arppak	*apacptr; /* pointer to ARP packet	*/
{
    /* In our system, the front end's IP */
    /* address is one less than the back */
    /* end's IP address */
    return ( ( 1 + *((int *) apacptr->ar_spa) ==
		   *((int *) apacptr->ar_tpa)));
}



/*
 * Handle an ARP packet by returning a response.
 */
HandleArp(epktin)
    struct	epacket	*epktin;	/* Ethernet frame	*/
{
    struct	arppak	*apacptr; /* pointer to ARP packet	*/

    apacptr = (struct arppak *) epktin->ep_data;
    if ( net2hs(apacptr->ar_op) == AR_REQ &&
	blkequ(myaddr, apacptr->ar_tpa, AR_PLEN)) {
	/* respond to arp request */
	blkcopy(epktin->ep_hdr.e_dest,
		epktin->ep_hdr.e_src, AR_HLEN);
	blkcopy(epktin->ep_hdr.e_src, myeaddr, AR_HLEN);
	apacptr->ar_op = hs2net(AR_RPLY);
	blkcopy(apacptr->ar_sha, myeaddr, AR_HLEN);
	blkcopy(apacptr->ar_spa, myaddr, AR_PLEN);
	blkcopy(apacptr->ar_tha, myeaddr, AR_HLEN);
	/* send ARP packet */
	ewrite(epktin, EMINPAK);
    }
}
