/* ttyinit.c - ttyinit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <tty.h>
#include <io.h>
#include <slu.h>    
#include <zsreg.h>

/*------------------------------------------------------------------------
 *  ttyinit - initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
ttyinit(devptr)
	struct	devsw	*devptr;
{
	register struct	tty *iptr;
	register struct	csr *cptr;
	int	 junk, isconsole;
	int Asm_ttyint();

	/* set up interrupt vector and interrupt dispatch table */
	set_evec( (int) SVECTOR, Asm_ttyint);

	/* set up tty parameters */
	iptr = &tty[devptr->dvminor];
	devptr->dvioblk = (char *) iptr;	/* fill tty control blk	*/
	isconsole = (devptr->dvnum == CONSOLE);	/* make console cooked	*/
	iptr->ioaddr = (struct zscc_device *) devptr->dvcsr;
						/* copy in chip address	*/
	iptr->ihead = iptr->itail = 0;		/* empty input queue	*/
	iptr->isem = screate(0);		/* chars. read so far=0	*/
	iptr->icnt = 0;				/* "			*/
	iptr->osem = screate(OBUFLEN);		/* buffer available=all	*/
	iptr->ocnt = 0;				/* "			*/
	iptr->odsend = 0;			/* sends delayed so far	*/
	iptr->ohead = iptr->otail = 0;		/* output queue empty	*/
	iptr->ehead = iptr->etail = 0;		/* echo queue empty	*/
	iptr->imode = (isconsole ? IMCOOKED : IMRAW);
	iptr->iecho = iptr->evis = isconsole;	/* echo console input	*/
	iptr->ierase = iptr->ieback = isconsole;/* console honors erase	*/
	iptr->ierasec = BACKSP;			/*  using ^h		*/
	iptr->ecrlf = iptr->icrlf = isconsole;	/* map RETURN on input	*/
	iptr->ocrlf = iptr->oflow = isconsole;
	iptr->ieof  = iptr->ikill = isconsole;	/* set line kill == @	*/
	iptr->iintr = FALSE;
	iptr->iintrc = INTRCH;
	iptr->iintpid = BADPID;
	iptr->ikillc = KILLCH;
	iptr->ieofc = EOFC;
	iptr->oheld = FALSE;
	iptr->ostart = STRTCH;
	iptr->ostop = STOPCH;
	iptr->icursor = 0;
	iptr->ifullc = TFULLC;

	/* set up the chip */
	initSerial(devptr->dvcsr);
}



/*
 *	UART initialization sequence.  This is written to both 
 *	halves of the UART in a little loop.
 */
unsigned char	uart_init[] = {
	/* Set up all the elements on the chip: */
    	3,	0,			/* disable receives */
	0,	ZSWR0_RESET_STATUS,	/* Reset ext status int */
	0,	ZSWR0_RESET_ERRORS,	/* (sdo) Reset read errors */
	2,	EVEC_LEVEL6,		/* (sdo) Int vector = level 6 autovec */
	15,	ZSR15_BREAK,		/* (sdo) interrupt on break */
	1,	ZSWR1_SIE|		/* (sdo) turn on interrupts */
	    	ZSWR1_TIE|
		ZSWR1_RIE,
	4,	ZSWR4_PARITY_EVEN|	/* Async mode, etc, etc, etc */
		ZSWR4_1_STOP|
		ZSWR4_X16_CLK,
	3,	ZSWR3_RX_8|
	    	ZSWR3_RX_ENABLE,	/* 8 char recvs, enable recvs */
	5,	ZSWR5_RTS|
		ZSWR5_TX_8|
		ZSWR5_TX_ENABLE|
		ZSWR5_DTR,
	11,	ZSWR11_TXCLK_BAUD|
		ZSWR11_RXCLK_BAUD,
	9,	ZSWR9_MASTER_IE,	/* (sdo) master interrupt enable */

	12,	ZSTimeConst(ZSCC_PCLK, 9600),	/* Default baud rate */
	13,	(ZSTimeConst(ZSCC_PCLK, 9600))/256,	/* Ditto, high order */
	14,	ZSWR14_BAUD_FROM_PCLK|
	    	ZSWR14_BAUD_ENA,
};


initSerial(addrc)
	register unsigned char *addrc;
{
	register unsigned char *p = uart_init;

#ifdef DEBUG
	kprintf("initserial called for address 0x%lx\n",(unsigned long) addrc);
#endif	

	for (; p < &uart_init[sizeof(uart_init)] ;) {
		DELAY(2);
		*addrc = *p++;
		DELAY(2);
	}
}
