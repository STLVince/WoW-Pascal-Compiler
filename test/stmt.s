	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$1, d(%rip)
	cmpl	$1, a(%rip)
	jne	.LBB0_8
	movl	$1, d(%rip)
	cmpl	$10, d(%rip)
	jle	.LBB0_3
	jmp	.LBB0_9
	.p2align	4, 0x90
.LBB0_7:
	incl	d(%rip)
	cmpl	$10, d(%rip)
	jg	.LBB0_9
.LBB0_3:
	movl	$1, b(%rip)
	.p2align	4, 0x90
.LBB0_4:
	cmpl	$9, b(%rip)
	jg	.LBB0_7
	incl	b(%rip)
	movl	$1, c(%rip)
	.p2align	4, 0x90
.LBB0_6:
	movl	c(%rip), %eax
	incl	%eax
	movl	%eax, c(%rip)
	cmpl	$9, %eax
	jle	.LBB0_6
	jmp	.LBB0_4
.LBB0_8:
	movl	$.L__unnamed_1, %edi
	movl	$str, %esi
	xorl	%eax, %eax
	callq	printf
.LBB0_9:
	movl	a(%rip), %esi
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_3, %edi
	movl	$__unnamed_4, %esi
	xorl	%eax, %eax
	callq	printf
	movl	b(%rip), %esi
	movl	$.L__unnamed_5, %edi
	xorl	%eax, %eax
	callq	printf
	movl	c(%rip), %esi
	movl	$.L__unnamed_6, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_7, %edi
	movl	$str, %esi
	xorl	%eax, %eax
	callq	printf
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	a,@object
	.section	.rodata,"a",@progbits
	.globl	a
	.p2align	2
a:
	.long	1
	.size	a, 4

	.type	str,@object
	.globl	str
str:
	.asciz	"hello"
	.size	str, 6

	.type	b,@object
	.bss
	.globl	b
	.p2align	2
b:
	.long	0
	.size	b, 4

	.type	c,@object
	.globl	c
	.p2align	2
c:
	.long	0
	.size	c, 4

	.type	d,@object
	.globl	d
	.p2align	2
d:
	.long	0
	.size	d, 4

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%s\n"
	.size	.L__unnamed_1, 4

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"%d\n"
	.size	.L__unnamed_2, 4

	.type	__unnamed_4,@object
	.section	.rodata,"a",@progbits
	.globl	__unnamed_4
__unnamed_4:
	.asciz	"b="
	.size	__unnamed_4, 3

	.type	.L__unnamed_3,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_3:
	.asciz	"%s"
	.size	.L__unnamed_3, 3

	.type	.L__unnamed_5,@object
.L__unnamed_5:
	.asciz	"%d\n"
	.size	.L__unnamed_5, 4

	.type	.L__unnamed_6,@object
.L__unnamed_6:
	.asciz	"%d\n"
	.size	.L__unnamed_6, 4

	.type	.L__unnamed_7,@object
.L__unnamed_7:
	.asciz	"%s\n"
	.size	.L__unnamed_7, 4


	.section	".note.GNU-stack","",@progbits
