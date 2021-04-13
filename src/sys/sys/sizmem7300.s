|* sizmem.s - sizmem */

MEMMAP	=	0x400000
GCR	=	0xe40000
TPAT	=	0x5533
|*------------------------------------------------------------------------
|*  sizmem  --  size memory, placing highest valid address in _maxaddr
|*  for  ATT7300, all possible 4M acts real even if not there--
|*  have to detect wrapped memory addresses.  So write a test pattern
|*  to location 0 (not used for reset vector on this system), and
|*  then check each start of .5M block for same pattern.  When
|*  it shows up again, that's not really memory, just a reflection
|*  of prior memory.  This works for up to 2M of onboard memory.
|*  If you need more than that for Xinu, you're serious enough to
|*  know how to fix this for more memory.
|*------------------------------------------------------------------------
	.text
	.globl	_sizmem
_sizmem:
	clrl d0
	movl d0,a0	  | moving pointer
	movw #TPAT,0      | put test pattern in loc 0
	movw #TPAT,d0     | and in d0
sizloop:
	addl #0x80000,a0  | next .5M block
	cmpl #0x200000,a0 | up to 2M of mem
	bge  sizdone
	cmpw a0@,d0       | ck for wrapped val showing up
	bne sizloop
sizdone:movl a0,d0        | here with wrap detected or 2M OK
	subl #4,d0        | Xinu wants last valid word, not first invalid
	movl d0,_maxaddr
	rts
