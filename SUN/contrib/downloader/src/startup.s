|
| Startup code for Sun3 Xinu resident downloader

	.globl	_start


	.text
_start:
	movw	#0x2700,sr		| disable interrupts

	|
	| move the code to the correct memory address
	|
leax:	lea	pc@(_start-(leax+2)),a0	| True current location of "_start"
	lea	_start:l,a1		| Desired      location of "_start"
	cmpl	a0,a1
	jeq	gobegin			| If already correct, jump around
	movl	#_end,d0		| Desired end of program
	subl	a1,d0			| Calculate length, round up.
	lsrl	#2,d0
movc:	movl	a0@+,a1@+		| Move it where it belongs.
	dbra	d0,movc


gobegin:
	jmp	begin:l			| Force non-PCrel jump

begin:
	jsr	_main
	rts				| Return to caller (PROM probably)
