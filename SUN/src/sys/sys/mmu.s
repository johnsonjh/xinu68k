| mmu.s asm language routines to access the mmu, etc

	.text			
	.globl _Fc3ReadBit8
	.globl _Fc3ReadBit16	
	.globl _Fc3ReadBit32	
	.globl _Fc3WriteBit8	
	.globl _Fc3WriteBit16	
	.globl _Fc3WriteBit32
	.globl _FlushCache

    
_Fc3ReadBit8:		
	movl sp@(4),a0			| /* get arg into a0 */
	movsb a0@, d0			| /* move from outer space */
	rts			

_Fc3ReadBit16:		
	movl sp@(4),a0			| /* get arg into a0 */
	movsw a0@, d0			| /* move from outer space */
	rts			

_Fc3ReadBit32:		
	movl sp@(4),a0			| /* get arg into a0 */
	movsl a0@,d0			| /* move from outer space */
	rts			

_Fc3WriteBit8:		
	movl sp@(4),a0			| /* get address into a0 */
	movl sp@(8),d0			| /* get value into d0 */
	movsb d0,a0@			| /* move to outer space */
	rts			

_Fc3WriteBit16:		
	movl sp@(4),a0			| /* get address into a0 */
	movl sp@(8),d0			| /* get value into d0 */
	movsw d0,a0@			| /* move to outer space */
	rts			

_Fc3WriteBit32:		
	movl sp@(4),a0			| /* get address into a0 */
	movl sp@(8),d0			| /* get value into d0 */
	movsl d0,a0@			| /* move to outer space */
	rts			


CACR_CLEAR	= 0x8
CACR_ENABLE	= 0x1
_FlushCache:
	movl	d0,sp@-			| save d0
	moveq	#CACR_CLEAR+CACR_ENABLE,d0
	.long	0x4e7b0002		| movc	d0,cacr
	movl	sp@+,d0			| restore d0
	rts			

