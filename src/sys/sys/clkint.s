|* clkint.s -  clkint

|*------------------------------------------------------------------------
|* clkint  --  real-time clock interrupt service routine 
|*------------------------------------------------------------------------
	.text
	.globl	_clkint
	.globl _tstat
_clkint:
	movl	a0,sp@-			| save a0
	movl	_tstat,a0		| address of hardware reg to ack tick
#ifdef ATT7300
| clock owns one bit of miscellaneous control reg--maintain other bits--
	movl	d0,sp@-
	movw    _soft_mcr7300,d0	| normal value for MCR
	andw	#0x7fff,d0		| clear high bit
	movw	d0,a0@			| acknowledge clock tick
	movw    _soft_mcr7300,a0@	|   with low-high on high bit of MCR
	movl    sp@+,d0
#else
	movb	#1,a0@			| acknowledge clock tick
#endif
	movl 	sp@+,a0			| restore a0
	subql	#1,_count6		| Is this the 6th interrupt?
	bpl	clret			|  no => return
	movl	#6,_count6		|  yes=> reset counter&continue
	tstl     _defclk		| Are clock ticks deferred?
	beq     notdef			|  no => skip down
	addql	#1,_clkdiff		|  yes=> count in clkdiff and
	rte				|        return quickly
notdef:	
	tstl     _slnempty		| Is sleep queue empty?
	beq     clpreem			|  yes=> skip down
	movl	a0,sp@-			| save a0
	movl 	_sltop,a0		|  no => decrement delta key
	subql	#1,a0@			|        of first process
	movl	sp@+,a0			| restore a0, cc's unaffected
	bpl     clpreem			|  ck if ready to wake up
	moveml	#0xc0c0,sp@-		| Save d0,d1,a0,a1 - C scratch regs
	jsr	_wakeup			|     
	moveml	sp@+,#0x0303		| Restore d0,d1,a0,a1
clpreem:
	subql	#1,_preempt		| Decrement preemption counter
	bpl	clret			|   and call resched if it
	moveml	#0xc0c0,sp@-
	jsr	_resched
	moveml	sp@+,#0x0303
clret:
	rte				| Return from interrupt
