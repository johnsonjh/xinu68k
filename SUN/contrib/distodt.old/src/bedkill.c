
/* 
 * bedkill.c - This program sends out a kill request to all backend daemons
 * 
 * Author:	Jim Griffioen/Shawn Oostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Jun  6 20:36:46 1988
 *
 * Copyright (c) 1988 Jim Griffioen/Shawn Oostermann
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>

#include "../h/utils.h"
#include "../h/bed.h"
#include "../h/bmutils.h"
#include "../h/netutils.h"
#include "../h/status.h"

jmp_buf env;			/* used to return from read time-outs	*/
int errno;		        /* error number when system err		*/
struct option opt;			/* command line options		*/


/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main (csb, rgsb)
int csb;
char *rgsb[];
{

	if (csb > 1) {
		fprintf(stderr, "usage: bedkill\n");
		exit(1);
	}
	killall();
	exit (0);
}


/*----------------------------------------------------------------------
 * killall - send a quit message to all back end daemons
 *----------------------------------------------------------------------
 */
killall()
{
	int fd_sock;
	int status;
	struct sockaddr_in sa;
	struct sockaddr_in sa_to;
	int salen;
	struct bm bm;
	int i;
	int len;
	int on = 1;			/* set socket option broadcast on*/
	
	initsigs();			/* initialize interrupts	*/
	
	/***** set up broadcast connection *****/
	fd_sock = socket(AF_INET,SOCK_DGRAM,0);

#ifdef VAX	
	/* set socket options so we can broadcast the IP address back */
	if (setsockopt(fd_sock,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on)) < 0) {
		fprintf(stderr, "not able to broadcast\n");
		exit(1);
	}
#endif	
	
	sa.sin_family = AF_INET;
	sa.sin_port   = htons(0);
	sa.sin_addr.s_addr = INADDR_ANY;
	status = bind(fd_sock, &sa, sizeof (struct sockaddr_in));
	
	sa_to.sin_family = AF_INET;
	sa_to.sin_port   = htons(KNOWN_UDP_PORT);
#ifdef VAX				/* VAX local net broadcast	*/
	sa_to.sin_addr.s_addr = htonl(INADDR_BROADCAST);
#else					/* SUN local net broadcast	*/
	sa_to.sin_addr.s_addr = htonl(BROADCAST);
#endif
	/***** broadcasting is now set up *****/
	
	bm.ver = CURVER;
	sprintf(bm.cmd, "%d", REQ_QUIT);
	strncpy (bm.detail, "detail", MAXDETAIL);
	
	status = sendto(fd_sock, (char *) &bm, lbm, 0,
			&sa_to, sizeof(struct sockaddr_in));
	if (status < 0) {
		perror("sendto");
		Error("sendto(): error broadcasting kill request");
	}

	i = 0;
	while (TRUE) {
		settimer(XSTIMEOUT);
		/* eventually we will timeout and jump here */
		if (setjmp(env) == EINTR) {
			canceltimer();
			printf("Kill backend daemon on %d machines\n", i);
			return(i);	/* return number of machines	*/
		}
		salen = sizeof(struct sockaddr_in);
		len = recvfrom(fd_sock, &bm, lbm, 0, &sa, &salen);
		if (len<=0) {
			canceltimer();
			perror("recv");
			Error("recv(): response code %d\n",len);
		}
		canceltimer();
		printf("Killed bed on machine '%s'\n", bm.detail);
		i++;
	}
}


/*
 *---------------------------------------------------------------------------
 * initsigs() - initialize interrupts
 *---------------------------------------------------------------------------
 */
initsigs()
{
	int alarmhandler(), inthandler();

	if ( signal(SIGINT,SIG_IGN) != SIG_IGN )
		signal(SIGINT,inthandler);
	if ( signal(SIGTERM,SIG_IGN) != SIG_IGN )
		signal(SIGTERM,inthandler);
	if ( signal(SIGQUIT,SIG_IGN) != SIG_IGN )
		signal(SIGQUIT,inthandler);

	signal(SIGALRM, alarmhandler);
}



/*
 *---------------------------------------------------------------------------
 * alarmhandler - return from alarm calls
 *---------------------------------------------------------------------------
 */
alarmhandler()
{
	signal(SIGALRM, alarmhandler);
	longjmp(env, EINTR);
	/* not reached */
}

/*
 *---------------------------------------------------------------------------
 * inthandler -- interrupt processing
 *---------------------------------------------------------------------------
 */
inthandler()
{
	exit(2);
}

