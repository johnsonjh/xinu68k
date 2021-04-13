/* mem.h - freestk, roundew, truncew */

/*----------------------------------------------------------------------
 * roundew, truncew - round or trunctate address to next even word
 *----------------------------------------------------------------------
 */
#define	roundew(x)	(WORD *)( (3 + (WORD)(x)) & ~03 )
#define	truncew(x)	(WORD *)( ((WORD)(x)) & ~03 )

/*----------------------------------------------------------------------
 *  freestk  --  free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define freestk(p,len)	freemem((unsigned)(p)			\
				- (unsigned)(roundew(len))	\
				+ (unsigned)sizeof(int),	\
				roundew(len) )

struct	mblock	{
	struct	mblock	*mnext;
	unsigned int	mlen;
	};
extern	struct	mblock	memlist;	/* head of free memory list	*/
extern	WORD	*maxaddr;		/* max memory address		*/
extern	WORD	end;			/* address beyond loaded memory	*/

