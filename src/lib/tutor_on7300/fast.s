/*  ---------------------------------------------------------------------------
  fast.s  -  fast loader
 ---------------------------------------------------------------------------
*/
#define	RetVal		%d0
#define TempD1		%d1
#define	TempD2		%d2
#define	TempD3		%d3
#define	CheckSum	%d4
#define	PackLen		%d5
#define	TPackLen	%d6
#define BssLen		%d7
#define BaseAddr	%a1
#define BufAddr		%a2
#define TBufAddr	%a3
#define DBUGptr		%a4
#define DBUGaddr	0xa0000

/* ---------------------------------------------------------------------------
  fast  --  loads the user program into memory;
 ---------------------------------------------------------------------------
 edited by eoneil for att 7300 2/91
*/
#define UARTDATA 0xe50001	 /* 	Port A NEC7301 Uart Data */
#define UARTSTAT 0xe50005
#define RXFULL 0		/*  bit# for Receiver Full Bit */
#define TXEMPTY 2		/*  bit# for Transmitter Empty Bit*/

#define PACKSIZE 0x0400
#define SOH 0x2f			/* '/' */
#define ACK 0x79			/* 'y' */
#define NAK 0x6e			/* 'n' */
#define ESC 0x1b			/* ESCAPE */

text 0
global loadfile

loadfile:
        mov.l	&DBUGaddr, DBUGptr
        mov.b	&-1, (DBUGptr)+
	sub.l	&PACKSIZE,%sp		/* alloc packet buf on stack */
	mov.l	%sp, BufAddr
	subq.l	&4, %sp

getpack:
        mov.b	&1, (DBUGptr)+
	mov.b	&0x73, RetVal		/* print an 's' */ 
	bsr	locrawputc
	bsr	locrawgetc
	cmp.b	RetVal,&SOH		/* scan for Start 	(1 byte) */
	bne	getpack			/* not start of packet, search more */
        mov.b	&2, (DBUGptr)+
	clr.l	CheckSum		/* clear checksum value */

getbase:			 	/* get the base address  (4 bytes) */
	bsr	getlsum
	mov.l	RetVal,	 BaseAddr

getlen:					/* get the packet length (4 bytes) */
	bsr	getlsum
	mov.l	RetVal, PackLen

chkrange:				/* check the range on the length */
	cmp.l	PackLen,&PACKSIZE
	bgt	sendnak

copyvars:				/* copy length and address */
	mov.l	PackLen, TPackLen	
	beq	getsum
	mov.l	BufAddr, TBufAddr
getbuf:
	bsr	getcsum
	mov.b	RetVal, (TBufAddr)+
	subq.l	&1, TPackLen		/* original line */
	bne	getbuf

/*----------------------------------------------------------------------------
/* getsum - get transmitted  checksum, total should be 0
/*----------------------------------------------------------------------------
*/

getsum:
	bsr	locrawgetc
	mov.l	RetVal, TempD1
	asl.l	&8, TempD1
	bsr	locrawgetc
	or.l	RetVal, TempD1
	asl.l	&8, TempD1
	bsr	locrawgetc
	or.l	RetVal, TempD1
	asl.l	&8, TempD1
	bsr	locrawgetc
	or.l	RetVal, TempD1
	and.l	TempD1, CheckSum
	beq	copypack

/*----------------------------------------------------------------------------
/* sendnak - send a negative acknowledgement and retry
/*----------------------------------------------------------------------------
*/

sendnak:
        mov.b	&3, (DBUGptr)+
	mov.b	&NAK,	RetVal
	bsr	locrawputc
	bra	getpack

/*----------------------------------------------------------------------------
/* copypack - copy packet from buffer to memory
/*----------------------------------------------------------------------------
*/

copypack:
	mov.l	BufAddr, TBufAddr
	mov.l	PackLen, TPackLen
	beq	sendack
cmore:
	mov.b	(TBufAddr)+, (BaseAddr)+
	subq.l	&1, TPackLen
	bne	cmore

/*----------------------------------------------------------------------------
/* sendack - send positive acknowledgement
/*----------------------------------------------------------------------------
*/

sendack:
        mov.b	&4, (DBUGptr)+
	mov.b	&ACK, RetVal
	bsr	locrawputc
	tst.l	PackLen
	bne	getpack
	bra	monitor  /* EOT indicated by len 0 pkt */


/*----------------------------------------------------------------------------
/*  locrawputc  --  put a character on the line
/*----------------------------------------------------------------------------
*/

locrawputc:
	btst	&TXEMPTY, UARTSTAT
	beq	locrawputc
	mov.b	RetVal, UARTDATA
debug1:
	btst	&TXEMPTY, UARTSTAT
	beq	debug1
	rts

/*---------------------------------------------------------------------------
/*  locrawgetc  --  get a character from the line
/*---------------------------------------------------------------------------
*/

locrawgetc:
	clr.l	RetVal
check1:
	btst	&RXFULL, UARTSTAT
	beq	check1
	mov.b	UARTDATA, RetVal
	rts

/*----------------------------------------------------------------------------
/*  getcsum  --  get a character (1 byte)  and add to checksum
/*----------------------------------------------------------------------------
*/

getcsum:
	bsr	locrawgetc
	add.b	RetVal, CheckSum
	rts

/*----------------------------------------------------------------------------
/*  getwsum  --  get a word (2 bytes) and add to checksum
/*----------------------------------------------------------------------------
*/

getwsum:
	clr.l	TempD1
	bsr	getcsum			/* get high order byte */
	mov.l	RetVal, TempD1		
	bsr	getcsum			/* get low order byte */
	asl.l	&8, TempD1 		
	or.w	TempD1, RetVal		/* merge together */
	rts

/*----------------------------------------------------------------------------
/*  getlsum  --  get a longword and add to checksum
/*----------------------------------------------------------------------------
*/

getlsum:
	clr.l	TempD2
	bsr	getwsum			/* get high order word */
	mov.l	RetVal, TempD2
	bsr	getwsum			/* get low order word */
	swap.w	TempD2
	or.l	TempD2, RetVal		/* combine the two */
	rts

/*----------------------------------------------------------------------------
/*  monitor  -- return control to the firmware monitor (NO re-initialization)
/*----------------------------------------------------------------------------
*/

monitor:
        mov.b	&5, (DBUGptr)+
notyet:
	btst	&TXEMPTY, UARTSTAT
	beq	notyet
	trap	&1

