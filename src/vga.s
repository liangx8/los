.text
.global update_cursor
.global cpu_flag
.global cr_flag	
.type update_cursor, @function
.type cpu_flag, @function
.type cr_flag, @function
update_cursor:
	mov		4(%esp),%edx		# first argument
	mov		%edx,%eax
	shl		$2,%eax				# x * 4

	add		%edx,%eax			# x * (4 + 1)

	shl		$4,%eax				# (x * (4 + 1)) * 2 * 2 * 2 * 2

#	mov		%eax,%edx
	mov		8(%esp),%ebx		# second argument

	add		%eax,%ebx

	mov		$14,%al				# crt control register 14
	mov		$0x3d4,%dx
	out		%al,(%dx)
	mov		$0x3d5,%dx
	mov		%bh,%al
	out		%al,(%dx)

	mov		$15,%al				# crt control register 15
	mov		$0x3d4,%dx
	out		%al,(%dx)
	mov		$0x3d5,%dx
	mov		%bl,%al
	out		%al,(%dx)
	mov		%ebx,%eax
	ret     $8

cpu_flag:
	pushf
	pop		%eax
	ret
cr_flag:
	mov		%cr0,%eax
	mov		%eax,buf
	mov		%cr1,%eax
	mov		%eax,buf+4
	mov		%cr2,%eax
	mov		%eax,buf+8
	mov		%cr3,%eax
	mov		%eax,buf+12
	mov		$buf,%eax
	mov		%cr4,%eax
	mov		%eax,buf+20
	mov		$buf,%eax
	ret
.data
buf:
	.rept 20
	.byte 0
	.endr
