|J. Test	3/81
|floating point routines for Nu(MC68000)

				|offsets in internal float structure
SIGN	= 0				|sign
EXPT	= 2				|exponent (-127/+127)
MANH	= 4				|high mantissa
MANL	= 8				|low mantissa

	.data
afloat:				|internal-a floating representation
a_sign:	.word	0
a_expt:	.word	0
a_manh:	.long	0
a_manl:	.long	0

bfloat:				|internal-b floating representation
b_sign:	.word	0
b_expt:	.word	0
b_manh:	.long	0
b_manl:	.long	0

lsum:				|storage for multiply
	.word	0
	.long	0			|high part of accumulated sum
	.long	0			|low part of accumulated sum
	.word	0

	
|
|convert external float to internal format
|d0,d1 contain the external float
|a0 points to afloat or bfloat
|
	.text
etoi:
	clrw	a0@			|clear sign
	tstl	d0			|test sign of external
	bge	1$			|set sign 0(+), 1(-)
	movw	#1,a0@

1$:	movl	d1,a0@(MANL)		|save low 32 bits of mantissa
	movl	d0,d1
	andl	#0x7FFFFF,d1
	orl	#0x800000,d1		|add hidden high order bit
	movl	d1,a0@(MANH)		|save high 1+23 bits of mantissa
	swap	d0
	asrl	#7,d0
	andw	#0xFF,d0		|isolate exponent
	bne	2$
	clrl	a0@			|zero sign, exponent,
	clrl	a0@(MANH)		|  high mantissa, and
	clrl	a0@(MANL)		|  low mantissa
	rts

2$:	subw	#128,d0			|convert from excess 0200
	movw	d0,a0@(EXPT)		|store converted value
	rts				|done

|
|convert internal format to external float
|a0 points to afloat or bfloat
|external float returned in d0,d1
|
	.text
itoe:
	clrl	d0
	movw	a0@(EXPT),d0		|get exponent
	addw	#128,d0			|convert to excess 0200
	bne	1$			|if exponent is zero
	clrl	d1			|  clear d0,d1
	rts				|  and return

1$:	tstw	a0@			|set sign
	beq	2$			|sign bit 0(+), 1(-)
	orw	#0x100,d0

2$:	swap	d0			|align sign and exponent
	asll	#7,d0			|  in high part of d0
	movl	a0@(MANH),d1		|get high part of mantissa
	bne	3$			|check for zero mantissa
	clrl	d0			|if zero - clear sign and
	rts				|  exponent and return

3$:	andl	#0x7FFFFF,d1		|delete high order hidden bit
	orl	d1,d0			|put high 23 bits of mantissa
	movl	a0@(MANL),d1		|put low 32 bits of mantissa
	rts				|done

|
|normalize internal float by adjusting exponent and
|shifting  mantissa appropriately so 1/2 <= mnt < 1
|a0 points to afloat or bfloat
|
	.text
normal:
	jsr	offset			|determine amount to shift
	addw	d0,a0@(EXPT)		|adjust exponent
	jsr	shift			|shift mantissa
	rts

|
|determine position of most significant bit of
|mantissa in relation to normalized decimal point
|a0 points to afloat or bfloat
|d0 returns offset of msb from decimal point
|
	.text
offset:
	moveq	#1,d0
	movl	a0@(MANH),d1		|check for high order bits
	bne	2$
	movl	a0@(MANL),d1		|check low order bits
	bne	1$
	clrw	d0			|zero shift count
	rts

1$:	subw	#32,d0			|need to shift at least 23
2$:	subqw	#1,d0			|find most significant bit
	asll	#1,d1
	bcc	2$
	addqw	#8,d0			|d0 contains exponent correction
	rts

|
|shift mantissa according to offset in d0
|a0 points to afloat or bfloat
|d0 contains shift count, <0 -> left shift, >0 -> right shift
|on return, d1 = 0, d2,d3 have shifted mantissa
|
	.text
shift:
	clrl	d1
	movl	a0@(MANH),d2		|d2 = high part of mantissa
	movl	a0@(MANL),d3		|d3 = low part of mantissa
	movw	d0,d1			|examine exponent correction
	bmi	2$			|shift left
	bne	1$			|shift right
	rts				|no shift - return

1$:	asrl	#1,d2			|shift entire mantissa
	roxrl	#1,d3			|  right by one bit
	subqw	#1,d1			|repeat until count
	bne	1$			|  is zero
	andl	#0xFFFFFF,d2		|zero top byte
	bra	shifte			|return

