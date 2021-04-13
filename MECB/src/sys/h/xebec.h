/* xebec.h */

/* Xebec S1410 5.25 inch Winchester Disk Controller */

/*=============================================================================
 * The following is structure of the Xebec disk controller registers.
 *	You may notice that every other byte has been swapped.  This is due
 *	to the way the 68000 does byte transfers.  See the User's Manual for
 *	details. 
 *=============================================================================
 */
struct	xbdcb	{			/* Xebec disk controller regs.	*/
	char	xunit;			/* unit/high-order address bits	*/
	char	xop;			/* command class/opcode		*/
	char	xladdr;			/* low-order address bits	*/
	char	xmaddr;			/* middle-order address bits	*/
	char	xcntl;			/* control field		*/
	char	xcount;			/* block count			*/
	char	xres2;			/* reserved field #2		*/
	char	xres1;			/* reserved field #1		*/
};

/* Xebec controller operation codes */

#define	XOTDR	 0			/* Test Drive Ready		*/
#define	XORCAL	 1			/* Recalibrate			*/
#define	XONONO	 2			/* Reserved (is a no-no)	*/
#define	XORSS	 3			/* Request Sense Status		*/
#define	XOFMTD	 4			/* Format Drive			*/
#define	XOCTF	 5			/* Check Track Format		*/
#define	XOFMTT	 6			/* Format Track			*/
#define	XOFMTBT	 7			/* Format Bad Track		*/
#define	XOREAD	 8			/* Read				*/
#define XONO2	 9			/* Reserved (is also a no-no)	*/
#define	XOWRITE	10			/* Write			*/
#define	XOSEEK	11			/* Seek				*/
#define	XOINIT	12			/* Initialize Drive Character.	*/
#define	XOREBEL	13			/* Read ECC Burst Error Length	*/
#define	XORAMD	14			/* Perform RAM Diagnostic Test	*/

#define	XRETRY	0			/* retry control byte		*/
