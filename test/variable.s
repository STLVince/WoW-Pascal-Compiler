	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movabsq	$-4620693217682128896, %rax
	movq	%rax, r(%rip)
	movb	$48, c(%rip)
	movl	$-2, x+8(%rip)
	movl	x+4(%rip), %eax
	movl	%eax, a(%rip)
	cmpl	$1, b(%rip)
	jne	.LBB0_2
	movl	$6, a(%rip)
	jmp	.LBB0_3
.LBB0_2:
	movl	$9, a(%rip)
.LBB0_3:
	movsd	r(%rip), %xmm0
	movl	$.L__unnamed_1, %edi
	movb	$1, %al
	callq	printf
	movl	x+8(%rip), %esi
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	printf
	movl	x(%rip), %esi
	movl	$.L__unnamed_3, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_4, %edi
	movl	$__unnamed_5, %esi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_6, %edi
	movl	$teststr, %esi
	xorl	%eax, %eax
	callq	printf
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	b,@object
	.section	.rodata,"a",@progbits
	.globl	b
	.p2align	2
b:
	.long	1
	.size	b, 4

	.type	teststr,@object
	.globl	teststr
teststr:
	.asciz	"aaa"
	.size	teststr, 4

	.type	a,@object
	.bss
	.globl	a
	.p2align	2
a:
	.long	0
	.size	a, 4

	.type	b.1,@object
	.globl	b.1
	.p2align	2
b.1:
	.long	0
	.size	b.1, 4

	.type	r,@object
	.globl	r
	.p2align	3
r:
	.quad	0
	.size	r, 8

	.type	bool,@object
	.globl	bool
bool:
	.byte	0
	.size	bool, 1

	.type	c,@object
	.globl	c
c:
	.byte	0
	.size	c, 1

	.type	s,@object
	.globl	s
s:
	.zero	1
	.size	s, 1

	.type	x,@object
	.globl	x
	.p2align	2
x:
	.zero	12
	.size	x, 12

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%f\n"
	.size	.L__unnamed_1, 4

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"%d\n"
	.size	.L__unnamed_2, 4

	.type	.L__unnamed_3,@object
.L__unnamed_3:
	.asciz	"%d\n"
	.size	.L__unnamed_3, 4

	.type	__unnamed_5,@object
	.section	.rodata,"a",@progbits
	.globl	__unnamed_5
__unnamed_5:
	.asciz	"aa"
	.size	__unnamed_5, 3

	.type	.L__unnamed_4,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_4:
	.asciz	"%s\n"
	.size	.L__unnamed_4, 4

	.type	.L__unnamed_6,@object
.L__unnamed_6:
	.asciz	"%s\n"
	.size	.L__unnamed_6, 4


	.section	".note.GNU-stack","",@progbits
