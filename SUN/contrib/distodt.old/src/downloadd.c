/* 
 * downloadd.c - Downloader Daemon - copies a.out file over network
 *               to /tftpboot/a.out
 * 
 * Author:	Jim Griffioen/Steve Chapin/Shawn Oosterman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Sun Feb 21 23:15:19 1988
 *
 * Copyright (c) 1988 Jim Griffioen/Steve Chapin/Shawn Oosterman
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
#include <setjmp.h>
#include <errno.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"
#include "../h/downloadd.h"
#include "../h/bedata.h"

#define SECS_TO_CONNECT 15

jmp_buf env;		/* used to return from read time-outs	*/
int errno;	        /* error number when system err		*/

/*---------------------------------------------------------------------------
 * downloadd - Download daemon forked off by the bed process
 *---------------------------------------------------------------------------
 */
downloadd(psa, sock, beconfig)
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
    int socktimeout();


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

    signal(SIGALRM, socktimeout);

    localport = 0;
    fdTcp = ListenTcp(&localport);

    if (opt.verbose) printf("using TCP port %d\n", localport);
    sprintf(mesg,"%d : local TCP port",localport);
    BedReply (psa, RESP_OK, mesg);
    close(sock);

    settimer(SECS_TO_CONNECT);
    ns = accept(fdTcp,&sa,&addrlen);
    canceltimer();

    sprintf(mesg,"Using %s%c%c",dev,'\015','\012');
    
    write(ns,mesg, strlen(mesg)+1);

    getaout(ns);

    if (opt.verbose) printf("downloader daemon exiting\n");

    exit(0);
}

getaout(ns)
int ns;				/* new TCP socket we are using   */
{
    int status;			/* ret status of sys calls       */
    int fd;			/* file descriptor of a.out file */
    char buff[BIGBUFSIZE];	/* buffer to read data into      */
    long aoutsize;		/* size fo the a.out file        */
    int len;			/* number of bytes read in       */


    /* Open /tftpboot/a.out for writing */
    if (opt.verbose) printf("removing /tftpboot/a.out\n");
    if (status=unlink("/tftpboot/a.out") != 0) {
	Log("Error removing /tftpboot/a.out");
    }
    if (opt.verbose) printf("recreating /tftpboot/a.out\n");
    if ((fd=open("/tftpboot/a.out", O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
	Log("Error opening /tftpboot/a.out");
	exit(1);
    }

    /* Get length of file being sent */
    if (sockgetstr(ns, buff, STIMEOUT) == NULL) {
	Log("Error reading during downloading");
	exit(1);
    }

    if (opt.verbose) printf("receiving a.out file\n");
    /* Read the file from the TCP connection and write into /tftpboot/a.out */
    aoutsize = atol(buff);
    while (aoutsize > 0) {
	if ((len=read(ns, buff, sizeof(buff))) == (-1)) {
	    Log("error reading a.out file over TCP connections");
	    exit(1);
	}
	if (write(fd, buff, len) == (-1)) {
	    Log("error writing to /tftpboot/a.out file");
	    exit(1);
	}
	aoutsize -= len;
    }

    if (opt.verbose) printf("a.out file transfer complete\n");
    /* Close the file descriptors before exiting */
    close(fd);
    close(ns);

    if (opt.verbose) printf("closing off file descriptors\n");
}


    
socktimeout()
{
    Log("DOWNLOADD: Connect (or Read) timeout - client not responding");
    exit(1);
}




/*
 *---------------------------------------------------------------------------
 * sockgetstr(sock, str, timeout) - get str from tcp socket-ret NULL if error
 *---------------------------------------------------------------------------
 */
char *sockgetstr(sock, str, timeout)
int sock;				/* socket descriptor handle	*/
char *str;
int timeout;
{
	char *sb;


	settimer(timeout);
	if (setjmp(env) == EINTR) {
		canceltimer();
		return(NULL);
	}
	sb = str;
	if (recv(sock, sb, 1, 0) < 1) {
		canceltimer();
		return(NULL);
	}
	while (*sb++ != '\0') {
		if (recv(sock, sb, 1, 0) <1) {
			canceltimer();
			return(NULL);
		}
	}
	canceltimer();
	return(str);
}



