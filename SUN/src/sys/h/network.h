/* network.h */

/* All includes needed for the network */

#include <lereg.h>    
#include <ether.h>
#include <ip.h>
#include <icmp.h>
#include <udp.h>
#include <net.h>
#include <dgram.h>
#include <arp.h>
#include <fserver.h>
#include <rfile.h>
#include <domain.h>

/* Declarations data conversion and checksum routines */

extern	short		cksum();	/* 1s comp of 16-bit 2s comp sum*/

/* for talking to the Vaxen */
#define h2vax(x) (unsigned) ((unsigned) ((x)>>8) + (unsigned)((x)<<8))
#define vax2h(x) (unsigned) ((unsigned) ((x)>>8) + (unsigned)((x)<<8))

/* The Sun uses network byte order already!! */
#define hs2net(x) (x)
#define net2hs(x) (x)
#define hl2net(x) (x)
#define net2hl(x) (x)

/* network macros */
#define hi8(x)   (unsigned char)  (((long) (x) >> 16) & 0x00ff)
#define low16(x) (unsigned short) ((long) (x) & 0xffff)



