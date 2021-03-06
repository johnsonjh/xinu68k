/*
 * sun3.c - utilities for uploading Sun3 memory using it's monitor
 *
 *    Shawn Ostermann, 10/10/87
 */

#include <signal.h>
#include <stdio.h>
#include <sgtty.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include "pm.h"

#define XINU_CONSOLE "/dev/ttya"
#define FOPEN_OPTIONS "r+"
#define BREAK_TIME 50

#define copy_sgtty(x,y) x.sg_ispeed = y.sg_ispeed; \
    		x.sg_ospeed = y.sg_ospeed; \
    		x.sg_kill = y.sg_kill; \
		x.sg_erase = y.sg_erase; \
		x.sg_flags = y.sg_flags;


int fdSun;
struct sgttyb mode, tty_mode;



/*
 * OpenSun -- open and check the connection to the Back End Sun
 */
OpenSun()
{
    int arg;
    char response[100];
    int cc;
    char class[32], fem[32], bem[32];

    /* these really should be options to pm */
    /* hard coded for the moment */
    strcpy(class, "SUN");
    strcpy(fem, "");
    strcpy(bem, "");
    
    fdSun = connectsun(class,fem,bem,1);


    if (verbose)
	printf("Sending break\n");

    /* make sure we have it's attention */
    write(fdSun, "\\\0", 2);		/* send a break character	*/
    usleep(500000);		/* 5/10 second */

    if (verbose)
	printf("Checking for monitor response\n");

    /* check for output */
    ioctl(fdSun, FIONREAD,&arg);
    if (arg == 0) {
	/* try again, but longer */
	write(fdSun, "\\\0", 2);	/* send a break character	*/
	usleep(1000000);		/* 1 second */
	ioctl(fdSun, FIONREAD,&arg);
	if (arg == 0) {
	    printf("FATAL: can't get monitor response\n");
	    return(-1);
	}
    }
    while (arg != 0) {
	cc = read(fdSun,response,100);
	ioctl(fdSun, FIONREAD,&arg);
    }
    if ( *(response+cc-1) != '>') {
	printf("FATAL: cannot get prompt from monitor\n");
	return(-1);
    }
    if (verbose)
	printf("Received monitor prompt\n");
    return(0);
}


/*
 * close the Sun connection, and reset ttya to it's normal state
 */
CloseSun()
{
    close(fdSun);
}



/*
 * return the integer at stack location "ptr"
 */
StackInt(ptr)
char *ptr;
{
    int retint;
    
    FillMem(ptr,ptr+3,&retint);
    return(retint);
}



/*
 *   Fill the memory pointed to by pcore with bytes from "first" to
 *      "last" from the Backend Sun.
 */
FillMem(first, last, pcore)
     register int first;
     int last;
     register unsigned char *pcore;
{
    int ret;
    int flong;

    flong = (last-first) > 0x1000;

    if (verbose && flong)
	printf("uploading sun memory from 0x%x to 0x%x\n",first,last);

    for (; first <= last; ++first, ++pcore) {
	if (verbose && flong)
	    if ( (first & 0x03ff) == 0)
		printf("0x%x\n",first);
	ret = readbyte(first);
	if (ret >= 0)
	    *pcore = ret;
	else
	    return(ret);
    }
    return(0);
}



/*
 * return the value of the single byte pointed to by "ptr" in
 *    the backend SUN memory.
 *
 *     uses several caches for efficiency
 *
 *   returns -1 on error
 */
#define CACHESIZE 256
#define NUMCACHES 5
#define BUFSIZE (CACHESIZE/16)*100
#define RETRIES 5  
struct cache {
    int	icache;
    unsigned char core[CACHESIZE];
} cache[NUMCACHES] = { {-1}, {-1}, {-1}};

int lastcache = 0;

