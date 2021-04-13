|Block transfer subroutine: blt(destination, source, count), returns count

	.globl	blt
	.text

blt:	movl	sp@(4),a0	|destination
	movl	sp@(8),a1	|source
	movl	a0,d0		|destination
	andl	#1,d0		|see if word aligned
	movl	a1,d1		|source
	andl	#1,d1		|see if long word aligned?
	cmpl	d0,d1		|do they agree?
	beq	1$		|yes, can do something interesting
	movl	sp@(12),d1	|count
	bra	5$		|ho, hum, just do byte moves
1$:	movl	sp@(12),d1	|count
	tstl	d0		|are we on a long boundry?
	beq	3$		|yes, don't worry about fudge
	negl	d0		|complement
	addql	#2,d0		|2 - adjustment = fudge
	cmpl	d1,d0		|is count bigger than fudge
	bge	5$		|no, must be 3 bytes or less
	subl	d0,d1		|shrink remaining count by this much
	subql	#1,d0		|dbf is a crock
2$:	movb	a1@+,a0@+	|move bytes to get to long boundry
	subql	#1,d0
	bge	2$		|while alignment count
3$:	movl	d1,d0		|copy remaining count
	andl	#~3,d0		|count mod 4 is number of long words
	beq	5$		|hmm, must not be any
	subl	d0,d1		|long words moved * 4 = bytes moved
	asrl	#2,d0		|number of long words
	cmpl	#12,d0		|do we have a bunch to do?
	blt	38$		|no, just do normal moves
	moveml	#0x7F3E,sp@-	|save some registers
34$:	moveml	a1@+,#0x7CFE	|block move via various registers
	moveml	#0x7CFE,a0@
	addl	#48,a0		|moveml won't let me auto inc a destination
	subl	#12,d0		|we moved twelve longs worth
	cmpl	#12,d0		|do we have another 12 to go
	bge	34$		|yes, keep at it
	moveml	sp@+,#0x7CFE	|restore registers
	tstl	d0		|any long's left
	beq	5$		|no, nothing but a few random bytes
38$:	subql	#1,d0		|dbf is a crock
4$:	movl	a1@+,a0@+	|copy as many long words as possible
	subql	#1,d0
	bge	4$		|while long word count
5$:	tstl	d1		|anything left to do?
	beq	7$		|nothing left
	subql	#1,d1		|dbf is a crock
6$:	movb	a1@+,a0@+	|copy any residual bytes
	subql	#1,d1
	bge	6$		|while byte count
7$:	movl	sp@(12),d0	|just return the count
	rts
