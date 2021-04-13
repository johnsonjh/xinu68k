/* tftpload.h - header file for tftpload programs */

/* Symbolic constants used throughout */

typedef	long		WORD;		/* maximum of (int, char *)	*/
typedef	char		Bool;		/* Boolean type			*/
#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
#define	NULL		(char *)0	/* Null pointer for linked lists*/
#define	NULLCH		(char)0		/* The null character		*/
#define	NULLSTR		""		/* Pointer to empty string	*/
#define	LOWBYTE		0377		/* mask for low-order 8 bits	*/
#define	HIBYTE		0177400		/* mask for high 8 of 16 bits	*/
#define	LOW16		0177777		/* mask for low-order 16 bits	*/

/* Universal return constants */

#define	OK		 1		/* system call ok		*/
#define	SYSERR		-1		/* system call failed		*/
#define	EOF		-2		/* End-of-file			*/

/* Miscellaneous utility inline functions */

#define	isodd(x)	(01&(int)(x))


#define MY_UDP_PORT 2053		/* my udp port			*/
#define NUM_RARP_RESPONSES 10		/* number of RARP responses to	*/
					/* sift thru before giving up	*/


#define TFTPPORT	69		/* port tftp server is at usually */

/* important addresses */
#define LOADSTART	0x4000		/* address to start loading at	*/
#define LOADENTRY	0x4000		/* entry point for loaded image	*/

/* some misc symbols */
extern int end, edata, start, etext;