2$:	asll	#1,d3			|shift entire mantissa
	roxll	#1,d2			|  left by one bit
	addqw	#1,d1			|repeat until count
	bne	2$			|  is zero
	
shifte:	movl	d2,a0@(MANH)		|store high part of mantissa
	movl	d3,a0@(MANL)		|store low part of mantissa
	rts				|done
	
|
|fetch floating arguments off stack
|convert to internal format in afloat and bfloat
|on return, a0 points to afloat, a1 points to bfloat
|
getargs:
	movl	#bfloat,a1		|a1 points to bfloat
	tstw	d0			|branch to 1$ for
	bne	1$			|  indirect argument fetch
	movl	a6@(16),d0		|b-high
	movl	a6@(20),d1		|b-low
	movl	a1,a0			|setup a0 for conversion
	jsr	etoi			|convert b-arg to internal form
	movl	a6@(8),d0		|a-high
	movl	a6@(12),d1		|a-low
	movl	#afloat,a0		|a0 points to afloat
	jsr	etoi			|convert a-arg to internal form
	rts
1$:	movl	a6@(12),d0		|b-high
	movl	a6@(16),d1		|b-low
	movl	a1,a0			|setup a0 for conversion
	jsr	etoi			|convert b-arg to internal form
	movl	a6@(8),a0		|a0 points to a-arg
	movl	a0@+,d0			|a-high
	movl	a0@,d1			|a-low
	movl	#afloat,a0		|a0 points to afloat
	jsr	etoi			|convert a-arg to internal form
	rts	

	.globl	fltused
	.text
fltused:
	rts				|simply define fltused

|
|free exponent returning fractional value
|
	.globl	frexp
	.text
frexp:
	link	a6,#0
	movl	#afloat,a0		|a0 points to afloat
	movl	a6@(8),d0		|a-high
	movl	a6@(12),d1		|a-low
	movl	a6@(16),a1		|place to return exponent
	jsr	etoi			|convert to internal form
	movw	a0@(EXPT),d0		|get unbiased exponent
	extl	d0			|  convert to long and
	movl	d0,a1@			|  return value
	clrw	a0@(EXPT)		|set exponent for fractional
	jsr	itoe			|  value, convert for return
	unlk	a6
	rts

|
|add/load exponent of float
|
	.globl	ldexp
	.text
ldexp:
	link	a6,#0
	movl	#afloat,a0		|a0 points to afloat
	movl	a6@(8),d0		|a-high
	movl	a6@(12),d1		|a-low
	jsr	etoi			|convert to internal form
	movl	a6@(16),d0		|add argument
	addw	d0,a0@(EXPT)		|  to exponent
	jsr	itoe			|convert and return
	unlk	a6
	rts


|
|separate integer/fractional parts of float
|
	.globl	_modf
	.text
_modf:
	link	a6,#0
	moveml	#0x3800,sp@-		|save d2,d3,d4
	movl	a6@(8),d0		|a-high
	movl	a6@(12),d1		|a-low
	movl	#afloat,a0		|a0 -> afloat = fractional
	jsr	etoi			|  part on return
	movl	#bfloat,a1		|a1 -> bfloat = integer part on return
	movw	a0@,a1@			|copy signs
	movw	a0@(EXPT),d4		|if exponent > 0
	bgt	1$			|  separate integer/fractional
	movw	#-128,a1@(EXPT)		|else integer part = 0
	movl	a1,a0			|  convert integer part first
	bra	modfe			|  no need to separate

1$:	movw	d4,a1@(EXPT)		|set integer exponent
	clrw	a0@(EXPT)		|set fractional exponent
	cmpw	#56,d4			|if shift count is < 56
	blt	2$			|  shift mantissa
	movl	a0@(MANL),a1@(MANL)	|else move mantissa to integer
	movl	a0@(MANH),a1@(MANH)	|  part and set fractional
	movw	#-128,a0@(EXPT)		|  part = 0
	movl	a1,a0			|convert decimal part first
	bra	modfe			|  on exit from modf

2$:	moveq	#-8,d0			|shift mantissa left
	jsr	shift			|  by 8 for alignment
	clrl	d0
