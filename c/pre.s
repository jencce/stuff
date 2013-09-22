	.file	"predictest.c"
	.section	.rodata
.LC0:
	.string	"sec %ld, usec %ld\n"
.LC1:
	.string	"end sec %ld, usec %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$848, %esp
	movl	$200, 844(%esp)
	movl	12(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L2
	movl	12(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	atoi
	movl	%eax, 840(%esp)
.L2:
	movl	$0, 836(%esp)
	jmp	.L3
.L4:
	movl	836(%esp), %eax
	imull	$200, %eax, %edx
	movl	836(%esp), %eax
	movl	%edx, 20(%esp,%eax,4)
	addl	$1, 836(%esp)
.L3:
	cmpl	$199, 836(%esp)
	jle	.L4
	cmpl	$0, 840(%esp)
	je	.L5
	movl	$0, 4(%esp)
	leal	820(%esp), %eax
	movl	%eax, (%esp)
	call	gettimeofday
	movl	824(%esp), %ecx
	movl	820(%esp), %edx
	movl	$.LC0, %eax
	movl	%ecx, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$0, 836(%esp)
	jmp	.L6
.L7:
	movl	836(%esp), %eax
	movl	20(%esp,%eax,4), %edx
	movl	836(%esp), %eax
	movl	20(%esp,%eax,4), %eax
	imull	%eax, %edx
	movl	836(%esp), %eax
	movl	%edx, 20(%esp,%eax,4)
	addl	$1, 836(%esp)
.L6:
	cmpl	$199, 836(%esp)
	jle	.L7
	movl	$0, 4(%esp)
	leal	828(%esp), %eax
	movl	%eax, (%esp)
	call	gettimeofday
	movl	832(%esp), %ecx
	movl	828(%esp), %edx
	movl	$.LC1, %eax
	movl	%ecx, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	jmp	.L8
.L5:
	movl	$0, 4(%esp)
	leal	820(%esp), %eax
	movl	%eax, (%esp)
	call	gettimeofday
	movl	824(%esp), %ecx
	movl	820(%esp), %edx
	movl	$.LC0, %eax
	movl	%ecx, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	jmp	.L9
.L10:
	movl	836(%esp), %eax
	movl	20(%esp,%eax,4), %edx
	movl	836(%esp), %eax
	movl	20(%esp,%eax,4), %eax
	imull	%eax, %edx
	movl	836(%esp), %eax
	movl	%edx, 20(%esp,%eax,4)
.L9:
	cmpl	$0, 844(%esp)
	setne	%al
	subl	$1, 844(%esp)
	testb	%al, %al
	jne	.L10
	movl	$0, 4(%esp)
	leal	828(%esp), %eax
	movl	%eax, (%esp)
	call	gettimeofday
	movl	832(%esp), %ecx
	movl	828(%esp), %edx
	movl	$.LC1, %eax
	movl	%ecx, 8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
.L8:
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
