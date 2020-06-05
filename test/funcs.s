	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$4, %edi
	callq	f
	movl	%eax, a(%rip)
	movl	$.L__unnamed_1, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	printf
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.globl	f
	.p2align	4, 0x90
	.type	f,@function
f:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, 4(%rsp)
	callq	g
	addl	$2, %eax
	movl	%eax, (%rsp)
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	f, .Lfunc_end1-f
	.cfi_endproc

	.globl	g
	.p2align	4, 0x90
	.type	g,@function
g:
	.cfi_startproc
	movl	%edi, -4(%rsp)
	leal	(%rdi,%rdi,2), %eax
	movl	%eax, -8(%rsp)
	retq
.Lfunc_end2:
	.size	g, .Lfunc_end2-g
	.cfi_endproc

	.type	a,@object
	.bss
	.globl	a
	.p2align	2
a:
	.long	0
	.size	a, 4

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%d\n"
	.size	.L__unnamed_1, 4


	.section	".note.GNU-stack","",@progbits