3$:	asll	#1,d3			|rotate d0<--d1<--d2<--d3
	roxll	#1,d2			|  registers until shift
	roxll	#1,d1			|  count = 0
	roxll	#1,d0
	subqw	#1,d4
	bne	3$
	movl	d3,a0@(MANL)		|save fractional components
	movl	d2,a0@(MANH)		|  of mantissa
	movl	d1,a1@(MANL)		|save integer components
	movl	d0,a1@(MANH)		|  of mantissa
	jsr	normal			|align fractional part
	subqw	#8,a0@(EXPT)		|  and adjust exponent
	movl	a1,a0			|align integer part at
	jsr	offset			|  decimal point without
	jsr	shift			|  altering exponent

modfe:	movl	a6@(16),a1		|get pointer argument
	jsr	itoe			|convert integer part
	movl	d0,a1@+			|  store in location
	movl	d1,a1@			|  given in argument
	movl	#afloat,a0		|convert fractional
	jsr	itoe			|  part and return
	moveml	sp@+,#0x1C
	unlk	a6
	rts

|
|convert floating value to fixed 32-bit integer
|
	.globl	fix
	.text
fix:
	link	a6,#0
	moveml	#0x3000,sp@-		|save d2,d3
	movl	a6@(8),d0		|d0 = high part of float
	movl	a6@(12),d1		|d1 = low order part
	movl	#afloat,a0		|a0 points to afloat
	jsr	etoi			|convert to internal format
	tstw	a0@(EXPT)		|test exponent
	bgt	1$			|if exponent is less
	clrl	d0			|  than or equal to zero
	bra	fixe			|  return zero

1$:	moveq	#-8,d0			|shift mantissa left
	jsr	shift			|  by 8 for alignment
	movw	a0@(EXPT),d1		|(note: after shift d1 = 0)
	clrl	d0			|clear d0
2$:	asll	#1,d3			|rotate d0<--d2<--d3
	roxll	#1,d2			|  registers until exponent
	roxll	#1,d0			|  count is exhausted
	subqw	#1,d1			|resultant fixed 32-bit
	bne	2$			|  value is in d0

	tstw	a0@			|check sign of float
	beq	fixe			|positive - d0 is ok
	negl	d0			|negative - negate d0
fixe:	moveml	sp@+,#0xC		|pop d2,d3
	unlk	a6
	rts

|
|convert fixed 32-bit integer to floating
|
	.globl	float
	.text
float:
	link	a6,#0
	moveml	#0x3000,sp@-		|save d2,d3
	movl	#afloat,a0		|a0 points to afloat
	clrl	a0@(MANH)		|clear junk from mantissa
	clrw	a0@			|clear sign
	movl	a6@(8),d0		|d0 = 32-bit long
	bmi	1$			|negative
	bpl	2$			|positive
	movw	#-128,a0@(EXPT)		|floating zero
	bra	floate			|return

1$:	movw	#1,a0@			|negative sign
	negl	d0			|convert to positive
2$:	movl	d0,a0@(MANL)		|move d0 to lower mantissa
	jsr	offset			|determine amount to shift
	jsr	shift			|shift mantissa
	addw	#56,d0			|calculate exponent
	movw	d0,a0@(EXPT)		|set exponent

floate:	jsr	itoe			|convert to external float
	moveml	sp@+,#0xC		|pop d2,d3
	unlk	a6
	rts

|
|add, subtract, compare two floating point numbers
|d0,d1 return result of fadd,fsub operations
|result of afadd,afaddf,afsub,afsubf stored
|fcmp sets condition codes upon return
|
	.globl	fsub
	.globl	fadd
	.globl	fcmp
	.globl	afadd
	.globl	afsub
	.globl	afaddf
	.globl	afsubf
	.text
fadd:
	link	a6,#0
	clrw	d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	add			|perform addition
	unlk	a6
	rts
fsub:
	link	a6,#0
	clrw	d0			|flag to getargs
	jsr	getargs			|get arguments
	eorw	#1,a1@			|reverse sign of b-arg
	jsr	add			|perform addition
	unlk	a6
	rts
fcmp:
	link	a6,#0
	clrw	d0			|flag to getargs
	jsr	getargs			|get arguments
	eorw	#1,a1@			|reverse sign of b-arg
	jsr	add			|perform compare
	tstl	d0			|set condition code
	unlk	a6
	rts
afadd:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	add			|perform addition
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@+			|  to store result
	movl	d1,a0@			|  of operation
	unlk	a6
	rts
afsub:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	eorw	#1,a1@			|reverse sign of b-arg
	jsr	add			|perform addition
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@+			|  to store result
	movl	d1,a0@			|  of operation
	unlk	a6
	rts
