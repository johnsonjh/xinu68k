.globl _console
.data
_console: .long 0	   | for printf, etc.
.text
DISABLE = 0x2700

	movw #DISABLE,sr  | make sure no ints for startup
	jsr _sizmem       | find out how much mem we have
	movel _maxaddr,sp  | relocate stack there
	jmp _nulluser      | and run nulluser


