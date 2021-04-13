/* mem.h - freestk, roundew, truncew */

/*----------------------------------------------------------------------
 * roundew, truncew - round or trunctate address to next even word
 *----------------------------------------------------------------------
 */
#define	roundew(x)	(WORD *)( (3 + (WORD)(x)) & ~03 )
#define	truncew(x)	(WORD *)( ((WORD)(x)) & ~03 )


/*----------------------------------------------------------------------
 * roundmb, truncmb -- round or truncate address up to size of mblock
 *----------------------------------------------------------------------
 */
#define	roundmb(x)	(WORD *)( (7 + (WORD)(x)) & ~07 )
#define	truncmb(x)	(WORD *)( ((WORD)(x)) & ~07 )


/*----------------------------------------------------------------------
 *  freestk  --  free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define freestk(p,len)	freemem((unsigned)(p)			\
				- (unsigned)(roundmb(len))	\
				+ (unsigned)sizeof(int),	\
				roundmb(len) )

struct	mblock	{
	struct	mblock	*mnext;
	unsigned int	mlen;
	};
extern	struct	mblock	memlist;	/* head of free memory list	*/
extern	WORD	*maxaddr;		/* max memory address		*/
extern	WORD	_end;			/* address beyond loaded memory	*/
extern	WORD	*end;			/* &_end + FILLSIZE		*/
