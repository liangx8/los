.text
.global update_cursor
.type update_cursor, @function
update_cursor:
	push	%ebp
	mov		%esp,%ebp
	mov		8(%ebp),%eax		# first argument
	mov		12(%ebp),%ecx		# second argument
	pop		%ebp
	ret
