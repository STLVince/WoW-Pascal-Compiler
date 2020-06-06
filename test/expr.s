	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	movslq	a(%rip), %rax
	movabsq	$4612136378390124954, %rcx
	movq	%rcx, f(%rip)
	movl	$3, g(%rip)
	movb	$0, i(%rip)
	imulq	$1431655766, %rax, %rcx
	movq	%rcx, %rdx
	shrq	$63, %rdx
	shrq	$32, %rcx
	addl	%edx, %ecx
	leal	(%rcx,%rcx,2), %ecx
	subl	%ecx, %eax
	movl	%eax, e(%rip)
	movabsq	$4294967296, %rax
	movq	%rax, j(%rip)
	movl	$2, j+8(%rip)
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.globl	test
	.p2align	4, 0x90
	.type	test,@function
test:
	.cfi_startproc
	movl	$3, -4(%rsp)
	movl	$3, %eax
	retq
.Lfunc_end1:
	.size	test, .Lfunc_end1-test
	.cfi_endproc

	.type	y,@object
	.section	.rodata,"a",@progbits
	.globl	y
	.p2align	2
y:
	.long	33
	.size	y, 4

	.type	a,@object
	.globl	a
	.p2align	2
a:
	.long	1
	.size	a, 4

	.type	b,@object
	.globl	b
	.p2align	3
b:
	.quad	4612136378390124954
	.size	b, 8

	.type	c,@object
	.globl	c
c:
	.asciz	"string"
	.size	c, 7

	.type	d,@object
	.globl	d
d:
	.byte	0
	.size	d, 1

	.type	e,@object
	.bss
	.globl	e
	.p2align	2
e:
	.long	0
	.size	e, 4

	.type	f,@object
	.globl	f
	.p2align	3
f:
	.quad	0
	.size	f, 8

	.type	g,@object
	.globl	g
	.p2align	2
g:
	.long	0
	.size	g, 4

	.type	h,@object
	.globl	h
h:
	.zero	1
	.size	h, 1

	.type	i,@object
	.globl	i
i:
	.byte	0
	.size	i, 1

	.type	j,@object
	.globl	j
	.p2align	2
j:
	.zero	12
	.size	j, 12

	.type	k,@object
	.globl	k
	.p2align	2
k:
	.long	0
	.size	k, 4


	.section	".note.GNU-stack","",@progbits
