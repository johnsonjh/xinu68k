
/* 
 * bmutils.c - Routines for sending and receiving bm structure (bed protocol)
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 20:53:41 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */

#include <stdio.h>
#include <pwd.h>
#include <ctype.h>
#include <strings.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../h/bed.h"
#include "../h/bmutils.h"


#define NULLCH '\00'    

/*---------------------------------------------------------------------------
 * bmsend - send a bm struct in a UDP packet
 *---------------------------------------------------------------------------
 */
bmsend(s, pbm, len, flags )
    int s;
    struct bm *pbm;
    int len, flags;
{
    return(send(s,(char *) pbm, len, flags));
}
     

/*---------------------------------------------------------------------------
 * bmsendto - send a bm struct in a UDP packet to a specific machine
 *---------------------------------------------------------------------------
 */
bmsendto(s, pbm, len, flags, to, tolen )
    int s;
    struct bm *pbm;
    int len, flags;
    struct sockaddr_in *to;
    int tolen;
{
    return(sendto(s,(char *) pbm,len,flags,to,tolen));
}
     

/*---------------------------------------------------------------------------
 * bmrecv - receive a bm structure in a UDP packet
 *---------------------------------------------------------------------------
 */
bmrecv(s, pbm, len, flags )
    int s;
    struct bm *pbm;
    int len, flags;
{
    static char bufrecv[MAXNETPACKET];
    int status;
    
    if ( (status = recv(s,bufrecv,MAXNETPACKET,flags)) <= 0)
      return(status);

    bcopy(bufrecv, (char *)pbm, sizeof(struct bm));

    return(sizeof(struct bm));
}
     


/*---------------------------------------------------------------------------
 * bmrecvfrom - receive a bm structure from a specified machine
 *---------------------------------------------------------------------------
 */
bmrecvfrom(s, pbm, len, flags, to, tolen)
    int s;
    struct bm *pbm;
    int len, flags;
    struct sockaddr_in *to;
    int tolen;
{
    static char bufrecv[MAXNETPACKET];
    int status;
    
    if ( (status = recvfrom(s,bufrecv,MAXNETPACKET,flags, to, tolen)) <= 0)
      return(status);

    bcopy(bufrecv, (char *) pbm, sizeof(struct bm));

    return(sizeof(struct bm));
}