afaddf:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	clrl	a0@(MANL)		|clear lower part of a-arg
	jsr	add			|perform addition
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@			|  to store result
	unlk	a6
	rts
afsubf:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	clrl	a0@(MANL)		|clear lower part of a-arg
	eorw	#1,a1@			|reverse sign of b-arg
	jsr	add			|perform addition
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@			|  to store result
	unlk	a6
	rts
add:
	moveml	#0x3000,sp@-		|save d2,d3
	movw	a0@(EXPT),d0		|compare
	movw	a1@(EXPT),d1		|  exponents
	subw	d1,d0			|  of a and b
	bmi	1$			|a_expt < b_expt
	movl	a1,a0			|switch a0 to point to bfloat
	jsr	shift			|shift mantissa of bfloat
	movl	#afloat,a1		|switch a1 to point to afloat
	addw	d0,a0@(EXPT)		|adjust b_expt accordingly
	bra	2$
1$:	negw	d0			|make the shift count positive
	jsr	shift			|shift mantissa of afloat
	addw	d0,a0@(EXPT)		|adjust a_expt accordingly
2$:	tstw	a0@
	beq	3$			|a-arg is negative so
	negl	d3			|  negate mantissa
	negxl	d2			|  for addition
3$:	movl	a1@(MANH),d0
	movl	a1@(MANL),d1
	tstw	a1@
	beq	4$			|b-arg is negative so
	negl	d1			|  negate mantissa
	negxl	d0			|  for addition
4$:	addl	d1,d3			|perform addition of
	addxl	d0,d2			|  mantissas
	tstl	d2			|check sign of result
	bge	5$
	negl	d3			|result is negative so
	negxl	d2			|  negate mantissa
	orw	#1,a0@			|  and set sign
	bra	adde
5$:	andw	#0,a0@			|result positive

adde:	movl	d2,a0@(MANH)		|store result
	movl	d3,a0@(MANL)		|  of computation
	jsr	normal			|normalize result
	jsr	itoe			|convert to external form
	moveml	sp@+,#0xC		|pop d2,d3
	rts

|
|negate a floating number
|argument on stack
|d0,d1 return result
|
	.globl	fneg
	.text
fneg:
	link	a6,#0
	movl	a6@(12),d1		|d1 = low part of float
	movl	a6@(8),d0		|d0 = high part of float
	bmi	1$			
	orl	#0x80000000,d0		|turn high order bit on
	bra	2$
1$:	andl	#0x7FFFFFFF,d0		|turn high order bit off
2$:	unlk	a6
	rts	

|
|multiply two floating numbers
|d0,d1 return result for fmul
|result for afmul,afmulf stored
|
	.globl	fmul
	.globl	afmul
	.globl	afmulf
	.text
fmul:
	link	a6,#0
	clrw	d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	mult			|multiply arguments
	unlk	a6
	rts
afmul:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	mult			|perform multiply
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@+			|  to store result
	movl	d1,a0@			|  of operation
	unlk	a6
	rts
afmulf:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	mult			|perform multiply
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@			|  to store result
	unlk	a6
	rts
mult:
	moveml	#0x3C20,sp@-		|save d2-d5,a2
	movw	a1@+,d0			|d0 = sign of b-arg
	eorw	d0,a0@+			|a_sign gets resultant sign
	movw	a1@+,d0 		|d0 = exponent of b-arg
	addw	d0,a0@+			|a_expt gets sum of exponents

	clrl	d2			|clear
	clrl	d3			|  summation registers
	clrl	d5			|  for multiply
	moveq	#4,d4			|loop count
	addql	#8,a1			|adjust a1 pointer
	movl	#lsum,a2		|adjust a2 pointer
l1$:	movw	a0@+,d0			|high to low words of afloat
	movw	a1@-,d1			|low to high words of bfloat
	mulu	d0,d1			|perform multiply
	addl	d1,d3
	addxl	d5,d2	
	subqw	#1,d4
	bne	l1$
	movl	d4,a2@+
	movl	d2,a2@+
	movl	d3,a2@

	clrl	d2
	clrl	d3
	moveq	#3,d4			|loop count
	subql	#2,a0			|adjust a0 pointer
	addql	#2,a2			|adjust a2 pointer
