/* dtc86.h */

/* Digital Technology Corp. DTC-86 disk controller host interface */

/*==============================================================================
 * The following is the structure for the dtc controller interface registers.
 *	You may notice that the bytes have been swapped.  This is due to the
 *	way the 68000 does byte transfers.  See the User's Manual for details.
 *==============================================================================
 */

struct	dtc	{			/* controller interface regs.	*/
	char	dt_ccsr;		/* command completion status	*/
	char	dt_csr;			/* control and status register	*/
	char	dt_darm;		/* 	dar middle		*/
	char	dt_darl;		/* data address register - low	*/
	char	dt_carm;		/* 	car middle		*/
	char	dt_carl;		/* command address register low */
	char	dt_carh;		/* car high			*/
	char	dt_darh;		/* dar high			*/
};

/* bits in the dtc csr register */

#define	DTGO	0x00			/* device start command		*/
#define	DTINTR	0x01			/* enable interface interrupt	*/
#define	DTDONE	0x80			/* command done			*/
#define	DTERROR	0x02			/* some error occurred		*/
#define	DTRESET	0x04			/* reset the interface board	*/

#define lbyte(value)	(char)(  ((long)value )       & (0xff))
#define mbyte(value)	(char)( (((long)value ) >>8)  & (0xff))
#define hbyte(value)	(char)( (((long)value ) >>16) & (0xff))
