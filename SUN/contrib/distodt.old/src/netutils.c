
/* 
 * netutils.c - Handy routines for dealing with the network
 * 
 * Author:	Tim Korb/Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 21:32:25 1988
 *
 * Copyright (c) 1988 Tim Korb/Shawn Oostermann/Jim Griffioen
 */

# include <stdio.h>
# include <ctype.h>
# include <sys/time.h>
# include <assert.h>
# include <errno.h>

# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

#include "../h/bed.h"

extern int errno;

# include "../h/utils.h"
# include "../h/netutils.h"

/*
 *---------------------------------------------------------------------------
 * GetNetName (ia) -- get name of network corresponding to network address ia.
 *---------------------------------------------------------------------------
 */
char *GetNetName (ia)
struct in_addr ia;
{
    struct netent *pne;

    if ((pne = getnetbyaddr (inet_netof (ia))) == NULL)
        SysError ("getnetbyaddr");
    return pne->n_name;
}

/*
 *---------------------------------------------------------------------------
 * ConnectUdp (sbHost, port) -- create a udp socket to destination
 * port on sbHost. Connect the socket to the port.
 *---------------------------------------------------------------------------
 */
int ConnectUdp (sbHost, port)
char *sbHost;
int port;
{
    int fd;
    struct sockaddr_in sa;	/* sa = socket address */
    struct hostent *phe;	/* he = host entry */
    struct in_addr na;		/* na = network address */
    struct servent *pse;	/* se = service entry */

    if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
        SysError ("socket");

    if ((phe = gethostbyname (sbHost)) == NULL)
        Error ("unknown host: %s", sbHost);

    na = *( (struct in_addr *) (phe->h_addr));

    sa.sin_port = htons(port);

    sa.sin_family = AF_INET;
    sa.sin_addr = na;
    if (connect (fd, &sa, lsa) == -1) {
        if (errno == ECONNREFUSED)
	    Error ("No process at port %d on host '%s'.\n", port, sbHost);
	else if (errno == ENETUNREACH)
	    Error ("Can't get to network %s from here.\n", GetNetName (sa.sin_addr));
        else
	    SysError ("connect");
	}
    return fd;
}

/*
 *---------------------------------------------------------------------------
 * FdToRemoteSa (fd) -- return psa of remote socket associated with fd.
 *---------------------------------------------------------------------------
 */
struct sockaddr_in *FdToRemoteSa (fd)
int fd;
{
    static struct sockaddr_in sa;
    int lsaT = lsa;

    if (getpeername (fd, &sa, &lsaT) == -1)
        SysError ("getpeername");
    assert (lsaT == lsa);
    assert (sa.sin_family == AF_INET);

    return &sa;
}

/*
 *---------------------------------------------------------------------------
 * FdToLocalSa (fd) -- return psa of local socket associated with fd.
 *---------------------------------------------------------------------------
 */
struct sockaddr_in *FdToLocalSa (fd)
int fd;
{
    static struct sockaddr_in sa;
    int lsaT = lsa;

    if (getsockname (fd, &sa, &lsaT) == -1)
        SysError ("getsockname");
    assert (lsaT == lsa);
    assert (sa.sin_family == AF_INET);

    {	/* since getsockname doesn't fill in our internet address... */
	char sb[100];
	struct hostent *phe;

	if (gethostname (sb, sizeof (sb)) != 0)
	    SysError ("gethostname");
	if ((phe = gethostbyname (sb)) == NULL)
	    SysError ("gethostbyname");
	sa.sin_addr = *(struct in_addr *) phe->h_addr;
    }

    return &sa;
}

/*
 *---------------------------------------------------------------------------
 * IaToSb (ia) -- convert internet address to string representation.
 *---------------------------------------------------------------------------
 */
