	.file	"marray.c"
	.globl	array
	.data
	.align 32
	.type	array, @object
	.size	array, 120
array:
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.long	9
	.long	10
	.long	11
	.long	12
	.long	13
	.long	14
	.long	15
	.long	16
	.long	17
	.long	18
	.long	19
	.long	20
	.long	21
	.long	22
	.long	23
	.long	24
	.long	25
	.long	26
	.long	27
	.long	28
	.long	29
	.globl	sa
	.align 32
	.type	sa, @object
	.size	sa, 40
sa:
	.long	1
	.long	2
	.long	3
	.zero	28
	.globl	a
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	100
	.section	.rodata
.LC0:
	.string	"a = %d\n"
.LC1:
	.string	"sa-p %p\n"
.LC2:
	.string	"sa-lu %lu\n"
.LC3:
	.string	"&sa-p %p\n"
.LC4:
	.string	"&sa-lu %lu\n"
.LC5:
	.string	"sp-p %p\n"
.LC6:
	.string	"sp-lu %lu\n"
.LC7:
	.string	"&sp-p %p\n"
.LC8:
	.string	"&sp-lu %lu\n"
.LC9:
	.string	"%p\n"
.LC10:
	.string	"%d\n"
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
	subl	$32, %esp
	movl	$sa, 16(%esp)
	movl	a, %edx
	movl	$.LC0, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC1, %eax
	movl	$sa, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC2, %eax
	movl	$sa, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC3, %eax
	movl	$sa, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC4, %eax
	movl	$sa, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	16(%esp), %edx
	movl	$.LC5, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	16(%esp), %edx
	movl	$.LC6, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC7, %eax
	leal	16(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC8, %eax
	leal	16(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC9, %eax
	movl	$array, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$.LC9, %eax
	movl	$array, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$array, %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$array, 20(%esp)
	movl	20(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$4, 20(%esp)
	movl	20(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$4, 20(%esp)
	movl	20(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$array, 24(%esp)
	movl	24(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$4, 24(%esp)
	movl	24(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$4, 24(%esp)
	movl	24(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	24(%esp), %eax
	addl	$40, %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$array, 28(%esp)
	movl	28(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$40, 28(%esp)
	movl	28(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	addl	$40, 28(%esp)
	movl	28(%esp), %eax
	movl	(%eax), %edx
	movl	$.LC10, %eax
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
