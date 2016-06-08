.section .text
.global cpu_id
.type cpu_id, @function
	
cpu_id:
	mov %edi,%eax
	cpuid
	mov %eax,cpuinfo
	mov %ebx,cpuinfo+4
	mov %ecx,cpuinfo+8
	mov %edx,cpuinfo+12
	mov $cpuinfo,%rax
	ret

.section .data
cpuinfo:
	.rept 16
	.byte 0
	.endr
