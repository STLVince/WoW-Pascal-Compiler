	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$5, %edi
	callq	fib
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

	.globl	fib
	.p2align	4, 0x90
	.type	fib,@function
fib:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movl	%edi, 12(%rsp)
	cmpl	$2, %edi
	jg	.LBB1_2
	movl	$1, 8(%rsp)
	jmp	.LBB1_3
.LBB1_2:
	movl	12(%rsp), %edi
	decl	%edi
	callq	fib
	movl	%eax, %ebx
	movl	12(%rsp), %edi
	addl	$-2, %edi
	callq	fib
	addl	%ebx, %eax
	movl	%eax, 8(%rsp)
.LBB1_3:
	movl	8(%rsp), %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	fib, .Lfunc_end1-fib
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
	.asciz	"%d"
	.size	.L__unnamed_1, 3


	.section	".note.GNU-stack","",@progbits
