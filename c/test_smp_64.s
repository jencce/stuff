	.file	"ts.c"
.globl i
	.bss
	.align 8
	.type	i, @object
	.size	i, 8
i:
	.zero	8
	.text
.globl main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	i(%rip), %rax
	movl	%eax, -4(%rbp)
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.5 20110214 (Red Hat 4.4.5-6)"
	.section	.note.GNU-stack,"",@progbits
