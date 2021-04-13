|* sizmem.s - sizmem */

DISABLE	=	0x2700
EXCPPC	=	0x0008

|*------------------------------------------------------------------------
|*  sizmem  --  size memory, placing highest valid address in _maxaddr
|*------------------------------------------------------------------------
	.text
	.globl	_sizmem, _end
.data
savsp:  .long 0
.text

_sizmem:
	movw	sr, sp@-
	movw	#DISABLE, sr
	movl	a0, sp@-
	movl	EXCPPC, sp@-
	movl	#siztrap, EXCPPC
	movl	sp,savsp       | remember sp for trap
	movl	#_end, a0      | start from end of program
sizloop:
	movl	a0@,a0@+      | ref mem where a0 points
	movl	a0, _maxaddr
	bra 	sizloop

siztrap:
	movl 	_maxaddr, a0
	subl	#4,a0
	movl 	a0,_maxaddr     | adjust to last actual word addr
	movl	savsp,sp        | drop stack frame from trap
	movl	sp@+, EXCPPC | restore vector
	movl	sp@+, a0     | and a0
	movw	sp@+, sr   | | and sr
	rts