l2$:	movw	a0@-,d0			|low to high of afloat
	movw	a1@+,d1			|high to low of bfloat
	mulu	d0,d1			|perform multiply
	addl	d1,d3
	addxl	d5,d2
	subqw	#1,d4
	bne	l2$
	movl	a2@-,d1
	movl	a2@-,d0
	addl	d1,d3
	addxl	d0,d2
	movl	d2,a2@+
	movl	d3,a2@	
	
	clrl	d2
	clrl	d3
	moveq	#2,d4			|loop count
	subql	#2,a1			|adjust a1 pointer
	addql	#2,a2			|adjust a2 pointer
l3$:	movw	a0@+,d0			|high to low of afloat
	movw	a1@-,d1			|low to high of bfloat
	mulu	d0,d1
	addl	d1,d3
	addxl	d5,d2
	subqw	#1,d4
	bne	l3$
	movl	a2@-,d1
	movl	a2@-,d0
	addl	d1,d3
	addxl	d0,d2
	movl	d2,a2@+
	movl	d3,a2@

	subql	#2,a0			|adjust a0 pointer
	addql	#2,a2			|adjust a2 pointer
	movw	a0@-,d0
	movw	a1@+,d1
	mulu	d0,d1
	addl	d1,a2@-

multe:	addql	#1,a2			|adjust a2 pointer
	movb	a2@+,a0@+		|move result
	movb	a2@+,a0@+		|  to afloat
	movb	a2@+,a0@+
	movb	a2@+,a0@+
	movb	a2@+,a0@+
	movb	a2@+,a0@+
	movb	a2@+,a0@+
	movb	a2@+,a0@+
	subql	#8,a0			|align a0 to point
	subql	#4,a0			|  to afloat
	jsr	normal			|normalize result
	jsr	itoe			|convert to external form
	moveml	sp@+,#0x43C		|pop registers
	rts

|
|divide two floating numbers
|
	.globl	fdiv
	.globl	afdiv
	.globl	afdivf
	.text
fdiv:
	link	a6,#0
	clrl	d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	div			|divide arguments
	unlk	a6
	rts
afdiv:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	div			|divide arguments
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@+			|  to store result
	movl	d1,a0@			|  of operation
	unlk	a6
	rts
afdivf:
	link	a6,#0
	moveq	#1,d0			|flag to getargs
	jsr	getargs			|get arguments
	jsr	div			|divide arguments
	movl	a6@(8),a0		|a0 points to where
	movl	d0,a0@			|  to store result
	unlk	a6
	rts
div:
	moveml	#0x3E00,sp@-		|save d2-d6
	movw	a1@+,d0			|d0 = sign of b-arg
	eorw	d0,a0@+			|a-sign gets resultant sign
	movw	a1@+,d0			|d0 = exponent of b-arg
	subw	d0,a0@+			|a-expt gets diff of exponents
	movl	a1@+,d0			|d0 = divisor high
	bne	ok			|if divisor = 0
	divu	d0,d1			|  cause trap and core dump

ok:	movl	a1@,d1			|d1 = divisor low
	movl	a0@+,d2 		|d2 = dividend high
	movl	a0@,d3			|d3 = dividend low
	clrl	d4			|clear quotient
	clrl	d5			|  register set
	moveq	#58,d6			|setup shift count
	bra	2$

1$:	subqw	#1,d6			|exit computation when
	beq	dive			|  loop count = 0
	movw	#0x10,cc		|set x-bit in ccr
	roxll	#1,d5			|shift 1 into quotient
	roxll	#1,d4			|  accumulator registers
	roxll	#1,d3			|continue shift into
	roxll	#1,d2			|  into remainder registers
2$:	subl	d1,d3			|subtract divisor from
	subxl	d0,d2			|  remainder
	bge	1$

3$:	subqw	#1,d6			|exit computation when
	beq	dive			|  loop count = 0
	asll	#1,d5			|shift 0 into quotient
	roxll	#1,d4			|  accumulator registers
	roxll	#1,d3			|continue shift into
	roxll	#1,d2			|  remainder registers
	addl	d1,d3			|add divisor to
	addxl	d0,d2			|  remainder
	bge	1$			|positive -> shift 1
	bra	3$			|negative -> shift 0

dive:	subql	#4,a0			|align a0 to mantissa
	movl	d4,a0@+			|store quotient
	movl	d5,a0@			|  into afloat mantissa
	subql	#8,a0			|align a0 to afloat
	jsr	normal			|normalize mantissa
	jsr	itoe			|convert to external float
	moveml	sp@+,#0x7C		|pop d2-d6
	rts