readbyte(pc)
     int pc;
{
    char output[BUFSIZE];
    int ret;
    int r;
    int ca;

    /* see if it's in one of the caches */
    for (ca=0; ca<NUMCACHES; ++ca) {
	if ( (pc >= cache[ca].icache) && (pc < cache[ca].icache+CACHESIZE))
	    return((unsigned int) (0xff&cache[ca].core[pc-cache[ca].icache]));
    }

    /* else create a new cache */
    lastcache = (lastcache + 1) % NUMCACHES;
    ca = lastcache;
    cache[ca].icache = pc & ~(CACHESIZE-1);
    for (r=0; r<RETRIES; ++r) {
	ret = ReadMon(cache[ca].icache,CACHESIZE,output,BUFSIZE);
	if (ret == 0) {
	    ret = ParseLine(cache[ca].icache,CACHESIZE,output,cache[ca].core);
	}
	if (ret == 0)
	    return((unsigned int) (0xff & cache[ca].core[pc-cache[ca].icache]));
	if (verbose)
	    printf("RETRY %d\n",r+1);
    }
    return(-1);
}





/*
 * Read from the backend sun using it's monitor program.
 *    first is beginning address, buf is storage for answer
 *    numbytes must be a multiple of 16 for error recovery to work.
 */
ReadMon(first, numbytes, buf, bufsize)
     int first;
     int numbytes;
     char *buf;
     int bufsize;
{
    char command[80];
    char ch;
    int cc;
    char *pch;
    int size;
    int expected;
    
    sprintf(command,"v %x %x b\r", first, first+numbytes-1);
    write(fdSun, command, strlen(command));

    *buf = '\000';
    do {
	read(fdSun,buf,1);
    }
    while(*buf != '\n');	/* read over echo */

    pch = buf;
    size = bufsize;
    while (TRUE) {
	cc = read(fdSun,pch,size);
	pch += cc;
	size -= cc;
	if ( (*(pch-1) == '>') && (*(pch-2) == '\n')) {
	    break;
	}
    }

    /* check for missed data */
    /* 81 bytes per line, plus one for the cursor */
    expected = (numbytes/16)*81 + 1;
    if ( (bufsize-size) == expected) {
	return(0);
    }
    else {
	if (verbose)
	    printf("ERROR: read %d chars, expected %d\n",bufsize - size, expected);
	return(-1);
    }
}
	 

/*
 * turn the line(s) pointed to by psb into an array of 16 chars.
 *   lines are from the monitor program, and look like:
 * 00004000:  46 FC 27 00 41 FA FF FA 43 F9 00 00 40 00 B3 C8     F|'...
 * 00004010:  46 FC 27 00 41 FA FF FA 43 F9 00 00 40 00 B3 C8     F|'...
 *
 *  start is beginning address expected
 *  numbytes is number of bytes expected
 *  psb is pointer to text from monitor output to "v" command
 *  rgch is place to put the converted array of bytes
 *     (start must be a multiple of 16, as must numbytes)
 */
ParseLine(start, numbytes, psb, rgch)
     int start;
     int numbytes;
     char *psb;
     unsigned char rgch[];
{
    int ret;
    int hex[16];
    int readbegin;
    int i;
    int line;

    if ((start & 0x0f != 0) | (numbytes & 0x0f != 0)) {
	printf("illegal start or numbytes\n");
	return(-1);
    }

    for (line = 0; line < (numbytes / 16); ++line) {
	ret = sscanf(psb,"%x: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x",
		     &readbegin,
		     &hex[0], &hex[1], &hex[2], &hex[3], &hex[4], &hex[5],
		     &hex[6], &hex[7], &hex[8], &hex[9], &hex[10], &hex[11],
		     &hex[12], &hex[13], &hex[14], &hex[15]);
	if (ret != 17) {
	    printf("FATAL: unable to parse monitor output for address %x\n",
		   (start+(line*16)));
	    printf("   nargs read: %d, text found: \n%s\n",ret,psb);
	    return(-1);
	}
	if (readbegin != (start+(line*16))) {
	    printf("bad address (exp %x, found %x) in line: \n%s\n",
		   (start+(line*16)),readbegin,psb);
	    return(-1);
	}
	for (i=0; i<16; ++i) {
		rgch[i] = hex[i];
	}
	rgch += 16;
	while( *psb != '\n' && *psb != '\00')
	    ++psb;
	++psb;
    }
    return(0);
}

