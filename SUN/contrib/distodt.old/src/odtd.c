
/* 
 * odtd.c - This is the odt daemon forked by the 'bed' process
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun  3 14:14:53 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */

#include <stdio.h>
#include <signal.h>
#include <sgtty.h>
#include <sys/file.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"
#include "../h/bedata.h"

#define BUFSIZE 100
#define SECS_TO_CONNECT 15
#define TOUCHTIME	30		/* touch lock file every 30 sec */

int	devfd = -1;			/* line tty file descriptor	*/
struct	sgttyb	Lttymode;		/* line tty mode upon entry	*/
struct	sgttyb	lttymode;		/* line tty mode for gtty/stty	*/

/*---------------------------------------------------------------------------
 * odtd - odtd daemon forked off by the BED process
 *---------------------------------------------------------------------------
 */
odtd(psa, sock, beconfig)
struct sockaddr_in *psa;
int sock;
struct odtdata *beconfig;
{
    int fdTcp;
    int len;
    int ns;
    struct sockaddr_in sa;
    u_short localport;
    char mesg[MAXDETAIL];
    int addrlen = sizeof(struct sockaddr_in);
    char dev[TTYLENGTH];
    char bemachine[MAXMACHINENAME];
    int baudrate;
    int ConTimeout();
    int Timeout();

    strcpy(dev, beconfig->sline);
    strcpy(bemachine, beconfig->bename);
    if (strcmp(beconfig->baud, "9600") == 0)
	baudrate = 9600;
    else if (strcmp(beconfig->baud, "2400") == 0)
	baudrate = 2400;
    else if (strcmp(beconfig->baud, "300") == 0)
	baudrate = 300;
    else if (strcmp(beconfig->baud, "4800") == 0)
	baudrate = 4800;

    
    signal(SIGALRM, ConTimeout);


    SetTtyLine(dev, baudrate);

    localport = 0;
    fdTcp = ListenTcp(&localport);
    
    sprintf(mesg,"%d : local TCP port",localport);
    BedReply (psa, RESP_OK, mesg);
    close(sock);			/* no longer need upd connection */
    alarm(SECS_TO_CONNECT);
    
    ns = accept(fdTcp,&sa,&addrlen);
    alarm(0);

    sprintf(mesg,"Using %s%c%c",dev,'\015','\012');
    
    write(ns,mesg, strlen(mesg));

    TCPxTTY(ns,devfd,bemachine);

    ioctl(devfd, TIOCSETP, &Lttymode);
    exit(0);
}



SetTtyLine(dev, baudrate)
char *dev;
int  baudrate;
{
    
    if ( (devfd = open(dev, O_RDWR, 0)) < 0) {
	Log ("SetTtyLine: Cannot open %s\n", dev);
    }

    if (ioctl(devfd, TIOCGETP, &Lttymode) < 0) {
	perror(dev);
	exit(1);
    } else {
	lttymode = Lttymode;
	lttymode.sg_flags |= RAW;
	lttymode.sg_flags &= ~ECHO;
	if ( TRUE /*tandem*/) 
	  lttymode.sg_flags |= TANDEM;
/*	
	if (baudrate != NOBAUD) {
	    Lttymode.sg_ospeed = Lttymode.sg_ispeed = baudrate;
	    lttymode.sg_ospeed = lttymode.sg_ispeed = baudrate;
	}
*/    
	if (ioctl(devfd, TIOCSETP, &lttymode) < 0) {
	    perror(dev);
	    exit(1);
	}
    }
}


TCPxTTY(fdTCP, fdTTY, bemachine)
     int fdTCP, fdTTY;
     char *bemachine;
{
    int bmaskRead;
    int bmaskResult;
    int zero = 0;
    int len;
    int idle;
    int esc;
    char buf[100];
    int i;
    long lasttime;
    long curtime;

    bmaskRead = BITMASK(fdTTY) | BITMASK(fdTCP);	
    lasttime = time(0);
    esc = 0;
    idle = 0;
    for (;;) {
	bmaskResult = bmaskRead;
	select(32, &bmaskResult, &zero, &zero, 0);

	curtime = time(0);
	if ((curtime-lasttime) > TOUCHTIME) {
		lasttime = curtime;
		touchlock(bemachine);
	}

	if ((bmaskResult & BITMASK(fdTCP)) != 0) {
	    len = read(fdTCP,buf,BUFSIZE);
	    if (len <= 0) {
		if (len != 0)
		  fprintf(stderr,"Coupling: bad return from fdTCP read: %d\n",len);
		return;
	    }
	    for (i=0; i<len; ++i) {
		idle = 0;
		if ( (esc==0) && (buf[i]=='\\') ) {
		    esc = 1;
		}
		else if ( (esc==1) && ((buf[i]&0177)=='\0') ) {
		    ioctl(fdTTY, TIOCSBRK, NULL);
		    usleep(10000);
		    ioctl(fdTTY, TIOCCBRK, NULL);
		} else {
		    if ( (write(fdTTY,buf+i,1)) <= 0) 
		      SysError("Coupling: bad return from fdTTY write");
		    esc = 0;
		}
	    }
	}
	if ((bmaskResult & BITMASK(fdTTY)) != 0) {
	    len = read(fdTTY,buf,BUFSIZE);
	    if (len <= 0) {
		if (len != 0)
		  fprintf(stderr,"Coupling: return from fdTTY read: %d\n",len);
		return;
	    }
	    if ( (len = write(fdTCP,buf,len)) <= 0)
	      SysError("Coupling: Bad return from fdTCP write");
	}
    }
}

ConTimeout()
{
    Log("ODTD: Connect timeout after %d seconds", SECS_TO_CONNECT);
    exit(0);
}
