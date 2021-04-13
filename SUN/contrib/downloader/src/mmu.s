| mmu.s asm language routines to access the mmu, etc

	.text			
	.globl _Fc3ReadBit8
	.globl _Fc3ReadBit32	
	.globl _Fc3WriteBit8	
	.globl _Fc3WriteBit32
    
_Fc3ReadBit8:		
	movl sp@(4),a0			| /* get arg into a0 */
	movsb a0@, d0			| /* move from outer space */
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

_Fc3WriteBit32:		
	movl sp@(4),a0			| /* get address into a0 */
	movl sp@(8),d0			| /* get value into d0 */
	movsl d0,a0@			| /* move to outer space */
	rts			

