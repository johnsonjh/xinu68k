|* ethint.s -  Asm_ethint


|*------------------------------------------------------------------------
|* ethint -- LANCE ethernet controller interrupt routine
|*------------------------------------------------------------------------
	.globl	_Asm_ethint

	.data
savePS:	.word

	.text
_Asm_ethint:
	| disable(ps);
	movw	sr, savePS		| save current interrupts status
	movw	#0x2700, sr		| disable all interrupts


	moveml	#0xc0c0, sp@-		| save d0, d1, a0, a1

        pea	_eth
	jsr	_ethinter		| call ethernet interrupt routine
	addqw	#0x4,sp

ret:
	moveml	sp@+,#0x0303		| restore d0, d1, d0, d1

	| restore (savePS);
	movw	savePS, sr		| return original content of sr

	rte				| Return from interrupt
