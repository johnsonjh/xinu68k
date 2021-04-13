/* coninit.c -- coninit */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>    
#include <zsreg.h>

/*------------------------------------------------------------------------
 *  coninit -- initialize the physical Sun console
 *------------------------------------------------------------------------
 */
coninit(devptr)
	struct	devsw	*devptr;
{
	int Asm_conint();

	/* set up the chip */
	/* initSerial(0xfe00004); */
}



/*
 *	UART initialization sequence.  This is written to both 
 *	halves of the UART in a little loop.
 */
unsigned char	KBuart_init[] = {
	/* Set up all the elements on the chip: */
    	3,	0,			/* disable receives */
	0,	ZSWR0_RESET_STATUS,	/* Reset ext status int */
	0,	ZSWR0_RESET_ERRORS,	/* (sdo) Reset read errors */
	2,	EVEC_LEVEL6,		/* (sdo) Int vector = level 6 autovec */
	1,	ZSWR1_TIE|		/* (sdo) turn on interrupts */
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

	12,	ZSTimeConst(ZSCC_PCLK, 1200),	/* Default baud rate */
	13,	(ZSTimeConst(ZSCC_PCLK, 1200))/256,	/* Ditto, high order */
	14,	ZSWR14_BAUD_FROM_PCLK|
	    	ZSWR14_BAUD_ENA,
	0,	ZSWR0_RESET_STATUS,	/* Reset ext status int */
	0,	ZSWR0_RESET_STATUS,	/* Reset ext status int */
};


KBinitSerial(addrc)
	register unsigned char *addrc;
{
	register unsigned char *p = KBuart_init;

	kprintf("KBinitserial called for address 0x%lx\n",(unsigned long) addrc);

	for (; p < &KBuart_init[sizeof(KBuart_init)] ;) {
		DELAY(2);
		*addrc = *p++;
		DELAY(2);
	}
}


