/* lfile.h */

/* Local disk layout: disk block 0 is directory, then index area, and	*/
/* then data blocks.  Each disk block (512 bytes) in the index area	*/
/* contains 8 iblocks, which are 64 bytes long.  Iblocks are referenced	*/
/* relative to 0, so the disk block address of iblock k is given by	*/
/* truncate(k/8)+1.  The offset of iblock k within its disk block is	*/
/* given by 64*remainder(k,8).  The directory entry points to a linked	*/
/* list of iblocks, and each iblock contains pointers to IBLEN (29) data*/
/* blocks. Index pointers contain a valid data block address or DBNULL.	*/

#include <iblock.h>
#include <file.h>

struct	flblk	{			/* file "device" control block	*/
	int	fl_id;			/* file's "device id" in devtab	*/
	int	fl_dev;			/* file is on this disk device	*/
	int	fl_pid;			/* process id accessing the file*/
	struct	fdes	*fl_dent;	/* file's in-core dir. entry	*/
	int	fl_mode;		/* FLREAD, FLWRITE, or both	*/
	IBADDR	fl_iba;			/* address of iblock in fl_iblk	*/
	struct	iblk	fl_iblk;	/* current iblock for file	*/
	int	fl_ipnum;		/* current iptr in fl_iblk	*/
	long	fl_pos;			/* current file position (bytes)*/
	Bool	fl_dch;			/* has fl_buff been changed?	*/
	char	*fl_bptr;		/* ptr to next char in fl_buff	*/
	char	fl_buff[DBUFSIZ];	/* current data block for file	*/
};

#ifdef	Ndf
extern	struct	flblk	fltab[];
#endif
