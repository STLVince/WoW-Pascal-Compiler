	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$1, %edi
	callq	max
	movl	%eax, max_a(%rip)
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.globl	max
	.p2align	4, 0x90
	.type	max,@function
max:
	.cfi_startproc
	movl	%edi, %eax
	movl	%edi, max_a(%rip)
	movl	%edi, max_max(%rip)
	retq
.Lfunc_end1:
	.size	max, .Lfunc_end1-max
	.cfi_endproc

	.type	a,@object
	.bss
	.globl	a
	.p2align	2
a:
	.long	0
	.size	a, 4

	.type	max_a,@object
	.globl	max_a
	.p2align	2
max_a:
	.long	0
	.size	max_a, 4

	.type	max_max,@object
	.globl	max_max
	.p2align	2
max_max:
	.long	0
	.size	max_max, 4


	.section	".note.GNU-stack","",@progbits
