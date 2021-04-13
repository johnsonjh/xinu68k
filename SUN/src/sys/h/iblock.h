/* iblock.h - ibtodb, ibdisp */

typedef	int		IBADDR;		/* iblocks addressed 0,1,2,...	*/

#define	IBLEN		29		/* # d-block ptrs in an i-block	*/
#define	IBNULL		-1		/* null pointer in i-block list	*/
#define	IBAREA		1		/* start if iblocks on disk	*/
#define	IBWDIR		TRUE		/* ibnew: write directory	*/
#define	IBNWDIR		FALSE		/* ibnew: don't write directory	*/

struct	iblk		{		/* index block layout		*/
	long		ib_byte;	/* first data byte indexed by	*/
					/*  this index block		*/
	IBADDR		ib_next;	/* address of next i-block	*/
	DBADDR		ib_dba[IBLEN];	/* ptrs to data blocks indexed	*/
};

#define	ibtodb(ib)	(((ib)>>3)+IBAREA)/* iblock to disk block addr.	*/
#define	ibdisp(ib)	(((ib)&07)*sizeof(struct iblk))
