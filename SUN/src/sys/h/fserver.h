/* fserver.h */

#ifndef	RSERVER
#define	RSERVER		"128.10.2.3:2001"/* remote file server address	*/
#endif

/* Definitions of remote file server constants and packet format */

/* Codes for operations */

#define	FS_ERROR	-1		/* returned for error on op.	*/
#define	FS_OPEN		0		/* open a file			*/
#define	FS_CLOSE	1		/* close a file			*/
#define	FS_READ		2		/* read data from a file	*/
#define	FS_WRITE	3		/* write data to a file		*/
#define	FS_UNLINK	4		/* unlink a file from directory	*/
#define	FS_RENAME	5		/* change the name of file (data*/
					/*  area in packet is new name)	*/
#define	FS_MKDIR	6		/* make a directory (count=mode)*/
#define	FS_RMDIR	7		/* remove a directory		*/
#define	FS_ACCESS	8		/* determine file accessibility	*/
#define	FS_TRANS	9		/* other transaction		*/

#define	RDATLEN		512		/* Maximum bytes transferred	*/
#define	RNAMLEN		80		/* Maximum bytes in file name	*/
#define	RCLOSED		-1		/* server device not opened	*/

struct	fphdr	{			/* Format of server packet hdr	*/
	long	f_pos;			/* byte position in file	*/
	short	f_count;		/* count of data bytes		*/
	short	f_op;			/* Operation requested		*/
	char	f_name[RNAMLEN];	/* Name of remote file		*/
};


struct	fpacket	{			/* Format of packet to/from rfs	*/
	struct	fphdr	fp_h;		/* packet header		*/
	char	fpdata[RDATLEN];	/* data sent to/from server	*/
};

#define	FPHLEN	sizeof(struct fphdr)	/* size of server packet header	*/
#define	FPPLEN	sizeof(struct fpacket)	/* size of server packet	*/
