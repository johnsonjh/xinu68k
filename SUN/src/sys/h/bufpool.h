/* bufpool.h */

#ifndef	NBPOOLS
#define	NBPOOLS	5			/* Maximum number of pools	*/
#endif
#ifndef	BPMAXB
#define	BPMAXB	2048			/* Maximum buffer length	*/
#endif
#define	BPMINB	2			/* Minimum buffer length	*/
#ifndef	BPMAXN
#define	BPMAXN	100			/* Maximum buffers in any pool	*/
#endif
struct	bpool	{			/* Description of a single pool	*/
	int	bpsize;			/* size of buffers in this pool	*/
	char	*bpnext;		/* pointer to next free buffer	*/
	int	bpsem;			/* semaphore that counts buffers*/
	};				/*  currently in THIS pool	*/

extern	struct	bpool bptab[];		/* Buffer pool table		*/
extern	int	nbpools;		/* current number of pools	*/
#ifdef	MEMMARK
extern	MARKER	bpmark;
#endif
