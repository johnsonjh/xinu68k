
IDPROMOFF = 0x0000000
FC_MAP    = 0x3

| getidprom(addr, size)
|     Read back <size> bytes of the ID prom and store them at <addr>.

	.globl	_getidprom
_getidprom:
	movl	sp@(4),a0	| address to move ID prom bytes to
	movl	sp@(8),d1	| How many bytes to move
	movl	d2,sp@-		| save a reg
	movc	sfc,d0		| save source func code
	movl	#FC_MAP,d2
	movc	d2,sfc		| set space 3
	lea	IDPROMOFF,a1	| select id prom
	jra	2$		| Enter loop at bottom as usual for dbra
1$:	movsb	a1@+,d2		| get a byte
	movb	d2,a0@+		| save it
2$:	dbra	d1,1$		| and loop
	movc	d0,sfc		| restore sfc
	movl	sp@+,d2		| restore d2
	rts
