	.file	"test_env.c"
	.globl	static_variable
	.data
	.align 4
	.type	static_variable, @object
	.size	static_variable, 4
static_variable:
	.long	5
	.text
	.globl	f
	.type	f, @function
f:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$44, %esp
	movl	$1, %esi
	.cfi_offset 3, -20
	.cfi_offset 6, -16
	.cfi_offset 7, -12
	movl	$10, %edi
	movl	$110, %ebx
	movl	$1, a_lang_lang_lang_lang
	movl	%edi, 8(%esp)
	movl	%esi, 4(%esp)
	movl	$10, (%esp)
	call	func_ret_int
	call	func_ret_double
	fstpl	-32(%ebp)
	movl	%ebx, (%esp)
	call	func_ret_char_ptr
	movl	%eax, %ebx
	addl	$44, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.globl	func_ret_int
	.type	func_ret_int, @function
func_ret_int:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	16(%ebp), %ecx
	movl	12(%ebp), %eax
	subl	$6, %eax
	movl	%eax, -4(%ebp)
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	addl	%edx, %eax
	addl	%ecx, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	func_ret_int, .-func_ret_int
	.globl	func_ret_double
	.type	func_ret_double, @function
func_ret_double:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	fldl	.LC1
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
.LFE2:
	.size	func_ret_double, .-func_ret_double
	.globl	func_ret_char_ptr
	.type	func_ret_char_ptr, @function
func_ret_char_ptr:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	addl	$1, %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
.LFE3:
	.size	func_ret_char_ptr, .-func_ret_char_ptr
	.section	.rodata
	.align 8
.LC1:
	.long	1374389535
	.long	1073815224
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
