/* network.h */

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

/*----------------------------------------------------------------------
 * roundew, truncew - round or trunctate address to next even word
 *----------------------------------------------------------------------
 */
#define	roundew(x)	(WORD *)( (3 + (WORD)(x)) & ~03 )
#define	truncew(x)	(WORD *)( ((WORD)(x)) & ~03 )


