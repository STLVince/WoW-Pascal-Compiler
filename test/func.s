	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$1, %edi
	callq	max
	movl	%eax, a(%rip)
	popq	%rax
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
	movl	%edi, -4(%rsp)
	movl	%edi, -8(%rsp)
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


	.section	".note.GNU-stack","",@progbits
