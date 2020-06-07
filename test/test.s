	.text
	.file	"Pascal"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$3, n(%rip)
	movl	$10, v(%rip)
	movl	$1, c+4(%rip)
	movl	$2, w+4(%rip)
	movabsq	$25769803780, %rax
	movq	%rax, c+8(%rip)
	movabsq	$34359738374, %rax
	movq	%rax, w+8(%rip)
	movl	$1, i(%rip)
	jmp	.LBB0_1
	.p2align	4, 0x90
.LBB0_6:
	incl	i(%rip)
.LBB0_1:
	movl	i(%rip), %eax
	cmpl	n(%rip), %eax
	jg	.LBB0_8
	movl	i(%rip), %esi
	movl	$.L__unnamed_1, %edi
	xorl	%eax, %eax
	callq	printf
	movl	v(%rip), %eax
	movl	%eax, j(%rip)
	jmp	.LBB0_3
	.p2align	4, 0x90
.LBB0_4:
	movslq	j(%rip), %rax
	movslq	i(%rip), %rcx
	movl	c(,%rcx,4), %edx
	movl	%eax, %esi
	subl	%edx, %esi
	movslq	%esi, %rdx
	movl	f(,%rdx,4), %edx
	addl	w(,%rcx,4), %edx
	movl	%edx, f(,%rax,4)
.LBB0_5:
	decl	j(%rip)
	movslq	i(%rip), %rax
	movl	c(,%rax,4), %eax
	decl	%eax
	cmpl	%eax, j(%rip)
	je	.LBB0_6
.LBB0_3:
	movslq	j(%rip), %rax
	movslq	i(%rip), %rcx
	movl	c(,%rcx,4), %edx
	movl	%eax, %esi
	subl	%edx, %esi
	movslq	%esi, %rdx
	movl	f(,%rdx,4), %edx
	addl	w(,%rcx,4), %edx
	cmpl	f(,%rax,4), %edx
	jg	.LBB0_4
	movslq	j(%rip), %rax
	movl	f(,%rax,4), %esi
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	printf
	jmp	.LBB0_5
.LBB0_8:
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	i,@object
	.bss
	.globl	i
	.p2align	2
i:
	.long	0
	.size	i, 4

	.type	j,@object
	.globl	j
	.p2align	2
j:
	.long	0
	.size	j, 4

	.type	n,@object
	.globl	n
	.p2align	2
n:
	.long	0
	.size	n, 4

	.type	v,@object
	.globl	v
	.p2align	2
v:
	.long	0
	.size	v, 4

	.type	c,@object
	.globl	c
	.p2align	4
c:
	.zero	404
	.size	c, 404

	.type	w,@object
	.globl	w
	.p2align	4
w:
	.zero	404
	.size	w, 404

	.type	f,@object
	.globl	f
	.p2align	4
f:
	.zero	404
	.size	f, 404

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%d\n"
	.size	.L__unnamed_1, 4

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"%d\n"
	.size	.L__unnamed_2, 4


	.section	".note.GNU-stack","",@progbits
