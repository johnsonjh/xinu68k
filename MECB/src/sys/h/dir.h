/* dir.h */

#define	FDNLEN	10			/* length of file name + 1	*/
#define	NFDES	28			/* number of files per directory*/

struct	fdes	{			/* description of each file	*/
	long	fdlen;			/* length in bytes		*/
	IBADDR	fdiba;			/* first index block		*/
	char	fdname[FDNLEN];		/* zero-terminated file name	*/
};

struct	dir	{			/* directory layout		*/
	int	d_iblks;		/* i-blocks on this disk	*/
	DBADDR	d_fblst;		/* pointer to list of free blks	*/
	IBADDR	d_filst;		/* pointer to list of free iblks*/
	int	d_id;			/* disk identification integer	*/
	int	d_nfiles;		/* current number of files	*/
	struct	fdes	d_files[NFDES];	/* descriptions of the files	*/
};

struct	freeblk	{			/* shape of block on free list	*/
	DBADDR	fbnext;			/* address of next free block	*/
};