char *IaToSb (ia)
struct in_addr ia;
{
    struct hostent *phe;
    static char sb[100];

# define iMax 100
    static char *rgsb[iMax];		/* cache */
    static struct in_addr rgia[iMax];
    static int iMac = 0;
    int i;

/* fprintf (stderr, "iatosb: %d (sb: '%s') ...\n", ia.s_addr, sb); fflush (stderr); */

    if (ia.s_addr == 0)
        return "0";

    for (i = 0; i < iMac; i++)
        if (rgia[i].s_addr == ia.s_addr)
	    return rgsb[i];

    phe = gethostbyaddr (&ia.s_addr, sizeof (ia.s_addr), AF_INET);
    if (phe == NULL)
        strcpy (sb, inet_ntoa (ia.s_addr));
    else {
        char *pch = (char *) index (phe->h_name, '-');
    	if (pch == NULL)	/* omit prefix on name if any */
            pch = phe->h_name;
    	else
            pch++;
        strcpy (sb, pch);
	}
    
/* fprintf (stderr, "sb: '%s'\n", sb); fflush (stderr); */

    if (iMac >= iMax) {
        Log ("IaToSb CACHE FULL");
	return sb;
	}
    else {
        rgsb[iMac] = (char *) strcpy (malloc (strlen (sb)+1), sb);
        rgia[iMac] = ia;
        return rgsb[iMac++];
	}
}

/*
 *---------------------------------------------------------------------------
 * ConnectTcp (sbHost, port) -- open a tcp connection to host sbHost at
 * the port named port. Return a two-way file descriptor to that socket.
 *---------------------------------------------------------------------------
 */
int ConnectTcp (sbHost, port)
char *sbHost;
int port;
{
    int fd;
    struct sockaddr_in sa;	/* sa = socket address */
    struct hostent *phe;	/* he = host entry */
    struct servent *pse;	/* se = service entry */

    if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
        SysError ("socket");

    sa.sin_family = AF_INET;

    if ((phe = gethostbyname (sbHost)) == NULL)
        Error ("unknown host: %s\n", sbHost);
    assert (phe->h_length == 4);
    sa.sin_addr = *( (struct in_addr *) (phe->h_addr));

    sa.sin_port = htons (port);

    if (connect (fd, &sa, sizeof sa) == -1) {
        if (errno == ECONNREFUSED)
	    Error ("No process is listening on port %d at %s.", port, sbHost);
	else if (errno == ENETUNREACH)
	    Error ("Can't get to network %s from here.\n", GetNetName (sa.sin_addr));
        else
	    SysError ("connect");
	}
    return fd;
}

/*
 *---------------------------------------------------------------------------
 * ListenTcp () -- create and return an fd that will accept TCP connections.
 *---------------------------------------------------------------------------
 */
ListenTcp (pport)
     u_short *pport;
{
    int fd;
    struct sockaddr_in sa;
    int len;

    if ((fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
        SysError ("socket");
    sa.sin_family = AF_INET;
    sa.sin_port = htons(*pport);
    sa.sin_addr.s_addr = INADDR_ANY;	/* accept connection from all */

    if (bind (fd, &sa, lsa) == -1)
        SysError ("bind");

    len = sizeof (sa);
    if (getsockname(fd, &sa, &len) < 0) {
      SysError("listenTcp: getsockname");
    }
    
    if (listen (fd, SOMAXCONN) == -1)	/* set up for listening */
        SysError ("listen");

    *pport = ntohs(sa.sin_port);
    return fd;
}

/*
 *---------------------------------------------------------------------------
 * ListenUdp (port) -- create and return an fd that will receive
 * a datagram on port named by port
 *---------------------------------------------------------------------------
 */
ListenUdp (port)
     int port;
{
    int fd;
    struct sockaddr_in sa;
    struct servent *pse;

    if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
        SysError ("socket");

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = INADDR_ANY;

    if (bind (fd, &sa, lsa) == -1) {
        Log ("%d already running", port);
	exit (REALLYDIE);
	}
    return fd;
}

