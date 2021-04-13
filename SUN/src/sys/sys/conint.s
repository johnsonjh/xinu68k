|* conint.s -  Asm_conint


|*------------------------------------------------------------------------
|* conint  --  zilog chip interrupt service routine for keyboard
|*------------------------------------------------------------------------
	.data

ZCONTROLA = 0xfe00004

	.text
	.globl	_Asm_conint


_Asm_conint:
	moveml	#0xc1c0, sp@-		| save d0, d1, d7, a0, a1

	jsr _coniin

	jmp done

|
| read from RR3 of the chip
|
	movl	#ZCONTROLA,a0		| address of A channel
	movb	#0x3,a0@		| select register 3	

	moveq	#20,d0			| must delay before reading
	movl	_cpudelay,d1
	asrl	d1,d0
1:	subql	#1,d0
	bgts	1b

	movb	a0@,d7			| d7 = RR3


|
| test for receive interrupt
|
|	btst	#0x5,d7			| xx1x xxxx = recv int. pending
|	beq	norecv
recv:
	jsr	_coniin			| call input interrupt routine
norecv:

|
| now, reset the interrupt and return
|
done:
	movl	#ZCONTROLA,a0		| address of A channel
	movb	#0x38,a0@		| reset interrupt

	moveml	sp@+,#0x0383		| restore d0, d1, d7, a0, a1
	rte				| Return from interrupt
