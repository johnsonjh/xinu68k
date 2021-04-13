	global	_disable
_disable:
	mov.l	%a0,	-(%sp)
	mov.l	8(%sp),	%a0
	mov.w	%sr,	(%a0)
	mov.w	&0x2700,%sr
	mov.l	(%sp)+,	%a0
	rts

	global _restore
_restore:
	mov.l	%a0,	-(%sp)
	mov.l	8(%sp),	%a0
	mov.w	(%a0),	%sr
	mov.l	(%sp)+,	%a0
	rts

	global	setlowpri

setlowpri:
	mov.w	&0x2000,%sr	
	rts

