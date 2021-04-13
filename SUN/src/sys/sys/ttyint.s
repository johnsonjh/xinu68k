|* ttyint.s -  Asm_ttyint


ZCONTROLA = 0xfe02004	| A serial line
ZCONTROLB = 0xfe02000	| B serial line
ZCONTROLM = 0xfe00000	| Mouse serial line

|*------------------------------------------------------------------------
|* ttyint  --  zilog chip interrupt service routine
|*------------------------------------------------------------------------
	.globl	_Asm_ttyint


	.data
savePS:	.word

	.text
_Asm_ttyint:
	| disable(ps);
	movw	sr, savePS		| save current interrupts status
	movw	#0x2700, sr		| disable all interrupts

	moveml	#0xc1c0, sp@-		| save d0, d1, d7, a0, a1

|
| read from RR0 of the chip
|
	movl	#ZCONTROLA,a0		| address of A channel
	movb	#0x0,a0@		| select register 0	

	moveq	#20,d0			| must delay before reading
	movl	_cpudelay,d1
	asrl	d1,d0
1:	subql	#1,d0
	bgts	1b

	movb	a0@,d7			| d7 = RR0

|
| test for ttyline break
|
	btst	#0x7,d7			| 1xxx xxxx = break detected
	beq	nobreak
break:
	jsr	_ttybreak		| call break routine
nobreak:


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
| test for transmit interrupt
|
	btst	#0x4,d7			| xxx1 xxxx = xmit interrupt pending
	beq	noxmit
xmit:
	pea	_tty
	jsr	_ttyoin			| call output interrupt routine
	addqw	#0x4,sp
noxmit:

|
| test for receive interrupt
|
	btst	#0x5,d7			| xx1x xxxx = recv int. pending
	beq	norecv
recv:
	pea	_tty
	jsr	_ttyiin			| call input interrupt routine
	addqw	#0x4,sp
norecv:

|
| now, reset the interrupt and return
|
	movl	#ZCONTROLA,a0		| address of A channel
	movb	#0x38,a0@		| reset interrupt

ret:
	moveml	sp@+,#0x0383		| restore d0, d1, d7, a0, a1

	| restore (savePS);
	movw	savePS, sr		| return original content of sr

	rte				| Return from interrupt
