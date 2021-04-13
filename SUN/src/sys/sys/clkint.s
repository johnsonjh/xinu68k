|* clkint.s -  clkint


|*------------------------------------------------------------------------
|* clkint  --  real-time clock interrupt service routine 
|*------------------------------------------------------------------------
	.globl	_dog_count
	.globl	_Asm_clkint
	.globl	_clkintend


IR_ENA_CLK5	= 0x20
IR_ENA_CLK7	= 0x80
INTERRUPT_BASE	= 0xfe0a000
CLOCK_BASE 	= 0xfe06000
clk_intrreg	= 0x10
DOG_TIMEOUT	= 150	| allowable 1/10's of seconds with intr disabled
			| 150 = 15 seconds
			| when interrupts are disabled for this long, the
			| system will panic

	.data
savePS:
	.word	0
_junk:
	.long	0
save_sr:
	.word	0
_dog_count:
	.long	DOG_TIMEOUT


	.text

_Asm_clkint:

	andb	#~IR_ENA_CLK7,INTERRUPT_BASE	| Shut it off,
	orb	#IR_ENA_CLK7,INTERRUPT_BASE	| then back on.
	tstb	CLOCK_BASE+clk_intrreg		| Clear the interrupt again

|
| fix for watchdog timer
|
| Clock moved to level7, (non-maskable), so we had better check if
| we are supposed to do anything by checking the old sr
| The word pointed to by the sp on an exception is the previous
| value of the status register, so check it.
	movw	sp@, save_sr		| get the previous SR
	andw	#0x0700, save_sr	| see if disabled
					| z flag set: enabled
	bne	watchdog		| check that something is happening

	movl	#DOG_TIMEOUT, _dog_count	| reset watchdog timer
endfix:
|
| end of fix
|

	subql	#1,_count10		| Is this 10th tick?
	bgt	clckdef			|  no => process tick
	movl	#10,_count10		|  yes=> reset counter&continue
	addql	#1,_clktime		| increment time-of-day clock

clckdef:
	tstl    _defclk			| Are clock ticks deferred?
	beq     notdef			|  no => skip down
	addql	#1,_clkdiff		|  yes=> count in clkdiff and
clret1:
	rte				| return quickly

notdef:
	moveml	#0xc0c0, sp@-		| same d0, d1, a0, a1

	jsr	_updateleds

	movw	#0x2700, sr		| disable ALL interrupts

	tstl    _slnempty		| Is sleep queue nonempty?
	beq     clpreem			|  no=> skip down
	movl	_sltop,a0
	subql	#1,a0@			|  yes => decrement delta key
	bpl     clpreem			|        of first process,
	jsr	_wakeup			|     

clpreem:
	subql	#1,_preempt		| Decrement preemption counter
	bpl	clret			|   and call resched if it
	jsr	_resched		|   reaches 0
clret:

	moveml	sp@+,#0x0303
	rte				| Return from interrupt
_clkintend:


watchdog:
	subl	#1, _dog_count		| decrement watchdog timer
	ble	timeout
	rte

timeout:
	moveml	#0xc0c0, sp@-		| same d0, d1, a0, a1
	jsr	_dog_timeout
	moveml	sp@+,#0x0303
	rte

