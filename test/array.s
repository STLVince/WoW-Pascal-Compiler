	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$1, a+4(%rip)
	movl	$3, a+12(%rip)
	movl	$.L__unnamed_1, %edi
	movl	$3, %esi
	xorl	%eax, %eax
	callq	printf
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	r,@object
	.bss
	.globl	r
	.p2align	3
r:
	.quad	0
	.size	r, 8

	.type	a,@object
	.globl	a
	.p2align	4
a:
	.zero	44
	.size	a, 44

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%d\n"
	.size	.L__unnamed_1, 4


	.section	".note.GNU-stack","",@progbits
