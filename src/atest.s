.data
msg:
	.ascii "hello\n"
	len = . - msg
	.text
	.global _start
_start:
	
