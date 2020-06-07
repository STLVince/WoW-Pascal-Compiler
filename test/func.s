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
	movl	$.L__unnamed_1, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	printf
	movl	$3, %edi
	callq	max
	movl	$.L__unnamed_2, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_3, %edi
	xorl	%eax, %eax
	callq	printf
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

	.globl	wow
	.p2align	4, 0x90
	.type	wow,@function
wow:
	.cfi_startproc
	movl	$5, -4(%rsp)
	movl	$5, %eax
	retq
.Lfunc_end2:
	.size	wow, .Lfunc_end2-wow
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

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"%d\n"
	.size	.L__unnamed_2, 4

	.type	.L__unnamed_3,@object
.L__unnamed_3:
	.asciz	"\n"
	.size	.L__unnamed_3, 2


	.section	".note.GNU-stack","",@progbits
